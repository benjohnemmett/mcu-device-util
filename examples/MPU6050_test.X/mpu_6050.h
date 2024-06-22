/* 
 * File:   mpu_6050.h
 * Author: ben
 *
 * Created on September 3, 2022, 10:27 PM
 */

#ifndef MPU_6050_H
#define	MPU_6050_H

#include "../I2C.X/i2c.h"

#define MPU_6050_ADDR 0x68
#define MPU_6050_WHO_AM_I 0x75
#define MPU_6050_ACCEL_XOUT_H 0x3B
#define MPU_6050_ACCEL_XOUT_L 0x3C
#define MPU_6050_ACCEL_YOUT_H 0x3D
#define MPU_6050_ACCEL_YOUT_L 0x3E
#define MPU_6050_ACCEL_ZOUT_H 0x3F
#define MPU_6050_ACCEL_ZOUT_L 0x40
#define MPU_6050_PWR_MGMT_1 0x6B
#define MPU_6050_CONFIG 0x1A
#define MPU_6050_GYRO_CONFIG 0x1B
#define MPU_6050_ACCEL_CONFIG 0x1C
#define MPU_6050_INT_ENABLE 0x38
#define MPU_6050_SIGNAL_PATH_RESET 0x68

#ifdef	__cplusplus
extern "C" {
#endif
    
    void Mpu_6050_initialize();
    void ReadAccelerations(uint8_t addr, int16_t *buffer);
    void RawAccelerationToMetersPerSecondSquared(int16_t *raw_accel, float *converted_accel);

#ifdef	__cplusplus
}
#endif

#endif	/* MPU_6050_H */

