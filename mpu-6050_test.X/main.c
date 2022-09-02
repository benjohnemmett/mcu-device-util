/*
 * File:   main.c
 * Author: ben
 *
 * Created on August 28, 2022, 6:07 PM
 */

#include <xc.h>
#include "main.h"
#include "uart/uart.h"
#include "i2c/i2c.h"
#include <util/delay.h>

#define MPU_6050_ADDR 0x68
#define MPUT_6050_WHO_AM_I 0x75
#define ACCEL_XOUT_H 0x3B
#define ACCEL_XOUT_L 0x3C
#define MPU_6050_PWR_MGMT_1 0x6B
#define MPU_6050_CONFIG 0x1A
#define MPU_6050_GYRO_CONFIG 0x1B
#define MPU_6050_ACCEL_CONFIG 0x1C
#define MPU_6050_INT_ENABLE 0x38
#define MPU_6050_SIGNAL_PATH_RESET 0x68

// Helpful example https://github.com/YifanJiangPolyU/MPU6050/blob/master/mpu6050.c

uint8_t ReadByte(uint8_t addr, uint8_t reg) {
    I2cSendStart(addr, I2C_WRITE);
    I2cWrite(reg);
    I2cSendStart(addr, I2C_READ);
    uint8_t data = I2cRead(I2C_NACK);
    I2cSendStop();
    
    return data;
}
uint16_t ReadUint16(uint8_t addr, uint8_t reg) {
    I2cSendStart(addr, I2C_WRITE);
    I2cWrite(reg);
    I2cSendStart(addr, I2C_READ);
    uint8_t data_h = I2cRead(I2C_ACK);
    uint8_t data_l = I2cRead(I2C_NACK);
    I2cSendStop();
    
    return (((uint16_t)data_h) << 8) | ((uint16_t)data_l);
}

void WriteByte(uint8_t addr, uint8_t reg, uint8_t value) {
    I2cSendStart(addr, I2C_WRITE);
    I2cWrite(reg);
    I2cWrite(value);
    I2cSendStop();
}

void WriteBytes(uint8_t addr, uint8_t reg, uint8_t *value, uint8_t length) {
    I2cSendStart(addr, I2C_WRITE);
    I2cWrite(reg);
    for (int i = 0; i < length; i++) {
        I2cWrite(*value);
        value++;
    }
    I2cSendStop();
}

int main(void) {
    // Set CPU clock divider to 1
    CCP = CCP_IOREG_gc; //Configuration Change Protection
    CLKCTRL.MCLKCTRLB = 0; //Clock Div = 1
    
    uart0_init(BAUD_RATE);
    I2cInitialize();
    _delay_ms(10);
    
    uart0_send_string((char*)"Starting up...\r\n");
    uint8_t who_am_i = ReadByte(MPU_6050_ADDR, MPUT_6050_WHO_AM_I);
    uart0_send_string((char*)"  I am ");
    uart0_print_u8(who_am_i);
    uart0_send_string((char*)"!\r\n");
    
    WriteByte(MPU_6050_ADDR, MPU_6050_PWR_MGMT_1, 0x00);
    WriteByte(MPU_6050_ADDR, MPU_6050_CONFIG, 0x01);
    WriteByte(MPU_6050_ADDR, MPU_6050_GYRO_CONFIG, 0x00);
    WriteByte(MPU_6050_ADDR, MPU_6050_ACCEL_CONFIG, 0x00);
    WriteByte(MPU_6050_ADDR, MPU_6050_INT_ENABLE, 0x00);
    WriteByte(MPU_6050_ADDR, MPU_6050_SIGNAL_PATH_RESET, 0x00);
    
    while (1) {
        
        uint8_t config = ReadByte(MPU_6050_ADDR, MPU_6050_CONFIG);
        uart0_send_string((char*)"  config = ");
        uart0_print_u8(config);
        uart0_send_string((char*)"\r\n");
        
        uint16_t acc_x = ReadUint16(MPU_6050_ADDR, ACCEL_XOUT_H);
        uart0_send_string((char*)"  ACC_X = ");
        uart0_print_u16(acc_x);
        uart0_send_string((char*)"\r\n");
        
        who_am_i = ReadByte(MPU_6050_ADDR, MPUT_6050_WHO_AM_I);
        uart0_send_string((char*)"  I am ");
        uart0_print_u8(who_am_i);
        uart0_send_string((char*)"!\r\n");
        
        _delay_ms(1000);
    }
    
    return 0;
}
