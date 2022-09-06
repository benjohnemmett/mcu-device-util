/* 
 * File:   i2c.h
 * Author: ben
 *
 * Created on August 28, 2022, 8:15 PM
 */

#ifndef I2C_H
#define	I2C_H

// Helpful example here: https://www.avrfreaks.net/forum/i2c-communication-atmega4809

#include <avr/io.h>
#include <util/twi.h>

#ifndef F_CPU
#define F_CPU 20000000
#endif

#ifndef F_SCL
#define F_SCL 100000
#endif

#define I2C_WRITE 0
#define I2C_READ 1
#define I2C_NACK 0
#define I2C_ACK 1

#ifdef	__cplusplus
extern "C" {
#endif

void I2cInitialize();
void I2cWaitForReadyFlag();
uint8_t I2cSendStart(uint8_t client_address, uint8_t is_reading);
void I2cWrite(uint8_t data);
uint8_t I2cRead(uint8_t ack);
void I2cSendStop();

// Higher level
uint8_t I2cReadByte(uint8_t addr, uint8_t reg);
int16_t I2cReadUint16(uint8_t addr, uint8_t reg);
void I2cWriteByte(uint8_t addr, uint8_t reg, uint8_t value);
void I2cWriteBytes(uint8_t addr, uint8_t reg, uint8_t *value, uint8_t length);

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

