/*
 * rgbHandler.h
 *
 *  Created on: 9 Aug 2024
 *      Author: yyang
 */

#ifndef RGBHANDLER_H_
#define RGBHANDLER_H_

#include "initialisations.h"

#define I2C_BUF_LEN                 2
#define I2C_RESET_VALUE             (0x00)
#define STARTUP_LED_DELAY_COUNT     (200U) //200 == 200ms
#define PRINT_COLOUR_INFO           0

typedef enum RGB_Colour_enum {
    WHITE           = 0,
    RED             = 1,
    BLUE            = 2,
    GREEN           = 3,
    PURPLE_GLOWING  = 4,
    PURPLE_PULSING  = 5,
    NO_LIGHT        = 6,
    GREEN_GLOWING   = 7,
    GREEN_PULSING   = 8,
    BLUE_GLOWING    = 9,
    BLUE_PULSING    = 10,
    WHITE_GLOWING   = 11,
    WHITE_PULSING   = 12,
    ORANGE_GLOWING  = 13,
    ORANGE_PULSING  = 14,
    CYAN_GLOWING    = 15,
    CYAN_PULSING    = 16,
}RGB_Colour;


/* RGB thread definitions & initializations*/
#define DEV_ADD                     (0XD0)

#define WRITE_REG0                  (0x00)
#define READ_REG0                   (0x24)


#define SHUTDOWN_ADD                (0x00)
#define WRITE_SHUTDOWN_REG          (0x20)

#define BREATHING_CONTROL_ADD           (0x01)
#define WRITE_BREATHING_CONTROL_REG     (0x40)
#define WRITE_BREATHING_CONTROL_REG_RM  (0x20) //RM is 0x20

#define LED_MODE_ADD                (0x02)
#define WRITE_LED_MODE_REG_1        (0x20) //One Shot
#define WRITE_LED_MODE_REG_2        (0x00) //PWM

#define CURRENT_ADD                 (0x03)
#define WRITE_CURRENT_REG           (0x08)

#define PWM1_ADD                    (0x04)
#define WRITE_PWM1_REG              (0xFF)

#define PWM2_ADD                    (0x06)
#define WRITE_PWM2_REG              (0xFF)

#define PWM3_ADD                    (0x05)
#define WRITE_PWM3_REG              (0xFF)

#define DATA_UPDATE_ADD             (0x07)
#define WRITE_DATA_UPDATE_REG       (0x00)

#define T01_ADD                     (0x0A)
#define WRITE_T01_REG               (0x00)

#define T02_ADD                     (0x0B)
#define WRITE_T02_REG               (0x00)

#define T03_ADD                     (0x0C)
#define WRITE_T03_REG               (0x00)

#define T1T21_ADD                   (0x10)
#define WRITE_T1T21_REG             (0x04)

#define T1T22_ADD                   (0x11)
#define WRITE_T1T22_REG             (0x04)

#define T1T23_ADD                   (0x12)
#define WRITE_T1T23_REG             (0x04)

#define T3T41_ADD                   (0x16)
#define WRITE_T3T41_REG             (0x00)

#define T3T42_ADD                   (0x17)
#define WRITE_T3T42_REG             (0x00)

#define T3T43_ADD                   (0X18)
#define WRITE_T3T43_REG             (0x00)

#define TIME_UPDATE_ADD             (0x1C)
#define WRITE_TIME_UPDATE_REG       (0x00)

#define LED_CONTROL_ADD             (0x1D)
#define WRITE_LED_CONTROL_REG       (0x07)

#define RESET_ADD                   (0x2F)
#define WRITE_RESET_REG             (0x00)

#define I2C_BUFF_EQUAL              (0U)

fsp_err_t RGB_Init(void);
void RGB_Set_Curr_LED(RGB_Colour in_color) ;
fsp_err_t IIC0_Master_2_Byte_Write(uint8_t inRegAdd, uint8_t inRegDat);
fsp_err_t RGB_Write_Colour(RGB_Colour inColour);
RGB_Colour LED_color_get(void);
void TEST_LED_Colors(void);
void TEST_RGB_COLOR(void);
void RGB_Startup_Sequence(void);

#endif /* RGBHANDLER_H_ */
