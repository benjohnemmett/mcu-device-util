/*
 * File:   main.c
 * Author: ben
 *
 * Created on August 7, 2023, 8:33 PM
 */

#define F_CPU 20000000
#define BAUD_RATE 9600

#include <xc.h>
#include <util/delay.h>
#include "../UART.X/uart.h"

int main(void) {
    // Set CPU clock divider to 1
    CCP = CCP_IOREG_gc; //Configuration Change Protection
    CLKCTRL.MCLKCTRLB = 0; //Clock Div = 1
    
    uart0_init(BAUD_RATE);
    uart0_send_string((char*)"\r\nUART Test\r\n\0");
    
    uart0_send_string((char*)"===== Decimal =====\r\n\0");
    for (unsigned short number = 0; number < 999; number++){
        uart0_print_num(number, 10);
        uart0_send_string((char*)", \0");
    }
    uart0_send_string((char*)"\r\n\r\n\0");
    
    uart0_send_string((char*)"\r\n0xFFFFFFFF in dec ");
    uart0_print_num(0xFFFFFFFF, 10);
    uart0_send_string((char*)"\r\n\r\n\0");
    _delay_ms(2000);
    
    uart0_send_string((char*)"\r\n===== Haxadecimal =====\r\n\0");
    for (unsigned short number = 0; number < 999; number++){
        uart0_send_string((char*)"0x\0");
        uart0_print_num(number, 16);
        uart0_send_string((char*)", \0");
    }
    
    uart0_send_string((char*)"\r\n0xFFFFFFFF in hex 0x");
    uart0_print_num(0xFFFFFFFF, 16);
    uart0_send_string((char*)"\r\n\r\n\0");
    _delay_ms(2000);
    
    uart0_send_string((char*)"\r\n===== Binary =====\r\n\0");
    for (unsigned short number = 0; number < 999; number++){
        uart0_send_string((char*)"0b\0");
        uart0_print_num(number, 2);
        uart0_send_string((char*)", \0");
    }
    
    uart0_send_string((char*)"\r\n0xFFFFFFFF in bin 0b");
    uart0_print_num(0xFFFFFFFF, 2);
    uart0_send_string((char*)"\r\n\r\n\0");
    
    _delay_ms(2000);
    
    while (1) {
        uart0_send_string((char*)"\r\n-- End of UART Test --\r\n\0");
        _delay_ms(10000);
    }
    
    return 0;
}
