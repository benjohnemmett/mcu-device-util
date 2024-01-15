#include "bmp390.h"

void Bmp390Initialize(I2cFunctions *i2c_functions, char addr, Bmp390CalibrationData *calibration_data) {
    i2c_functions->f_I2cWriteByte(addr, BMP390_REG_PWR_CTRL, BMP390_PWR_CTRL_START_EVERYTHING_bm);
    Bmp390RawTrimmingCoefficients trimming_coefficients;
    Bmp390GetTrimmingCoefficients(i2c_functions, addr, &trimming_coefficients);
    Bmp390CalculateCalibrationData(&trimming_coefficients, calibration_data);
}

void Bmp390GetTrimmingCoefficients(I2cFunctions *i2c_functions, char addr, Bmp390RawTrimmingCoefficients *trimming_coefficients) {
    i2c_functions->f_I2cReadBytes(addr, BMP390_REG_NVM_PAR_T1, (uint8_t*)trimming_coefficients, BMP390_TRIMMING_COEFFICENT_LENGTH);
}

/*
 From the BMP390 datasheet appendix section 8.4
 */
void Bmp390CalculateCalibrationData(Bmp390RawTrimmingCoefficients *raw_trimming_coefficients, Bmp390CalibrationData *calibration_data) {
    calibration_data->t1 = raw_trimming_coefficients->t1 * exp2(8);
    calibration_data->t2 = raw_trimming_coefficients->t2 / exp2(30);
    calibration_data->t3 = raw_trimming_coefficients->t3 / exp2(48);
    calibration_data->p1 = (raw_trimming_coefficients->p1 - exp2(14)) / exp2(20);
    calibration_data->p2 = (raw_trimming_coefficients->p2 - exp2(14)) / exp2(29);
    calibration_data->p3 = raw_trimming_coefficients->p3 / exp2(32);
    calibration_data->p4 = raw_trimming_coefficients->p4 / exp2(37);
    calibration_data->p5 = raw_trimming_coefficients->p5 * exp2(3);
    calibration_data->p6 = raw_trimming_coefficients->p6 / exp2(6);
    calibration_data->p7 = raw_trimming_coefficients->p7 / exp2(8);
    calibration_data->p8 = raw_trimming_coefficients->p8 / exp2(15);
    calibration_data->p9 = raw_trimming_coefficients->p9 / exp2(48);
    calibration_data->p10 = raw_trimming_coefficients->p10 / exp2(48);
    calibration_data->p11 = raw_trimming_coefficients->p11 / exp2(65);
}

uint32_t Bmp390ReadTemperatureRaw(I2cFunctions *i2c_functions, unsigned char addr) {
    i2c_functions->f_I2cSendStart(addr, I2C_WRITE);
    i2c_functions->f_I2cWrite(BMP390_REG_TEMP_DATA_0);
    i2c_functions->f_I2cSendStart(addr, I2C_READ);
    unsigned long lsb = i2c_functions->f_I2cRead(I2C_ACK);
    unsigned long data1 = i2c_functions->f_I2cRead(I2C_ACK);
    unsigned long msb = i2c_functions->f_I2cRead(I2C_NACK);
    i2c_functions->f_I2cSendStop();
    unsigned long raw_temp = (msb << 16) | (data1 << 8) | lsb;
    
    return raw_temp;
}

/*
 From the BMP390 datasheet appendix section 8.5
 */
float Bmp390CompensateTempterature(uint32_t uncomp_temp, Bmp390CalibrationData *calibration_data) {
    float partial_data1;
    float partial_data2;
    
    partial_data1 = (float)(uncomp_temp - calibration_data->t1);
    partial_data2 = (float)(partial_data1 * calibration_data->t2);
    
    calibration_data->t_lin = partial_data2 + (partial_data1 * partial_data1) * calibration_data->t3;
    
    return calibration_data->t_lin;
}

