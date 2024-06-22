/*
 * File:   main.c
 * Author: ben
 *
 * Created on August 28, 2022, 6:07 PM
 */

#include "main.h"
#include <xc.h>
#include <util/delay.h> // TODO make delay functions hardware agnostic
#include "../../uart/uart.h"
#include "../../i2c/i2c.h"
#include "../../MPU6050.X/mpu_6050.h"

// Helpful example https://github.com/YifanJiangPolyU/MPU6050/blob/master/mpu6050.c

GyroscopeData gyroscope_data;
AccelerationData acceleration_data;

int main(void) {
    // Set CPU clock divider to 1
    CCP = CCP_IOREG_gc; //Configuration Change Protection
    CLKCTRL.MCLKCTRLB = 0; //Clock Div = 1
    
    uart0_init(BAUD_RATE);
    uart0_send_string((char*)"Starting up...\r\n");
    
    I2cInitialize();
    Mpu_6050_initialize();
    
    uint8_t who_am_i = I2cReadByte(MPU_6050_ADDR, MPU_6050_WHO_AM_I);
    uart0_send_string((char*)" I2C Who am I ->");
    uart0_print_u8(who_am_i);
    uart0_send_string((char*)"\r\n");
    
    SetAccelerometerRange(MPU_6050_ADDR, MPU_6050_ACCEL_RANGE_4G_bm);
    SetGyroscopeRange(MPU_6050_ADDR, MPU_6050_GYRO_RANGE_1000_bm);
    SetDigitalLowPassFilter(MPU_6050_ADDR, MPU_6050_DLPF_ACC_BW_5_HZ_bm);
    
    while (1) {
        ReadAccelerometer(MPU_6050_ADDR, ACC_LSB_4G, &acceleration_data);
        ReadGyroscope(MPU_6050_ADDR, GYRO_LSB_1000_DEG_PER_SEC, &gyroscope_data);
        
        uart0_send_string((char*)"  ACC_X: ");
        uart0_print_s16(((int16_t)(acceleration_data.x_mm_per_sec_squared * 1000)));
        uart0_send_string((char*)"\r\n");
        uart0_send_string((char*)"  ACC_Y: ");
        uart0_print_s16(((int16_t)(acceleration_data.y_mm_per_sec_squared * 1000)));
        uart0_send_string((char*)"\r\n");
        uart0_send_string((char*)"  ACC_Z: ");
        uart0_print_s16(((int16_t)(acceleration_data.z_mm_per_sec_squared * 1000)));
        uart0_send_string((char*)"\r\n");
   
        uart0_send_string((char*)"  GYRO_X: ");
        uart0_print_s16(gyroscope_data.x_deg_per_second);
        uart0_send_string((char*)"\r\n");
        uart0_send_string((char*)"  GYRO_Y: ");
        uart0_print_s16(gyroscope_data.y_deg_per_second);
        uart0_send_string((char*)"\r\n");
        uart0_send_string((char*)"  GYRO_Z: ");
        uart0_print_s16(gyroscope_data.z_deg_per_second);
        
        uart0_send_string((char*)"\r\n\r\n");
        
        _delay_ms(1000);
    }
    
    return 0;
}
