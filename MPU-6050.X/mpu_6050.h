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

#define GYRO_LSB_250_DEG_PER_SEC 131
#define GYRO_LSB_500_DEG_PER_SEC 65.5
#define GYRO_LSB_1000_DEG_PER_SEC 32.8
#define GYRO_LSB_2000_DEG_PER_SEC 16.4

#ifdef	__cplusplus
extern "C" {
#endif
    
    struct GyroscopeData {
        int16_t x;
        int16_t y;
        int16_t z;
    };
    typedef struct GyroscopeData GyroscopeData;
    
    void Mpu_6050_initialize(I2cFunctions *i2c_functions);
    
    void ReadAccelerations(I2cFunctions *i2c_functions, uint8_t addr, int16_t *buffer);
    
    void RawAccelerationToMetersPerSecondSquared(int16_t *raw_accel, float *converted_accel);
    
    void ReadGyroscope(I2cFunctions *i2c_functions, uint8_t addr, float lsb_sensitivity, GyroscopeData *data_out);

#ifdef	__cplusplus
}
#endif

#endif	/* MPU_6050_H */

