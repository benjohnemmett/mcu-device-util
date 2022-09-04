#include "i2c.h"

#define TWI0_BAUD(F_SCL) ( ( ((float)F_CPU / (2 * (float)F_SCL)) ) - 10 )

void I2cInitialize() {
    // 25.3.2.1.1 Host Initialization
    TWI0.CTRLA = TWI_SDAHOLD_50NS_gc;//TWI_SDAHOLD_OFF_gc;
    TWI0.MBAUD = TWI0_BAUD(100000);
    TWI0.MCTRLA = TWI_ENABLE_bm;// | TWI_SMEN_bm;
    TWI0.MSTATUS = TWI_BUSSTATE_IDLE_gc;
}

uint8_t I2cSendStart(uint8_t client_address, uint8_t is_reading) {
    TWI0.MADDR = (client_address << 1) | is_reading;
    while (!(TWI0.MSTATUS & (TWI_WIF_bm | TWI_RIF_bm))); // Wait for read/write complete
    return !(TWI0.MSTATUS & TWI_RXACK_bm);
}

void I2cWrite(uint8_t data) {
    while(!(TWI0.MSTATUS & TWI_CLKHOLD_bm)); // Wait for write interrupt flag to be high
    TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc;
    TWI0.MDATA = data;
    while(!(TWI0.MSTATUS & TWI_WIF_bm)); // Wait for write interrupt flag to be high
}

uint8_t I2cRead(uint8_t ack) {
    while(!(TWI0.MSTATUS & TWI_CLKHOLD_bm)); // Wait for write interrupt flag to be high
    if (ack) {
        TWI0.MCTRLB = TWI_MCMD_RECVTRANS_gc;
    } else { // NACK
        TWI0.MCTRLB = TWI_ACKACT_bm | TWI_MCMD_RECVTRANS_gc;
    }
    uint8_t data = TWI0.MDATA;

    return data;
}

void I2cSendStop() {
    while(!(TWI0.MSTATUS & TWI_CLKHOLD_bm)); // Wait for write interrupt flag to be high
    TWI0.MCTRLB = (TWI_ACKACT_NACK_gc | TWI_MCMD_STOP_gc);
}

uint8_t I2cReadByte(uint8_t addr, uint8_t reg) {
    I2cSendStart(addr, I2C_WRITE);
    I2cWrite(reg);
    I2cSendStart(addr, I2C_READ);
    uint8_t data = I2cRead(I2C_NACK);
    I2cSendStop();
    
    return data;
}

int16_t I2cReadUint16(uint8_t addr, uint8_t reg) {
    I2cSendStart(addr, I2C_WRITE);
    I2cWrite(reg);
    I2cSendStart(addr, I2C_READ);
    uint8_t data_h = I2cRead(I2C_ACK);
    uint8_t data_l = I2cRead(I2C_NACK);
    I2cSendStop();
    
    return ((data_h) << 8) | (data_l);
}

void I2cWriteByte(uint8_t addr, uint8_t reg, uint8_t value) {
    I2cSendStart(addr, I2C_WRITE);
    I2cWrite(reg);
    I2cWrite(value);
    I2cSendStop();
}

void I2cWriteBytes(uint8_t addr, uint8_t reg, uint8_t *value, uint8_t length) {
    I2cSendStart(addr, I2C_WRITE);
    I2cWrite(reg);
    for (int i = 0; i < length; i++) {
        I2cWrite(*value);
        value++;
    }
    I2cSendStop();
}
