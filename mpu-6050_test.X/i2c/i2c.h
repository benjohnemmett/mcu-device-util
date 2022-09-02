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
#include "../main.h"

#define I2C_WRITE 0
#define I2C_READ 1
#define I2C_NACK 0
#define I2C_ACK 1

#ifdef	__cplusplus
extern "C" {
#endif

void I2cInitialize();
uint8_t I2cSendStart(uint8_t client_address, uint8_t is_reading);
void I2cWrite(uint8_t data);
uint8_t I2cRead(uint8_t ack);
void I2cSendStop();

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_H */

