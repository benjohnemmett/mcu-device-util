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

int main(void) {
    // Set CPU clock divider to 1
    CCP = CCP_IOREG_gc; //Configuration Change Protection
    CLKCTRL.MCLKCTRLB = 0; //Clock Div = 1
    
    uart0_init(BAUD_RATE);
    I2cInitialize();
    
    uart0_send_string((char*)"Starting Scan...\r\n");
    uint8_t address = 0;
    
    while (1) {
        if(I2cSendStart(address, I2C_WRITE)) {
            uart0_send_string((char*)"  Found Client at ");
            uart0_print_u8(address);
            uart0_send_string((char*)"!\r\n");
        }
        
        address++;
        if (address > 127) {
            address = 0;
        }
        
        _delay_ms(50);
    }
    
    return 0;
}
