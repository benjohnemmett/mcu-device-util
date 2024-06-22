/* 
 * File:   main.h
 * Author: ben
 *
 * Created on August 28, 2022, 6:18 PM
 */

#ifndef MAIN_H
#define	MAIN_H

#define F_CPU 20000000
#define BAUD_RATE 9600

// Hardware specific includes
#include <util/delay.h> // TODO abstract the AVR-specific delay functions
#include "../../uart/UART-ATMega4809.X/uart.h"
#include "../../i2c/I2C-ATMega4809.X/i2c.h"

#ifdef	__cplusplus
extern "C" {
#endif


#ifdef	__cplusplus
}
#endif

#endif	/* MAIN_H */

