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
    
    SetupPca9685(&i2c_functions, PCA9685_PRESCALE_50HZ);
    uart0_send_string("Running PCA9685 Servo Test \r\n");
    RunPca9685ServoTest(&i2c_functions, 1);
    
    return 0;
}
