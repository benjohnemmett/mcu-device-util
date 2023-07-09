
#include "PCA9685.h"

void SetupPca9685(I2cFunctions *i2c_functions, uint8_t prescale) {
    
    i2c_functions->f_I2cInitialize();
    // Turn on: restart, enable auto-increment address, leave in sleep mode for now
    i2c_functions->f_I2cWriteByte(PCA9685_ADDR, PCA9685_REG_MODE1, (PCA9685_SLEEP_bm | PCA9685_RESTART_bm | PCA9685_AUTO_INC_bm));
    // Set Prescaler
    i2c_functions->f_I2cWriteByte(PCA9685_ADDR, PCA9685_REG_PRESCALE, prescale);
    // Disable sleep mode
    i2c_functions->f_I2cWriteByte(PCA9685_ADDR, PCA9685_REG_MODE1, PCA9685_AUTO_INC_bm);
    // Configure open-drain, non-inverted, 
    i2c_functions->f_I2cWriteByte(PCA9685_ADDR, PCA9685_REG_MODE2, PCA9685_OUTDRV_bm);
}

void SetLed(I2cFunctions *i2c_functions, uint8_t led_number, uint16_t pulse_width_ticks) {
    uint8_t led_on_off_values[4] = { 0x00, 0x00, (pulse_width_ticks & 0xFF), (pulse_width_ticks >> 8) & 0xFF }; 
    uint8_t led_reg = PCA9685_REG_LED0_ON_L + led_number * PCA9685_NUM_REG_PER_LED;
    i2c_functions->f_I2cWriteBytes(PCA9685_ADDR, led_reg, led_on_off_values, PCA9685_NUM_REG_PER_LED);
}

/*
 * Sets a PCA9685 LED pin value for servo motor control. Valid input values for 
 * position are -1.0 to 1.0 and map to the PWM duty cycle range of 5% to 10%.
 * PWM frequency must be set prior to calling this function.
 */
void SetServo(I2cFunctions *i2c_functions, uint8_t servo_number, float position) {
    uint16_t pulse_width_ticks = PCA9685_SERVO_PWM_TICKS_CENTER + PCA9685_SERVO_PWM_TICKS_HALF_RANGE * position;
    if (pulse_width_ticks < PCA9685_MIN_SERVO_PWM_TICKS) {
        pulse_width_ticks = PCA9685_MIN_SERVO_PWM_TICKS;
    } else if (pulse_width_ticks > PCA9685_MAX_SERVO_PWM_TICKS) {
        pulse_width_ticks = PCA9685_MAX_SERVO_PWM_TICKS;
    }
    SetLed(i2c_functions, servo_number, pulse_width_ticks);
}

void RunPca9685LightTest(I2cFunctions *i2c_functions) {
    uint16_t pwm_on_duration_per_light[6] = {700, 1300, 1800, 2300, 2800, 3300};
    int8_t pwm_delta_per_light_each_iteration[6] = {100, 100, 100, 100, 100, 100};
    
    while (1) {
        for(int i = 0; i < 6; i++) {
            if (pwm_on_duration_per_light[i] > 3800){
                pwm_delta_per_light_each_iteration[i] = -100;
            } else if (pwm_on_duration_per_light[i] < 100) {
                pwm_delta_per_light_each_iteration[i] = 100;
            }

            pwm_on_duration_per_light[i] += pwm_delta_per_light_each_iteration[i];
            SetLed(i2c_functions, i, pwm_on_duration_per_light[i]);
        }
        _delay_ms(20);
    }
}

/*
 * Test for servos. Loops through the following steps
 * 1. Center each servo with 100 ms delay between each.
 * 2. Sweep each servo in turn from center up to max and down to min.
 */
void RunPca9685ServoTest(I2cFunctions *i2c_functions, uint8_t num_servos) {
    
    while(1) {
        // Sweep each servo in turn
        for (int servo_index = 0; servo_index < num_servos; servo_index++) {
            SetServo(i2c_functions, servo_index, 1.0);
            _delay_ms(500);
            
            SetServo(i2c_functions, servo_index, -1.0);
            _delay_ms(500);
            
            float position = -0.99;
            float delta = 0.02;
            
            while (position < 1.0) {
                position += delta;
                SetServo(i2c_functions, servo_index, position);
                _delay_ms(20);
            }
            
            delta = -0.02;
            while (position > -1.0) {
                position += delta;
                SetServo(i2c_functions, servo_index, position);
                _delay_ms(20);
            }
        }
        _delay_ms(500);
    }
}
