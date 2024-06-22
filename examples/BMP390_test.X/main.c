/*
 * File:   main.c
 * Author: ben
 *
 * Created on August 3, 2023, 9:43 PM
 */

#define F_CPU 20000000
#define BAUD_RATE 9600
#define BMP390_ADDRESS 0b01110110

#include <xc.h>
#include <stdio.h>
#include <util/delay.h>
#include "../../BMP390.X/bmp390.h"
#include "../../uart/uart.h"

char stringBuffer[120];

int main(void) {
    // Set CPU clock divider to 1
    CCP = CCP_IOREG_gc; //Configuration Change Protection
    CLKCTRL.MCLKCTRLB = 0; //Clock Div = 1
    
    uart0_init(BAUD_RATE);
    uart0_send_string((char*)"* * * * * *\r\n\0");
    
    I2cInitialize();
    uart0_send_string((char*)"I2cInitialize complete\r\n\0");
    
    uint8_t chip_id = I2cReadByte(BMP390_ADDRESS, BMP390_REG_CHIP_ID);
    uart0_send_string((char*)" Chip ID: \0");
    uart0_print_u8(chip_id);
    uart0_send_string((char*)"\r\n\0");
    
    uint8_t rev_id = I2cReadByte(BMP390_ADDRESS, BMP390_REG_REV_ID);
    uart0_send_string((char*)" Rev ID: \0");
    uart0_print_u8(rev_id);
    uart0_send_string((char*)"\r\n\0");
    
    Bmp390CalibrationData calibration_data;
    Bmp390Initialize(BMP390_ADDRESS, &calibration_data);
    
    sprintf(stringBuffer, "t1 = %g\r\n", calibration_data.t1);
    uart0_send_string(stringBuffer);
    sprintf(stringBuffer, "t2 = %g\r\n", calibration_data.t2);
    uart0_send_string(stringBuffer);
    sprintf(stringBuffer, "t3 = %g\r\n", calibration_data.t3);
    uart0_send_string(stringBuffer);
    sprintf(stringBuffer, "p1 = %g\r\n", calibration_data.p1);
    uart0_send_string(stringBuffer);
    sprintf(stringBuffer, "p2 = %g\r\n", calibration_data.p2);
    uart0_send_string(stringBuffer);
    sprintf(stringBuffer, "p3 = %g\r\n", calibration_data.p3);
    uart0_send_string(stringBuffer);
    sprintf(stringBuffer, "p4 = %g\r\n", calibration_data.p4);
    uart0_send_string(stringBuffer);
    sprintf(stringBuffer, "p5 = %g\r\n", calibration_data.p5);
    uart0_send_string(stringBuffer);
    sprintf(stringBuffer, "p6 = %g\r\n", calibration_data.p6);
    uart0_send_string(stringBuffer);
    sprintf(stringBuffer, "p7 = %g\r\n", calibration_data.p7);
    uart0_send_string(stringBuffer);
    sprintf(stringBuffer, "p8 = %g\r\n", calibration_data.p8);
    uart0_send_string(stringBuffer);
    sprintf(stringBuffer, "p9 = %g\r\n", calibration_data.p9);
    uart0_send_string(stringBuffer);
    sprintf(stringBuffer, "p10 = %g\r\n", calibration_data.p10);
    uart0_send_string(stringBuffer);
    sprintf(stringBuffer, "p11 = %g\r\n", calibration_data.p11);
    uart0_send_string(stringBuffer);
    
    Bmp390SetIirFilterCoefficient(BMP390_ADDRESS, BMP390_IIR_FILTER_COEF_3_bm);
    Bmp390SetOverSampleRates(BMP390_ADDRESS, BMP390_OSR_PRESSURE_8x_bm & BMP390_OSR_TEMPERATURE_1x_bm);
    Bmp390SetOutputDataRate(BMP390_ADDRESS, BMP390_ODR_6p25_HZ);
    
    while (1) {
        float temp_c = Bmp390ReadTemperatureInC(BMP390_ADDRESS, &calibration_data);
        float temp_f = temp_c * 1.8 + 32;
        sprintf(stringBuffer, "\r\nTemp is %f C\r\n", temp_c);
        uart0_send_string(stringBuffer);
        sprintf(stringBuffer, "Temp is %f F\r\n", temp_f);
        uart0_send_string(stringBuffer);
        
        float pressure_pa = Bmp390ReadPressureInPa(BMP390_ADDRESS, &calibration_data);
        sprintf(stringBuffer, "Pressure is %f pa\r\n", pressure_pa);
        uart0_send_string(stringBuffer);
        
        _delay_ms(5000);
    }
    
    return 0;
}
