/*
 * File:   main.c
 * Author: ben
 *
 * Created on August 28, 2022, 6:07 PM
 */

#include <xc.h>
#include <util/delay.h>
#include "main.h"
#include "../UART.X/uart.h"
#include "../I2C.X/i2c.h"
#include "mpu_6050.h"

// Helpful example https://github.com/YifanJiangPolyU/MPU6050/blob/master/mpu6050.c

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
    
    while (1) {
        int16_t accel[3];
        float accel_m_per_s2[3];
        
        ReadAccelerations(MPU_6050_ADDR, accel);
        RawAccelerationToMetersPerSecondSquared(accel, accel_m_per_s2);
        
        uart0_send_string((char*)"  ACC_X --> ");
        uart0_print_s16(((int16_t)(accel_m_per_s2[0]*1000)));
        uart0_send_string((char*)"\r\n");
        uart0_send_string((char*)"  ACC_Y --> ");
        uart0_print_s16(((int16_t)(accel_m_per_s2[1]*1000)));
        uart0_send_string((char*)"\r\n");
        uart0_send_string((char*)"  ACC_Z --> ");
        uart0_print_s16(((int16_t)(accel_m_per_s2[2]*1000)));
        uart0_send_string((char*)"\r\n\r\n");
        
        _delay_ms(1000);
    }
    
    return 0;
}
