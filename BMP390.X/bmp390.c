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

void Bmp390ReadTemperatureRaw(I2cFunctions *i2c_functions, unsigned char addr, RawTempData *data) {
    i2c_functions->f_I2cSendStart(addr, I2C_WRITE);
    i2c_functions->f_I2cWrite(BMP390_REG_TEMP_DATA_0);
    i2c_functions->f_I2cSendStart(addr, I2C_READ);
    data->data0 = i2c_functions->f_I2cRead(I2C_ACK);
    data->data1 = i2c_functions->f_I2cRead(I2C_ACK);
    data->data2 = i2c_functions->f_I2cRead(I2C_NACK);
    i2c_functions->f_I2cSendStop();
}

uint32_t Bmp390GetRawTemperatureRawBytes(RawTempData *data) {
    unsigned long lsb = (unsigned long)data->data0;
    unsigned long data1 = (unsigned long)data->data1;
    unsigned long msb = (unsigned long)data->data2;
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
    RawTempData raw_data;
    Bmp390ReadTemperatureRaw(i2c_functions, addr, &raw_data);
    uint32_t raw_temperature = Bmp390GetRawTemperatureRawBytes(&raw_data);
    float temperature_c = Bmp390CompensateTempterature(raw_temperature, calibration_data);
    
    return temperature_c;
}

float Bmp390ReadPressureInPa(I2cFunctions *i2c_functions, unsigned char addr) {
    i2c_functions->f_I2cSendStart(addr, I2C_WRITE);
    i2c_functions->f_I2cWrite(BMP390_REG_PRESSURE_DATA_0);
    i2c_functions->f_I2cSendStart(addr, I2C_READ);
    unsigned char data0 = i2c_functions->f_I2cRead(I2C_ACK);
    unsigned long data1 = i2c_functions->f_I2cRead(I2C_ACK);
    unsigned long data2 = i2c_functions->f_I2cRead(I2C_NACK);
    i2c_functions->f_I2cSendStop();
    
    unsigned long raw_pressure = (data2 << 16) | (data1 << 8) | (data0);
    float pressure_pa =  ((float) raw_pressure) * 2.64;
    
    return pressure_pa;
}

void Bmp390SetOverSampleRates(I2cFunctions *i2c_functions, unsigned char addr, unsigned char osr_masks) {
    
}

void Bmp390SetIirFilterCoefficient(I2cFunctions *i2c_functions, unsigned char addr, unsigned char iir_coeff_mask) {
    
}

void Bmp390SetOutputDataRate(I2cFunctions *i2c_functions, unsigned char addr, unsigned char odr_sel_mask) {
    
}
