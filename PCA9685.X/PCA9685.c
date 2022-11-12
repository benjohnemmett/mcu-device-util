
#include "PCA9685.h"

void SetupPca9685(I2cFunctions *i2c_functions) {
    
    i2c_functions->f_I2cInitialize();
    // Turn on: restart, enable auto-increment address, leave in sleep mode for now
    i2c_functions->f_I2cWriteByte(PCA9685_ADDR, PCA9685_REG_MODE1, (PCA9685_SLEEP_bm | PCA9685_RESTART_bm | PCA9685_AUTO_INC_bm));
    // Set Prescaler
    i2c_functions->f_I2cWriteByte(PCA9685_ADDR, PCA9685_REG_PRESCALE, PCA9685_PRESCALE_TO_USE);
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