float Bmp390ReadTemperatureInC(I2cFunctions *i2c_functions, unsigned char addr, Bmp390CalibrationData *calibration_data) {
    uint32_t raw_temperature = Bmp390ReadTemperatureRaw(i2c_functions, addr);
    float temperature_c = Bmp390CompensateTempterature(raw_temperature, calibration_data);
    
    return temperature_c;
}

uint32_t Bmp390ReadPressureRaw(I2cFunctions *i2c_functions, unsigned char addr) {
    i2c_functions->f_I2cSendStart(addr, I2C_WRITE);
    i2c_functions->f_I2cWrite(BMP390_REG_PRESSURE_DATA_0);
    i2c_functions->f_I2cSendStart(addr, I2C_READ);
    unsigned long lsb = i2c_functions->f_I2cRead(I2C_ACK);
    unsigned long data1 = i2c_functions->f_I2cRead(I2C_ACK);
    unsigned long msb = i2c_functions->f_I2cRead(I2C_NACK);
    i2c_functions->f_I2cSendStop();
    unsigned long raw_pressure = (msb << 16) | (data1 << 8) | lsb;
    
    return raw_pressure;
}
/* 
  From the BMP390 datasheet appendix section 8.6
 */
float Bmp390CompensatePressure(uint32_t uncomp_press, Bmp390CalibrationData *calibration_data) {
    float comp_press;
    float partial_data1;
    float partial_data2;
    float partial_data3;
    float partial_data4;
    float partial_out1;
    float partial_out2;
    
    partial_data1 = calibration_data->p6 * calibration_data->t_lin;
    partial_data2 = calibration_data->p7 * (calibration_data->t_lin * calibration_data->t_lin);
    partial_data3 = calibration_data->p8 * (calibration_data->t_lin * calibration_data->t_lin * calibration_data->t_lin);
    partial_out1 = calibration_data->p5 + partial_data1 + partial_data2 + partial_data3;
    
    partial_data1 = calibration_data->p2 * calibration_data->t_lin;
    partial_data2 = calibration_data->p3 * (calibration_data->t_lin * calibration_data->t_lin);
    partial_data3 = calibration_data->p4 * (calibration_data->t_lin * calibration_data->t_lin * calibration_data->t_lin);
    partial_out2 = (float)uncomp_press * (calibration_data->p1 + partial_data1 + partial_data2 + partial_data3);
    
    partial_data1 = (float)uncomp_press * (float)uncomp_press;
    partial_data2 = calibration_data->p9 + calibration_data->p10 * calibration_data->t_lin;
    partial_data3 = partial_data1 * partial_data2;
    partial_data4 = partial_data3 + ((float)uncomp_press * (float)uncomp_press * (float)uncomp_press) * calibration_data->p11;
    comp_press = partial_out1 + partial_out2 + partial_data4;
    
    return comp_press;
}

float Bmp390ReadPressureInPa(I2cFunctions *i2c_functions, unsigned char addr, Bmp390CalibrationData *calibration_data) {
    unsigned long uncomp_pressure = Bmp390ReadPressureRaw(i2c_functions, addr);
    float pressure_pa = Bmp390CompensatePressure(uncomp_pressure, calibration_data);
    
    return pressure_pa;
}

void Bmp390SetOverSampleRates(I2cFunctions *i2c_functions, unsigned char addr, unsigned char osr_masks) {
    i2c_functions->f_I2cWriteByte(addr, BMP390_REG_OSR, osr_masks);
}

void Bmp390SetIirFilterCoefficient(I2cFunctions *i2c_functions, unsigned char addr, unsigned char iir_coeff_mask) {
    i2c_functions->f_I2cWriteByte(addr, BMP390_REG_CONFIG, iir_coeff_mask);
}

void Bmp390SetOutputDataRate(I2cFunctions *i2c_functions, unsigned char addr, unsigned char odr_sel_mask) {
    i2c_functions->f_I2cWriteByte(addr, BMP390_REG_ODR, odr_sel_mask);
}
