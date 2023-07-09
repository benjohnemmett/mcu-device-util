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

void ReadAccelerations(I2cFunctions *i2c_functions, uint8_t addr, int16_t *buffer) {
    
    uint8_t data_h, data_l;
    
    i2c_functions->f_I2cSendStart(addr, I2C_WRITE);
    i2c_functions->f_I2cWrite(MPU_6050_ACCEL_XOUT_H);
    i2c_functions->f_I2cSendStart(addr, I2C_READ);
    
    data_h = i2c_functions->f_I2cRead(I2C_ACK);
    data_l = i2c_functions->f_I2cRead(I2C_ACK);
    buffer[0] = (data_h << 8) | data_l;
    data_h = i2c_functions->f_I2cRead(I2C_ACK);
    data_l = i2c_functions->f_I2cRead(I2C_ACK);
    buffer[1] = (data_h << 8) | data_l;
    data_h = i2c_functions->f_I2cRead(I2C_ACK);
    data_l = i2c_functions->f_I2cRead(I2C_NACK);
    buffer[2] = (data_h << 8) | data_l;
    
    i2c_functions->f_I2cSendStop();
}

void RawAccelerationToMetersPerSecondSquared(int16_t *raw_accel, float *converted_accel) {
    converted_accel[0] = (raw_accel[0] / 16384.) * 9.8;
    converted_accel[1] = (raw_accel[1] / 16384.) * 9.8;
    converted_accel[2] = (raw_accel[2] / 16384.) * 9.8;
}

