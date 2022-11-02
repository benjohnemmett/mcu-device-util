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
    
    SetupPCA9685ForServoControl(&i2c_functions);
    uart0_send_string("Setup Complete...\r\n");
    RunPca9685Test(&i2c_functions);
    uart0_send_string("PCA9685 Test Complete\r\n");
    
    return 0;
}
