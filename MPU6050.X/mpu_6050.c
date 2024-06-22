#include "mpu_6050.h"

void Mpu_6050_initialize() {
    I2cInitialize();
    
    I2cWriteByte(MPU_6050_ADDR, MPU_6050_PWR_MGMT_1, 0x00);
    I2cWriteByte(MPU_6050_ADDR, MPU_6050_CONFIG, 0x01);
    I2cWriteByte(MPU_6050_ADDR, MPU_6050_GYRO_CONFIG, 0x00);
    I2cWriteByte(MPU_6050_ADDR, MPU_6050_ACCEL_CONFIG, 0x00);
    I2cWriteByte(MPU_6050_ADDR, MPU_6050_INT_ENABLE, 0x00);
    I2cWriteByte(MPU_6050_ADDR, MPU_6050_SIGNAL_PATH_RESET, 0x00);
}

void ReadGyroscope(uint8_t addr, float lsb_sensitivity, GyroscopeData *data_out) {
    uint8_t data_h, data_l;
    int16_t raw_value;
    
    I2cSendStart(addr, I2C_WRITE);
    I2cWrite(MPU_6050_GYRO_XOUT_H);
    I2cSendStart(addr, I2C_READ);
    
    data_h = I2cRead(I2C_ACK);
    data_l = I2cRead(I2C_ACK);
    raw_value = (data_h << 8) | data_l;
    data_out->x_deg_per_second = (raw_value / lsb_sensitivity);
    
    data_h = I2cRead(I2C_ACK);
    data_l = I2cRead(I2C_ACK);
    raw_value = (data_h << 8) | data_l;
    data_out->y_deg_per_second = (raw_value / lsb_sensitivity);
    
    data_h = I2cRead(I2C_ACK);
    data_l = I2cRead(I2C_ACK);
    raw_value = (data_h << 8) | data_l;
    data_out->z_deg_per_second = (raw_value / lsb_sensitivity);
    
    I2cSendStop();
}

void ReadAccelerometer(uint8_t addr, float lsb_sensitivity, AccelerationData *data_out) {
    uint8_t data_h, data_l;
    int16_t raw_value;
    
    I2cSendStart(addr, I2C_WRITE);
    I2cWrite(MPU_6050_ACCEL_XOUT_H);
    I2cSendStart(addr, I2C_READ);
    
    data_h = I2cRead(I2C_ACK);
    data_l = I2cRead(I2C_ACK);
    raw_value = (data_h << 8) | data_l;
    data_out->x_mm_per_sec_squared = (raw_value / lsb_sensitivity) * GRAVITATIONAL_ACC;
    
    data_h = I2cRead(I2C_ACK);
    data_l = I2cRead(I2C_ACK);
    raw_value = (data_h << 8) | data_l;
    data_out->y_mm_per_sec_squared = (raw_value / lsb_sensitivity) * GRAVITATIONAL_ACC;
    
    data_h = I2cRead(I2C_ACK);
    data_l = I2cRead(I2C_ACK);
    raw_value = (data_h << 8) | data_l;
    data_out->z_mm_per_sec_squared = (raw_value / lsb_sensitivity) * GRAVITATIONAL_ACC;
    
    I2cSendStop();
}

void SetAccelerometerRange(uint8_t addr, uint8_t range_bitmask) {
    I2cWriteByte(MPU_6050_ADDR, MPU_6050_ACCEL_CONFIG, range_bitmask);
}

void SetGyroscopeRange(uint8_t addr, uint8_t range_bitmask) {
    I2cWriteByte(MPU_6050_ADDR, MPU_6050_GYRO_CONFIG, range_bitmask);
}

void SetDigitalLowPassFilter(uint8_t addr, uint8_t dlpf_config_bitmask) {
    I2cWriteByte(MPU_6050_ADDR, MPU_6050_CONFIG, dlpf_config_bitmask);
}