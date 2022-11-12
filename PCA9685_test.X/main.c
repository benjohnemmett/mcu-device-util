/*
 * File:   main.c
 * Author: ben
 *
 * Created on October 13, 2022, 5:11 PM
 */

#include "main.h"
#include <xc.h>
#include "../UART.X/uart.h"
#include "../PCA9685.X/PCA9685.h"
#include "../I2C.X/i2c.h"

#define PCA9685_PRESCALE_TO_USE PCA9685_PRESCALE_MIN

I2cFunctions i2c_functions;

int main(void) {
    // Set CPU clock divider to 1
    CCP = CCP_IOREG_gc; //Configuration Change Protection
    CLKCTRL.MCLKCTRLB = 0; //Clock Div = 1
    
    i2c_functions.f_I2cInitialize = I2cInitialize;
    i2c_functions.f_I2cWriteByte = I2cWriteByte;
    i2c_functions.f_I2cWriteBytes = I2cWriteBytes;
    
    uart0_init(BAUD_RATE);
    uart0_send_string("Starting up PCA9685 Test\r\n");
    
    SetupPca9685(&i2c_functions);
    uart0_send_string("Running PCA9685 Light Test \r\n");
    RunPca9685LightTest(&i2c_functions);
    
    return 0;
}
