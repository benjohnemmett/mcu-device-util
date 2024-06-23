# MCU Device Util
Common C libraries for interfacing with devices. Based on MPLAB X IDE projects. Currently only supports ATMega4809, but is made to be extended to other platforms. 

# Libraries
- MPU-6050
- PCA9685
- BMP390

# Examples
- I2C Scanner: Utility that scans all I2C addresses and prints a message to the UART when a device is found.
- MPU-6050 Test: Configures the MPU-6050 over I2C and reads acceleration and rotational motion data.
- PCA9685 Test: Configures the PCA9685 over I2C for controlling LEDS or servo motors with PWM.
- BMP390 Test: Configures the BMP390, reads pressure & temperature data, and writes data to the UART. 
