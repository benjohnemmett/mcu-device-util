#include "i2c.h"

#define TWI0_BAUD(F_SCL) ((((float)F_CPU / (float)F_SCL)) - 10 )

void I2cInitialize() {
    // 25.3.2.1.1 Host Initialization
    TWI0.CTRLA |= TWI_SDAHOLD_OFF_gc;
    TWI0.MBAUD = TWI0_BAUD(100000);
    TWI0.MCTRLA = TWI_ENABLE_bm;
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
}

uint8_t I2cSendStart(uint8_t client_address, uint8_t is_reading) {
    TWI0.MADDR = (client_address << 1) | is_reading;
    while (!(TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm))); // Wait for read/write complete
    return !(TWI0.MSTATUS & TWI_RXACK_bm);
}

void I2cWrite(uint8_t data) {
    while(!(TWI0.MSTATUS & TWI_WIF_bm)); // Wait for write interrupt flag 
    TWI0.MDATA = data;
    TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc;
}

uint8_t I2cRead(uint8_t ack) {
    while(!(TWI0.MSTATUS & TWI_RIF_bm)); // Wait for read interrupt flag
    uint8_t data = TWI0.MDATA;
    if (ack) {
        TWI0.MCTRLB = (TWI_ACKACT_ACK_gc | TWI_MCMD_RECVTRANS_gc);
    } else { // NACK
        TWI0.MCTRLB = (TWI_ACKACT_NACK_gc | TWI_MCMD_RECVTRANS_gc);
    }
    return data;
}

void I2cSendStop() {
    TWI0.MCTRLB = (TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc);
}
