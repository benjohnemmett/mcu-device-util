#include "mpu_6050.h"

void Mpu_6050_initialize(I2cFunctions *i2c_functions) {
    i2c_functions->f_I2cInitialize();
    
    i2c_functions->f_I2cWriteByte(MPU_6050_ADDR, MPU_6050_PWR_MGMT_1, 0x00);
    i2c_functions->f_I2cWriteByte(MPU_6050_ADDR, MPU_6050_CONFIG, 0x01);
    i2c_functions->f_I2cWriteByte(MPU_6050_ADDR, MPU_6050_GYRO_CONFIG, 0x00);
    i2c_functions->f_I2cWriteByte(MPU_6050_ADDR, MPU_6050_ACCEL_CONFIG, 0x00);
    i2c_functions->f_I2cWriteByte(MPU_6050_ADDR, MPU_6050_INT_ENABLE, 0x00);
    i2c_functions->f_I2cWriteByte(MPU_6050_ADDR, MPU_6050_SIGNAL_PATH_RESET, 0x00);
}

void ReadGyroscope(I2cFunctions *i2c_functions, uint8_t addr, float lsb_sensitivity, GyroscopeData *data_out) {
    uint8_t data_h, data_l;
    int16_t raw_value;
    
    i2c_functions->f_I2cSendStart(addr, I2C_WRITE);
    i2c_functions->f_I2cWrite(MPU_6050_GYRO_XOUT_H);
    i2c_functions->f_I2cSendStart(addr, I2C_READ);
    
    data_h = i2c_functions->f_I2cRead(I2C_ACK);
    data_l = i2c_functions->f_I2cRead(I2C_ACK);
    raw_value = (data_h << 8) | data_l;
    data_out->x_deg_per_second = (raw_value / lsb_sensitivity);
    
    data_h = i2c_functions->f_I2cRead(I2C_ACK);
    data_l = i2c_functions->f_I2cRead(I2C_ACK);
    raw_value = (data_h << 8) | data_l;
    data_out->y_deg_per_second = (raw_value / lsb_sensitivity);
    
    data_h = i2c_functions->f_I2cRead(I2C_ACK);
    data_l = i2c_functions->f_I2cRead(I2C_ACK);
    raw_value = (data_h << 8) | data_l;
    data_out->z_deg_per_second = (raw_value / lsb_sensitivity);
    
    i2c_functions->f_I2cSendStop();
}

void ReadAccelerometer(I2cFunctions *i2c_functions, uint8_t addr, float lsb_sensitivity, AccelerationData *data_out) {
    uint8_t data_h, data_l;
    int16_t raw_value;
    
    i2c_functions->f_I2cSendStart(addr, I2C_WRITE);
    i2c_functions->f_I2cWrite(MPU_6050_ACCEL_XOUT_H);
    i2c_functions->f_I2cSendStart(addr, I2C_READ);
    
    data_h = i2c_functions->f_I2cRead(I2C_ACK);
    data_l = i2c_functions->f_I2cRead(I2C_ACK);
    raw_value = (data_h << 8) | data_l;
    data_out->x_mm_per_sec_squared = (raw_value / lsb_sensitivity) * GRAVITATIONAL_ACC;
    
    data_h = i2c_functions->f_I2cRead(I2C_ACK);
    data_l = i2c_functions->f_I2cRead(I2C_ACK);
    raw_value = (data_h << 8) | data_l;
    data_out->y_mm_per_sec_squared = (raw_value / lsb_sensitivity) * GRAVITATIONAL_ACC;
    
    data_h = i2c_functions->f_I2cRead(I2C_ACK);
    data_l = i2c_functions->f_I2cRead(I2C_ACK);
    raw_value = (data_h << 8) | data_l;
    data_out->z_mm_per_sec_squared = (raw_value / lsb_sensitivity) * GRAVITATIONAL_ACC;
    
    i2c_functions->f_I2cSendStop();
}
