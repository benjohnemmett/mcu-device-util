/*
 * File:   main.c
 * Author: ben
 *
 * Created on October 13, 2022, 5:11 PM
 */


#define F_CPU 20000000
#define BAUD_RATE 9600

#include <xc.h>
#include "../../PCA9685.X/PCA9685.h"
#include "../../uart/uart.h"

int main(void) {
    // Set CPU clock divider to 1
    CCP = CCP_IOREG_gc; //Configuration Change Protection
    CLKCTRL.MCLKCTRLB = 0; //Clock Div = 1
    
    uart0_init(BAUD_RATE);
    uart0_send_string("Starting up PCA9685 Test\r\n");
    
//    SetupPca9685(PCA9685_PRESCALE_50HZ);
//    uart0_send_string("Running PCA9685 Servo Test \r\n");
//    RunPca9685ServoTest(1);
    
    SetupPca9685(PCA9685_PRESCALE_MIN);
    uart0_send_string("Running PCA9685 light Test \r\n");
    RunPca9685LightTest();
    
    return 0;
}
