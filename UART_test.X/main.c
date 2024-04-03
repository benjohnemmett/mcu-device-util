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
    
    uint8_t uart_index = 1;
    
    uart_init(uart_index, BAUD_RATE);
    uart_send_string(uart_index, (char*)"\r\nUART Test\r\n\0");
    
    uart_send_string(uart_index, (char*)"===== Decimal =====\r\n\0");
    for (unsigned short number = 0; number < 999; number++){
        uart_print_num(uart_index, number, 10);
        uart_send_string(uart_index, (char*)", \0");
    }
    uart_send_string(uart_index, (char*)"\r\n\r\n\0");
    
    uart_send_string(uart_index, (char*)"\r\n0xFFFFFFFF in dec ");
    uart_print_num(uart_index, 0xFFFFFFFF, 10);
    uart_send_string(uart_index, (char*)"\r\n\r\n\0");
    _delay_ms(2000);
    
    uart_send_string(uart_index, (char*)"\r\n===== Haxadecimal =====\r\n\0");
    for (unsigned short number = 0; number < 999; number++){
        uart_send_string(uart_index, (char*)"0x\0");
        uart_print_num(uart_index, number, 16);
        uart_send_string(uart_index, (char*)", \0");
    }
    
    uart_send_string(uart_index, (char*)"\r\n0xFFFFFFFF in hex 0x");
    uart_print_num(uart_index, 0xFFFFFFFF, 16);
    uart_send_string(uart_index, (char*)"\r\n\r\n\0");
    _delay_ms(2000);
    
    uart_send_string(uart_index, (char*)"\r\n===== Binary =====\r\n\0");
    for (unsigned short number = 0; number < 999; number++){
        uart_send_string(uart_index, (char*)"0b\0");
        uart_print_num(uart_index, number, 2);
        uart_send_string(uart_index, (char*)", \0");
    }
    
    uart_send_string(uart_index, (char*)"\r\n0xFFFFFFFF in bin 0b");
    uart_print_num(uart_index, 0xFFFFFFFF, 2);
    uart_send_string(uart_index, (char*)"\r\n\r\n\0");
    
    _delay_ms(2000);
    
    while (1) {
        uart_send_string(uart_index, (char*)"\r\n-- End of UART Test --\r\n\0");
        _delay_ms(10000);
    }
    
    return 0;
}
