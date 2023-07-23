/* 
 * File:   i2c_interface.h
 * Author: ben
 *
 * Created on July 22, 2023, 10:14 PM
 * 
 * This interface is a way to pass functions for handling I2C logic in a platform
 * independent way. Include this in any library that requires I2C and let the 
 * calling code pass in the I2cFunctions struct.
 */

#include <avr/io.h>

#ifndef I2C_INTERFACE_H
#define	I2C_INTERFACE_H

#ifdef	__cplusplus
extern "C" {
#endif

    typedef struct I2cFunctions {
        uint8_t (*f_I2cSendStart)(uint8_t client_address, uint8_t is_reading);
        void (*f_I2cWrite)(uint8_t data);
        uint8_t (*f_I2cRead)(uint8_t ack);
        void (*f_I2cSendStop)();
        
        void (*f_I2cInitialize)();
        void (*f_I2cWriteByte)(uint8_t addr, uint8_t reg, uint8_t value);
        void (*f_I2cWriteBytes)(uint8_t addr, uint8_t reg, uint8_t *value, uint8_t length);
    } I2cFunctions;

#ifdef	__cplusplus
}
#endif

#endif	/* I2C_INTERFACE_H */

