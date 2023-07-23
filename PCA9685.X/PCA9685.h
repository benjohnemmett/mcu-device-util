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
#include "../I2C.X/i2c_interface.h"

#define PCA9685_ADDR 0x40
#define PCA9685_PRESCALE_50HZ 129
#define PCA9685_PRESCALE_MIN 3

#define PCA9685_REG_MODE1 0x00
#define PCA9685_REG_MODE2 0x01
#define PCA9685_REG_LED0_ON_L 0x06
#define PCA9685_REG_LED0_ON_H 0x07
#define PCA9685_REG_LED0_OFF_L 0x08
#define PCA9685_REG_LED0_OFF_H 0x09
#define PCA9685_REG_PRESCALE 0xFE

#define PCA9685_NUM_REG_PER_LED 4

// Mode 1 bits
#define PCA9685_SLEEP 4
#define PCA9685_SLEEP_bm (1 << PCA9685_SLEEP)
#define PCA9685_AUTO_INC 5
#define PCA9685_AUTO_INC_bm (1 << PCA9685_AUTO_INC)
#define PCA9685_RESTART 7
#define PCA9685_RESTART_bm (1 << PCA9685_RESTART)

// Mode 2 bits
#define PCA9685_OUTDRV 2
#define PCA9685_OUTDRV_bm (1 << PCA9685_OUTDRV)

#define PCA9685_MIN_SERVO_PWM_TICKS 205
#define PCA9685_MAX_SERVO_PWM_TICKS 409
#define PCA9685_SERVO_PWM_TICKS_CENTER ((PCA9685_MAX_SERVO_PWM_TICKS + PCA9685_MIN_SERVO_PWM_TICKS) >> 1)
#define PCA9685_SERVO_PWM_TICKS_RANGE (PCA9685_MAX_SERVO_PWM_TICKS - PCA9685_MIN_SERVO_PWM_TICKS)
#define PCA9685_SERVO_PWM_TICKS_HALF_RANGE ((PCA9685_MAX_SERVO_PWM_TICKS - PCA9685_MIN_SERVO_PWM_TICKS) >> 1)

#ifdef	__cplusplus
extern "C" {
#endif

    void SetupPca9685(I2cFunctions *i2c_functions, uint8_t prescale);
    void SetLed(I2cFunctions *i2c_functions, uint8_t led_number, uint16_t pulse_width_ticks);
    void RunPca9685LightTest(I2cFunctions *i2c_functions);
    void RunPca9685ServoTest(I2cFunctions *i2c_functions, uint8_t num_servos);

#ifdef	__cplusplus
}
#endif

#endif	/* PCA9685_H */

