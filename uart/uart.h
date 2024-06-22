/* 
 * File:   uart.h
 * Author: ben
 *
 * Created on August 12, 2022, 4:36 PM
 */

#ifndef UART_H
#define	UART_H

#include <stdint.h>

#ifndef F_CPU
#define F_CPU 20000000
#endif

#define UART_NORMAL_MODE 16
#define UART_DOUBLE_SPEED_MODE 8

#define USART_BAUD_RATE(BAUD_RATE) ((float)(F_CPU * 64 / (UART_NORMAL_MODE * (float)BAUD_RATE)) + 0.5)

#ifdef	__cplusplus
extern "C" {
#endif

    uint8_t uart_init(uint8_t uart_index, unsigned int baud);
    
    uint8_t uart_send_char(uint8_t uart_index, char value);
    
    uint8_t uart_send_string(uint8_t uart_index, char *string);
    
    uint8_t uart_print_num(uint8_t uart_index, unsigned long value, int base);
    
    void uart0_init();
    
    void uart0_send_char(char value);
    
    void uart0_send_string(char *string);
    
    void uart0_print_u8(uint8_t value);
    
    void uart0_print_u16(uint16_t value);
    
    void uart0_print_s16(int16_t value);
    
    void uart0_print_num(unsigned long u, int base);
    
#ifdef	__cplusplus
}
#endif

#endif	/* UART_H */

