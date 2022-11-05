/* 
 * File:   PCA9685.h
 * Author: ben
 *
 * Created on October 13, 2022, 5:12 PM
 */

#ifndef PCA9685_H
#define	PCA9685_H

#define F_CPU 20000000

#include <xc.h>
#include <util/delay.h>

#define PC9685_ADDR 0x40
#define PC9685_PRESCALE_50HZ 129

#define PC9685_REG_MODE1 0x00
#define PC9685_REG_MODE2 0x01
#define PC9685_REG_LED0_ON_L 0x06
#define PC9685_REG_LED0_ON_H 0x07
#define PC9685_REG_LED0_OFF_L 0x08
#define PC9685_REG_LED0_OFF_H 0x09
#define PC9685_REG_PRESCALE 0xFE

#define PC9685_NUM_REG_PER_LED 4

// Mode 1 bits
#define PC9685_SLEEP 4
#define PC9685_SLEEP_bm (1 << PC9685_SLEEP)
#define PC9685_AUTO_INC 5
#define PC9685_AUTO_INC_bm (1 << PC9685_AUTO_INC)
#define PC9685_RESTART 7
#define PC9685_RESTART_bm (1 << PC9685_RESTART)

#ifdef	__cplusplus
extern "C" {
#endif
    typedef struct I2cFunctions {
        void (*f_I2cInitialize)();
        void (*f_I2cWriteByte)(uint8_t addr, uint8_t reg, uint8_t value);
        void (*f_I2cWriteBytes)(uint8_t addr, uint8_t reg, uint8_t *value, uint8_t length);
    } I2cFunctions;

    void SetupPCA9685ForServoControl(I2cFunctions *i2c_functions);
    void SetLed(I2cFunctions *i2c_functions, uint8_t led_number, uint16_t pulse_width_ticks);
    void RunPca9685Test(I2cFunctions *i2c_functions);

#ifdef	__cplusplus
}
#endif

#endif	/* PCA9685_H */

