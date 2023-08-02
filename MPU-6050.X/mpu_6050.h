/* 
 * File:   mpu_6050.h
 * Author: ben
 *
 * Created on September 3, 2022, 10:27 PM
 */

#ifndef MPU_6050_H
#define	MPU_6050_H

#include "../I2C.X/i2c_interface.h"
#include "../I2C.X/i2c.h"


#ifndef __DEFINED_uint8_t
typedef unsigned char uint8_t;
#define __DEFINED_uint8_t
#endif

#ifndef __DEFINED_int16_t 
typedef signed int int16_t;
#define __DEFINED_int16_t
#endif

#ifndef __DEFINED_uint16_t 
typedef unsigned int uint16_t;
#define __DEFINED_uint16_t
#endif

#define GRAVITATIONAL_ACC 9.8

#define MPU_6050_ADDR 0x68
#define MPU_6050_WHO_AM_I 0x75
#define MPU_6050_ACCEL_XOUT_H 0x3B
#define MPU_6050_ACCEL_XOUT_L 0x3C
#define MPU_6050_ACCEL_YOUT_H 0x3D
#define MPU_6050_ACCEL_YOUT_L 0x3E
#define MPU_6050_ACCEL_ZOUT_H 0x3F
#define MPU_6050_ACCEL_ZOUT_L 0x40
#define MPU_6050_GYRO_XOUT_H 0x43
#define MPU_6050_GYRO_XOUT_L 0x44
#define MPU_6050_GYRO_YOUT_H 0x45
#define MPU_6050_GYRO_YOUT_L 0x46
#define MPU_6050_GYRO_ZOUT_H 0x47
#define MPU_6050_GYRO_ZOUT_L 0x48

#define MPU_6050_PWR_MGMT_1 0x6B
#define MPU_6050_CONFIG 0x1A
#define MPU_6050_GYRO_CONFIG 0x1B
#define MPU_6050_ACCEL_CONFIG 0x1C
#define MPU_6050_INT_ENABLE 0x38
#define MPU_6050_SIGNAL_PATH_RESET 0x68

#define MPU_6050_GYRO_RANGE_250_bm 0x00
#define MPU_6050_GYRO_RANGE_500_bm 0x01 << 3
#define MPU_6050_GYRO_RANGE_1000_bm 0x02 << 3
#define MPU_6050_GYRO_RANGE_2000_bm 0x03 << 3

#define MPU_6050_ACCEL_RANGE_2G_bm 0x00
#define MPU_6050_ACCEL_RANGE_4G_bm 0x01 << 3
#define MPU_6050_ACCEL_RANGE_8G_bm 0x02 << 3
#define MPU_6050_ACCEL_RANGE_16G_bm 0x03 << 3

#define MPU_6050_DLPF_ACC_BW_260_HZ_bm 0x00
#define MPU_6050_DLPF_ACC_BW_184_HZ_bm 0x01
#define MPU_6050_DLPF_ACC_BW_94_HZ_bm 0x02
#define MPU_6050_DLPF_ACC_BW_44_HZ_bm 0x03
#define MPU_6050_DLPF_ACC_BW_21_HZ_bm 0x04
#define MPU_6050_DLPF_ACC_BW_10_HZ_bm 0x05
#define MPU_6050_DLPF_ACC_BW_5_HZ_bm 0x06

#define ACC_LSB_2G 16384.
#define ACC_LSB_4G 8192.
#define ACC_LSB_8G 4096.
#define ACC_LSB_16G 2048.
#define GYRO_LSB_250_DEG_PER_SEC 131
#define GYRO_LSB_500_DEG_PER_SEC 65.5
#define GYRO_LSB_1000_DEG_PER_SEC 32.8
#define GYRO_LSB_2000_DEG_PER_SEC 16.4

#ifdef	__cplusplus
extern "C" {
#endif
    
    struct GyroscopeData {
        int16_t x_deg_per_second;
        int16_t y_deg_per_second;
        int16_t z_deg_per_second;
    };
    typedef struct GyroscopeData GyroscopeData;
    
    struct AccelerationData {
        float x_mm_per_sec_squared;
        float y_mm_per_sec_squared;
        float z_mm_per_sec_squared;
    };
    typedef struct AccelerationData AccelerationData;
    
    void Mpu_6050_initialize(I2cFunctions *i2c_functions);
    
    void ReadAccelerometer(I2cFunctions *i2c_functions, uint8_t addr, float lsb_sensitivity, AccelerationData *acceleration_data);
    
    void ReadGyroscope(I2cFunctions *i2c_functions, uint8_t addr, float lsb_sensitivity, GyroscopeData *data_out);
    
    void SetAccelerometerRange(I2cFunctions *i2c_functions, uint8_t addr, uint8_t range_bitmask);
    
    void SetGyroscopeRange(I2cFunctions *i2c_functions, uint8_t addr, uint8_t range_bitmask);
    
    void SetDigitalLowPassFilter(I2cFunctions *i2c_functions, uint8_t addr, uint8_t dlpf_config_bitmask);

#ifdef	__cplusplus
}
#endif

#endif	/* MPU_6050_H */

