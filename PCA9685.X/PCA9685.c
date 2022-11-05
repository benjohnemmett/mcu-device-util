
#include "PCA9685.h"

void SetupPCA9685ForServoControl(I2cFunctions *i2c_functions) {
    
    i2c_functions->f_I2cInitialize();
    // Turn on: restart, enable auto-increment address, leave in sleep mode for now
    i2c_functions->f_I2cWriteByte(PC9685_ADDR, PC9685_REG_MODE1, (PC9685_SLEEP_bm | PC9685_RESTART_bm | PC9685_AUTO_INC_bm));
    // Set Prescaler
    i2c_functions->f_I2cWriteByte(PC9685_ADDR, PC9685_REG_PRESCALE, PC9685_PRESCALE_50HZ);
    // Disable sleep mode
    i2c_functions->f_I2cWriteByte(PC9685_ADDR, PC9685_REG_MODE1, PC9685_AUTO_INC_bm);
    // Configure open-drain, non-inverted, 
    i2c_functions->f_I2cWriteByte(PC9685_ADDR, PC9685_REG_MODE2, 0x00);
}


void SetLed(I2cFunctions *i2c_functions, uint8_t led_number, uint16_t pulse_width_ticks) {
    uint8_t led_on_off_values[4] = { 0x00, 0x00, (pulse_width_ticks & 0xFF), (pulse_width_ticks >> 8) & 0xFF }; 
    uint8_t led_reg = PC9685_REG_LED0_ON_L + led_number * PC9685_NUM_REG_PER_LED;
    i2c_functions->f_I2cWriteBytes(PC9685_ADDR, led_reg, led_on_off_values, PC9685_NUM_REG_PER_LED);
}

void RunPca9685Test(I2cFunctions *i2c_functions) {
    //uint8_t values[4] = { 0x00, 0x00, 0xFF, 0x07 }; 
    //i2c_functions->f_I2cWriteBytes(PC9685_ADDR, PC9685_REG_LED0_ON_L, values, PC9685_NUM_REG_PER_LED);
    uint16_t ticks = 100;
    int8_t delta = 10;
    while (1) {
        if (ticks > 400){
            delta = -10;
        } else if (ticks < 220) {
            delta = 10;
        }
        
        ticks += delta;
        SetLed(i2c_functions, 0, ticks);
        _delay_ms(100);
    }
}
