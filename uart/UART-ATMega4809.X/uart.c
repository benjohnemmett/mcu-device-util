#include "../uart.h"
#include <avr/io.h>

//Helpful example: https://github.com/MicrochipTech/TB3216_Getting_Started_with_USART/blob/master/Send_Hello_World/main.c

#define MAXBUF (sizeof(long int) * 8)

#define UART_ERROR 0
#define UART_SUCCESS 1

uint8_t _get_uart_reg_ptr(uint8_t uart_number, USART_t** usart_reg_ptr_ptr)
{
    switch(uart_number)
    {
        case 0:
            *usart_reg_ptr_ptr = &USART0;
            return UART_SUCCESS;
        case 1:
            *usart_reg_ptr_ptr = &USART1;
            return UART_SUCCESS;
        case 2:
            *usart_reg_ptr_ptr = &USART2;
            return UART_SUCCESS;
        case 3:
            *usart_reg_ptr_ptr = &USART3;
            return UART_SUCCESS;
    }
    return UART_ERROR;
}

    
uint8_t uart_init(uint8_t usart_index, unsigned int baud) {
    //1. Set the baud rate (USARTn.BAUD).
    USART_t *usart_regs;
    if (!_get_uart_reg_ptr(usart_index, &usart_regs))
    {
        // Failed to init invalid usart index
        return UART_ERROR;
    }
    
    usart_regs->BAUD = (uint16_t)USART_BAUD_RATE(baud);

    //2. Set the frame format and mode of operation (USARTn.CTRLC).
    usart_regs->CTRLC = USART_CMODE_ASYNCHRONOUS_gc | 
                   USART_PMODE_DISABLED_gc | 
                   USART_SBMODE_1BIT_gc |
                   USART_CHSIZE_8BIT_gc;

    //3. Configure the TXD pin as an output.
    if (usart_index == 0)
    {
        PORTA.DIRSET = PIN0_bm;
        PORTA.DIRCLR = PIN1_bm;
    }
    else if (usart_index == 1)
    {
        PORTC.DIRSET = PIN0_bm;
        PORTC.DIRCLR = PIN1_bm;
    }
    else if (usart_index == 2)
    {
        PORTF.DIRSET = PIN0_bm;
        PORTF.DIRCLR = PIN1_bm;
    }
    else if (usart_index == 3)
    {
        PORTB.DIRSET = PIN0_bm;
        PORTB.DIRCLR = PIN1_bm;
    }

    //4. Enable the transmitter and the receiver (USARTn.CTRLB).
    usart_regs->CTRLB = USART_TXEN_bm | 
                   USART_RXEN_bm;
    
    return UART_SUCCESS;
}

uint8_t uart_send_char(uint8_t usart_index, char value) {
    USART_t *usart_regs;
    if (!_get_uart_reg_ptr(usart_index, &usart_regs))
    {
        // Failed to init invalid usart index
        return UART_ERROR;
    }
    
    while(!(usart_regs->STATUS & USART_DREIF_bm)) {}
    usart_regs->TXDATAL = value;
    
    return UART_SUCCESS;
}

uint8_t uart_send_string(uint8_t usart_index, char *string) {
    while (string[0]) {
        if (!uart_send_char(usart_index, string[0]))
        {
            return UART_ERROR;
        }
        string++;
    }
    return UART_SUCCESS;
}

void uart0_init(unsigned int baud) {
    uart_init(0, baud);
}

void uart0_send_char(char value) {
    uart_send_char(0, value);
}

void uart0_send_string(char *string) {
    while (string[0]) {
        uart0_send_char(string[0]);
        string++;
    }
}

//  Based on https://opensource.apple.com/source/xnu/xnu-201/osfmk/kern/printf.c.auto.html
uint8_t uart_print_num(uint8_t uart_index, unsigned long value, int base) {

    char buffer[MAXBUF];
	register char *buffer_pointer = &buffer[MAXBUF-1];
	static char digits[] = "0123456789ABCDEF";
    
    do {
        *buffer_pointer-- = digits[value % base];
        value /= base;
    } while (value != 0);
    
    while (++buffer_pointer != &buffer[MAXBUF]) {
        if(!uart_send_char(uart_index, *buffer_pointer))
        {
            return UART_ERROR;
        }
    }
    return UART_SUCCESS;
}

/*
 *  Write the ASCII value of a decimal number. 
 *  Input must be in the range [0 9]
*/
char DecToAscii(uint8_t dec) {
    return (char)(dec + 0x30);
}

/*
 * Write the ASCII representation of the 8 bit unsigned value.
 */
void uart0_print_u8(uint8_t value) {
    uint8_t printZeros = 0;

    if (value > 99) {
        uint8_t hundreds = value / 100;
        value -= 100*hundreds;
        uart0_send_char(DecToAscii(hundreds));
        printZeros = 1;
    }

    if (value > 9) {
        uint8_t tens = value / 10;
        value -= 10*tens;
        uart0_send_char(DecToAscii(tens));
    } else if (printZeros) {
        uart0_send_char(DecToAscii(0));
    }
    uart0_send_char(DecToAscii(value));
}

/*
 * Write the ASCII representation of the 16 bit unsigned value.
 */
void uart0_print_u16(uint16_t value) {
    uint8_t printZeros = 0;

    if (value > 9999) {
        uint8_t tenthousands = value / 10000;
        value -= 10000*tenthousands;
        uart0_send_char(DecToAscii(tenthousands));
        printZeros = 1;
    }

    if (value > 999) {
        uint8_t thousands = value / 1000;
        value -= 1000*thousands;
        uart0_send_char(DecToAscii(thousands));
        printZeros = 1;
    } else if (printZeros) {
        uart0_send_char(DecToAscii(0));
    }

    if (value > 99) {
        uint8_t hundreds = value / 100;
        value -= 100*hundreds;
        uart0_send_char(DecToAscii(hundreds));
        printZeros = 1;
    } else if (printZeros) {
        uart0_send_char(DecToAscii(0));
    }

    if (value > 9) {
        uint8_t tens = value / 10;
        value -= 10*tens;
        uart0_send_char(DecToAscii(tens));
    } else if (printZeros) {
        uart0_send_char(DecToAscii(0));
    }
    uart0_send_char(DecToAscii(value));
}

void uart0_print_s16(int16_t value) {
    if (value < 0) {
        uart0_send_char('-');
        value = -1 * value;
    }
    uart0_print_u16(value);
}

void uart0_print_num(unsigned long value, int base) {
    uart_print_num(0, value, base);
}