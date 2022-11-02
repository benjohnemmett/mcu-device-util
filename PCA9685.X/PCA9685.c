
#include "PCA9685.h"

void SetupPCA9685ForServoControl(I2cFunctions *i2c_functions) {
    
    i2c_functions->f_I2cInitialize();
    // Set Prescaler to 121
    i2c_functions->f_I2cWriteByte(PC9685_ADDR, PC9685_REG_PRESCALE, PC9685_PRESCAL_50HZ);
    //Turn on: disable sleep, enable auto-increment address
    i2c_functions->f_I2cWriteByte(PC9685_ADDR, PC9685_REG_MODE1, PC9685_AUTO_INC_bm);
    //Configure open-drain, non-inverted, 
    i2c_functions->f_I2cWriteByte(PC9685_ADDR, PC9685_REG_MODE2, 0x00);
}

void RunPca9685Test(I2cFunctions *i2c_functions) {
    uint8_t values[4] = { 0x00, 0x00, 0xFF, 0x07 }; 
    i2c_functions->f_I2cWriteBytes(PC9685_ADDR, PC9685_REG_LED0_ON_L, values, 4);
}
