/* 
 * File:   bmp390.h
 * Author: ben
 *
 * Created on August 3, 2023, 9:49 PM
 */

#ifndef BMP390_H
#define	BMP390_H

#include <math.h>
#include <stdint.h>
#include "../I2C.X/i2c_interface.h"
#include "../I2C.X/i2c.h"

#define BMP390_REG_CHIP_ID 0x00
#define BMP390_REG_REV_ID 0x01
#define BMP390_REG_ERR 0x02
#define BMP390_REG_STATUS 0x03
#define BMP390_REG_PRESSURE_DATA_0 0x04
#define BMP390_REG_PRESSURE_DATA_1 0x05
#define BMP390_REG_PRESSURE_DATA_2 0x06
#define BMP390_REG_TEMP_DATA_0 0x07
#define BMP390_REG_TEMP_DATA_1 0x08
#define BMP390_REG_TEMP_DATA_2 0x09
#define BMP390_REG_TIME_DATA_0 0x0C
#define BMP390_REG_TIME_DATA_1 0x0D
#define BMP390_REG_TIME_DATA_2 0x0E
#define BMP390_REG_PWR_CTRL 0x1B
#define BMP390_REG_OSR 0x1C
#define BMP390_REG_ODR 0x1D
#define BMP390_REG_CONFIG 0x1F
#define BMP390_REG_CMD 0x1F

#define BMP390_REG_NVM_PAR_T1 0x31

#define BMP390_PWR_CTRL_START_EVERYTHING_bm 0b00110011
#define BMP390_OSR_PRESSURE_1x_bm 0x00
#define BMP390_OSR_PRESSURE_2x_bm 0x01
#define BMP390_OSR_PRESSURE_4x_bm 0x02
#define BMP390_OSR_PRESSURE_8x_bm 0x03
#define BMP390_OSR_PRESSURE_16x_bm 0x04
#define BMP390_OSR_PRESSURE_32x_bm 0x05
#define BMP390_OSR_TEMPERATURE_1x_bm 0x00 << 3
#define BMP390_OSR_TEMPERATURE_2x_bm 0x01 << 3
#define BMP390_OSR_TEMPERATURE_4x_bm 0x02 << 3
#define BMP390_OSR_TEMPERATURE_8x_bm 0x03 << 3
#define BMP390_OSR_TEMPERATURE_16x_bm 0x04 << 3
#define BMP390_OSR_TEMPERATURE_32x_bm 0x05 << 3
#define BMP390_ODR_200_HZ 0x00
#define BMP390_ODR_100_HZ 0x01
#define BMP390_ODR_500_HZ 0x02
#define BMP390_ODR_25_HZ 0x03
#define BMP390_ODR_12p5_HZ 0x04
#define BMP390_ODR_6p25_HZ 0x05
#define BMP390_IIR_FILTER_COEF_0_BYPASS_bm 0b0000
#define BMP390_IIR_FILTER_COEF_1_bm 0b0010
#define BMP390_IIR_FILTER_COEF_3_bm 0b0100
#define BMP390_IIR_FILTER_COEF_7_bm 0b0110
#define BMP390_IIR_FILTER_COEF_15_bm 0b1000
#define BMP390_IIR_FILTER_COEF_31_bm 0b1010
#define BMP390_IIR_FILTER_COEF_63_bm 0b1100
#define BMP390_IIR_FILTER_COEF_127_bm 0b1110

#define BMP390_TRIMMING_COEFFICENT_LENGTH 21

#ifdef	__cplusplus
extern "C" {
#endif
    
    struct RawTempData {
        uint8_t data0;
        uint8_t data1;
        uint8_t data2;
    };
    typedef struct RawTempData RawTempData;

    struct Bmp390Data {
        float temperature_c;
        float pressure_pa;
    };
    typedef struct Bmp390Data Bmp390Data;
    
    struct Bmp390RawTrimmingCoefficients {
        uint16_t t1;
        uint16_t t2;
        int8_t t3;
        int16_t p1;
        int16_t p2;
        int8_t p3;
        int8_t p4;
        uint16_t p5;
        uint16_t p6;
        int8_t p7;
        int8_t p8;
        int16_t p9;
        int8_t p10;
        int8_t p11;
    };
    typedef struct Bmp390RawTrimmingCoefficients Bmp390RawTrimmingCoefficients;
    
    struct Bmp390CalibrationData {
        float t1;
        float t2;
        float t3;
        float p1;
        float p2;
        float p3;
        float p4;
        float p5;
        float p6;
        float p7;
        float p8;
        float p9;
        float p10;
        float p11;
        float t_lin;
    };
    typedef struct Bmp390CalibrationData Bmp390CalibrationData;
    
    void Bmp390Initialize(I2cFunctions *i2c_functions, char addr, Bmp390CalibrationData *calibration_data);
    
    void Bmp390GetTrimmingCoefficients(I2cFunctions *i2c_functions, char addr, Bmp390RawTrimmingCoefficients *trimming_coefficients);
    
    void Bmp390CalculateCalibrationData(Bmp390RawTrimmingCoefficients *raw_trimming_coefficients, Bmp390CalibrationData *trimming_coefficients);
    
    void Bmp390ReadTemperatureRaw(I2cFunctions *i2c_functions, unsigned char addr, RawTempData *data);

    float Bmp390ReadTemperatureInC(I2cFunctions *i2c_functions, unsigned char addr, Bmp390CalibrationData *calibration_data);
            
    float Bmp390ReadPressureInPa(I2cFunctions *i2c_functions, unsigned char addr);
    
    void Bmp390SetOverSampleRates(I2cFunctions *i2c_functions, unsigned char addr, unsigned char osr_masks);
    
    void Bmp390SetIirFilterCoefficient(I2cFunctions *i2c_functions, unsigned char addr, unsigned char iir_coeff_mask);
    
    void Bmp390SetOutputDataRate(I2cFunctions *i2c_functions, unsigned char addr, unsigned char odr_sel_mask);

#ifdef	__cplusplus
}
#endif

#endif	/* BMP390_H */

