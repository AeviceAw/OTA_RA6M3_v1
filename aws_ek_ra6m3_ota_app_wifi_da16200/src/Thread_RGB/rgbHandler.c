/*
 * rgbHandler.c
 *
 *  Created on: 9 Aug 2024
 *      Author: yyang
 */
#include "initialisations.h"

volatile i2c_master_event_t g_master_event = (i2c_master_event_t)I2C_RESET_VALUE;
static bool rgb_init_status     = false;
RGB_Colour color         = PURPLE_PULSING;       //system tracked LED state
uint8_t g_master_buf[I2C_BUF_LEN] = {0x00, 0x00};

RGB_Colour LED_PREV=PURPLE_PULSING;
RGB_Colour LED_CURR=PURPLE_PULSING;

void i2c_rgb_callback(i2c_master_callback_args_t *p_args) {
    if (NULL != p_args) {
        g_master_event = p_args->event;
    } // end if (event)
} // end i2c_rgb_callback()

/*******************************************************************************************************************//**
 *  @brief       Master API to Open the I2C Port and Initialize RGB and Color through
 *  @param[in]   None
 *  @retval      fsp_err_t            FSP_SUCCESS
 **********************************************************************************************************************/
fsp_err_t RGB_Init(void){

    volatile fsp_err_t status=FSP_ERR_ASSERTION;

    status = R_IIC_MASTER_Open (&g_i2c_rgb_master_ctrl, &g_i2c_rgb_master_cfg);

    status &= IIC0_Master_2_Byte_Write(SHUTDOWN_ADD, WRITE_SHUTDOWN_REG);
    status &= IIC0_Master_2_Byte_Write(LED_MODE_ADD, WRITE_LED_MODE_REG_2);
    status &= IIC0_Master_2_Byte_Write(CURRENT_ADD, WRITE_CURRENT_REG);
    status &= IIC0_Master_2_Byte_Write(PWM1_ADD, 0xFF);
    status &= IIC0_Master_2_Byte_Write(PWM2_ADD, 0xFF);
    status &= IIC0_Master_2_Byte_Write(PWM3_ADD, 0xFF);
    status &= IIC0_Master_2_Byte_Write(DATA_UPDATE_ADD, WRITE_DATA_UPDATE_REG);
    status &= IIC0_Master_2_Byte_Write(T01_ADD, WRITE_T01_REG);
    status &= IIC0_Master_2_Byte_Write(T02_ADD, WRITE_T02_REG);
    status &= IIC0_Master_2_Byte_Write(T03_ADD, WRITE_T03_REG);
    status &= IIC0_Master_2_Byte_Write(T1T21_ADD, WRITE_T1T21_REG);
    status &= IIC0_Master_2_Byte_Write(T1T22_ADD, WRITE_T1T22_REG);
    status &= IIC0_Master_2_Byte_Write(T1T23_ADD, WRITE_T1T23_REG);
    status &= IIC0_Master_2_Byte_Write(T3T41_ADD, WRITE_T3T41_REG);
    status &= IIC0_Master_2_Byte_Write(T3T42_ADD, WRITE_T3T42_REG);
    status &= IIC0_Master_2_Byte_Write(T3T43_ADD, WRITE_T3T43_REG);
    status &= IIC0_Master_2_Byte_Write(LED_CONTROL_ADD, WRITE_LED_CONTROL_REG);
    status &= IIC0_Master_2_Byte_Write(TIME_UPDATE_ADD, WRITE_TIME_UPDATE_REG);

    /* RGB Initialize Sequence */
    uint8_t sequence_type = 1;
    switch (sequence_type){
        case 0:
            TEST_RGB_COLOR();
            break;
        case 1:
            RGB_Startup_Sequence();
            break;
    } // end switch(LED_TEST)

    status &= RGB_Write_Colour(PURPLE_PULSING);

    return status;
} // end RGB_init()

/*******************************************************************************************************************//**
 *  @brief       Master API to Run Startup Color Modes
 *  @param[in]   None
 *  @retval      None
 **********************************************************************************************************************/
void RGB_Startup_Sequence(void) {
    bool status = false;
    RGB_Colour startUpSeq[] = {
                               CYAN_GLOWING, GREEN_GLOWING, CYAN_GLOWING, GREEN_GLOWING,
                               CYAN_GLOWING, GREEN_GLOWING, CYAN_GLOWING, GREEN_GLOWING};

    for(uint8_t i = 0; i < sizeof(startUpSeq); i++) {
        status = RGB_Write_Colour(startUpSeq[i]);
        if(status!=FSP_SUCCESS) {
            break;//failed to write
        }
        vTaskDelay(STARTUP_LED_DELAY_COUNT);
    } // end loop through LED Color

    if(status==FSP_SUCCESS) {
        rgb_init_status = true;//RGB successfully initialised
    } // end if FSP_SUCCESS
} // end RGB_Startup_Sequence()

/*******************************************************************************************************************//**
 *  @brief       Master API to Test the Color Modes
 *  @param[in]   None
 *  @retval      None
 **********************************************************************************************************************/
void TEST_LED_Colors(void) {
    RGB_Write_Colour(PURPLE_GLOWING);
    vTaskDelay(500); // Delay for 0.5 sec
    RGB_Write_Colour(PURPLE_PULSING);
    vTaskDelay(500); // Delay for 0.5 sec
    RGB_Write_Colour(GREEN_GLOWING);
    vTaskDelay(500); // Delay for 0.5 sec
    RGB_Write_Colour(GREEN_PULSING);
    vTaskDelay(500); // Delay for 0.5 sec
    RGB_Write_Colour(CYAN_GLOWING);
    vTaskDelay(500); // Delay for 0.5 sec
    RGB_Write_Colour(CYAN_PULSING);
    vTaskDelay(500); // Delay for 0.5 sec
} // end TEST_LED_Colors()


/*******************************************************************************************************************//**
 *  @brief       Master API to Test the RGB
 *  @param[in]   None
 *  @retval      None
 **********************************************************************************************************************/
void TEST_RGB_COLOR(void) {
    RGB_Write_Colour(RED);
    vTaskDelay(1000); // Delay for 1.0 sec
    RGB_Write_Colour(BLUE);
    vTaskDelay(1000); // Delay for 1.0 sec
    RGB_Write_Colour(GREEN);
    vTaskDelay(1000); // Delay for 1.0 sec
    RGB_Write_Colour(RED);
    vTaskDelay(1000); // Delay for 1.0 sec
    RGB_Write_Colour(BLUE);
    vTaskDelay(1000); // Delay for 1.0 sec
    RGB_Write_Colour(GREEN);
    vTaskDelay(1000); // Delay for 1.0 sec
} // end TEST_RGB_COLOR()

/*******************************************************************************************************************//**
 *  @brief       Master API to actuate a color on the LED
 *  @param[in]   RGB_Colour_t    <<inColour>>
 *  @retval      fsp_err_t            <<0/1>>  No meaning at the moment
 **********************************************************************************************************************/
fsp_err_t RGB_Write_Colour(RGB_Colour inColour) {
    volatile fsp_err_t writeStatus = 0;//assume true

    LED_CURR = inColour;

    if (LED_PREV==inColour)
        return 0; // No need to change

    //Only update when there is a color change
    switch(inColour) {
        case PURPLE_GLOWING:        {
            writeStatus = IIC0_Master_2_Byte_Write(LED_MODE_ADD, WRITE_LED_MODE_REG_2);
            writeStatus |= IIC0_Master_2_Byte_Write(PWM1_ADD, 0x00);//green
            writeStatus |= IIC0_Master_2_Byte_Write(PWM2_ADD, 0xFF);//red
            writeStatus |= IIC0_Master_2_Byte_Write(PWM3_ADD, 0x96);//blue
            writeStatus |= IIC0_Master_2_Byte_Write(DATA_UPDATE_ADD, WRITE_DATA_UPDATE_REG);
            break;
        }

        case PURPLE_PULSING:        {
            writeStatus = IIC0_Master_2_Byte_Write(PWM1_ADD, 0x00);//green
            writeStatus |= IIC0_Master_2_Byte_Write(PWM2_ADD, 0xFF);//red
            writeStatus |= IIC0_Master_2_Byte_Write(PWM3_ADD, 0x96);//blue
            writeStatus |= IIC0_Master_2_Byte_Write(DATA_UPDATE_ADD, WRITE_DATA_UPDATE_REG);

            writeStatus |= IIC0_Master_2_Byte_Write(LED_MODE_ADD, WRITE_LED_MODE_REG_1);
            writeStatus |= IIC0_Master_2_Byte_Write(BREATHING_CONTROL_ADD, WRITE_BREATHING_CONTROL_REG);
            writeStatus |= IIC0_Master_2_Byte_Write(T1T21_ADD, 0x24); // Change Request: 12-Jan-2021 from 0x24 to 0x68
            writeStatus |= IIC0_Master_2_Byte_Write(T1T22_ADD, 0x24); //                 To match the Pulsing rate of Green and Blue
            writeStatus |= IIC0_Master_2_Byte_Write(T1T23_ADD, 0x24); // Change Request: 13-Jan-2021 from 0x68 to 0x24
            writeStatus |= IIC0_Master_2_Byte_Write(T3T41_ADD, 0x24); //                 Revert back to Previous version
            writeStatus |= IIC0_Master_2_Byte_Write(T3T42_ADD, 0x24); //                 Flashing to indicate wifi is trying hard to connect
            writeStatus |= IIC0_Master_2_Byte_Write(T3T43_ADD, 0x24);
            writeStatus |= IIC0_Master_2_Byte_Write(TIME_UPDATE_ADD, WRITE_TIME_UPDATE_REG); //time update register
            break;
        }
        case CYAN_GLOWING:        {
            writeStatus = IIC0_Master_2_Byte_Write(LED_MODE_ADD, WRITE_LED_MODE_REG_2);
            writeStatus |= IIC0_Master_2_Byte_Write(PWM1_ADD, 0xFF);//green
            writeStatus |= IIC0_Master_2_Byte_Write(PWM2_ADD, 0x00);//red
            writeStatus |= IIC0_Master_2_Byte_Write(PWM3_ADD, 0xFF);//blue
            writeStatus |= IIC0_Master_2_Byte_Write(DATA_UPDATE_ADD, WRITE_DATA_UPDATE_REG);
            break;
        }
        case CYAN_PULSING:        {
            writeStatus = IIC0_Master_2_Byte_Write(PWM1_ADD, 0xFF);//green
            writeStatus |= IIC0_Master_2_Byte_Write(PWM2_ADD, 0x00);//red
            writeStatus |= IIC0_Master_2_Byte_Write(PWM3_ADD, 0xFF);//blue
            writeStatus |= IIC0_Master_2_Byte_Write(DATA_UPDATE_ADD, WRITE_DATA_UPDATE_REG);

            writeStatus |= IIC0_Master_2_Byte_Write(LED_MODE_ADD, WRITE_LED_MODE_REG_1);
            writeStatus |= IIC0_Master_2_Byte_Write(BREATHING_CONTROL_ADD, WRITE_BREATHING_CONTROL_REG);
            writeStatus |= IIC0_Master_2_Byte_Write(T1T21_ADD, 0x68);
            writeStatus |= IIC0_Master_2_Byte_Write(T1T22_ADD, 0x68);
            writeStatus |= IIC0_Master_2_Byte_Write(T1T23_ADD, 0x68);
            writeStatus |= IIC0_Master_2_Byte_Write(T3T41_ADD, 0x68);
            writeStatus |= IIC0_Master_2_Byte_Write(T3T42_ADD, 0x68);
            writeStatus |= IIC0_Master_2_Byte_Write(T3T43_ADD, 0x68);
            writeStatus |= IIC0_Master_2_Byte_Write(TIME_UPDATE_ADD, WRITE_TIME_UPDATE_REG); //time update register
            break;
        }
        case GREEN_GLOWING:        {
            writeStatus = IIC0_Master_2_Byte_Write(LED_MODE_ADD, WRITE_LED_MODE_REG_2);
            writeStatus |= IIC0_Master_2_Byte_Write(PWM1_ADD, 0xFF);//green
            writeStatus |= IIC0_Master_2_Byte_Write(PWM2_ADD, 0xFF);//red
            writeStatus |= IIC0_Master_2_Byte_Write(PWM3_ADD, 0x00);//blue
            writeStatus |= IIC0_Master_2_Byte_Write(DATA_UPDATE_ADD, WRITE_DATA_UPDATE_REG);
            break;
        }
        case GREEN_PULSING:        {
            writeStatus = IIC0_Master_2_Byte_Write(PWM1_ADD, 0xFF);//green
            writeStatus |= IIC0_Master_2_Byte_Write(PWM2_ADD, 0xFF);//red
            writeStatus |= IIC0_Master_2_Byte_Write(PWM3_ADD, 0x00);//blue
            writeStatus |= IIC0_Master_2_Byte_Write(DATA_UPDATE_ADD, WRITE_DATA_UPDATE_REG);

            writeStatus |= IIC0_Master_2_Byte_Write(LED_MODE_ADD, WRITE_LED_MODE_REG_1);
            writeStatus |= IIC0_Master_2_Byte_Write(BREATHING_CONTROL_ADD, WRITE_BREATHING_CONTROL_REG);
            writeStatus |= IIC0_Master_2_Byte_Write(T1T21_ADD, 0x68);
            writeStatus |= IIC0_Master_2_Byte_Write(T1T22_ADD, 0x68);
            writeStatus |= IIC0_Master_2_Byte_Write(T1T23_ADD, 0x68);
            writeStatus |= IIC0_Master_2_Byte_Write(T3T41_ADD, 0x68);
            writeStatus |= IIC0_Master_2_Byte_Write(T3T42_ADD, 0x68);
            writeStatus |= IIC0_Master_2_Byte_Write(T3T43_ADD, 0x68);
            writeStatus |= IIC0_Master_2_Byte_Write(TIME_UPDATE_ADD, WRITE_TIME_UPDATE_REG); //time update register
            break;
        }
        case WHITE:
        {
            writeStatus &= IIC0_Master_2_Byte_Write(LED_MODE_ADD, WRITE_LED_MODE_REG_2);
            writeStatus &= IIC0_Master_2_Byte_Write(PWM1_ADD, 0xFF);//green
            writeStatus &= IIC0_Master_2_Byte_Write(PWM2_ADD, 0xFF);//red
            writeStatus &= IIC0_Master_2_Byte_Write(PWM3_ADD, 0xFF);//blue
            writeStatus &= IIC0_Master_2_Byte_Write(DATA_UPDATE_ADD, WRITE_DATA_UPDATE_REG);
            break;
        }
        case RED:
        {
            writeStatus &= IIC0_Master_2_Byte_Write(LED_MODE_ADD, WRITE_LED_MODE_REG_2);
            writeStatus &= IIC0_Master_2_Byte_Write(PWM1_ADD, 0x00);//green
            writeStatus &= IIC0_Master_2_Byte_Write(PWM2_ADD, 0xFF);//red
            writeStatus &= IIC0_Master_2_Byte_Write(PWM3_ADD, 0x00);//blue
            writeStatus &= IIC0_Master_2_Byte_Write(DATA_UPDATE_ADD, WRITE_DATA_UPDATE_REG);
            break;
        }
        case BLUE:
        {
            writeStatus &= IIC0_Master_2_Byte_Write(LED_MODE_ADD, WRITE_LED_MODE_REG_2);
            writeStatus &= IIC0_Master_2_Byte_Write(PWM1_ADD, 0x00);//green
            writeStatus &= IIC0_Master_2_Byte_Write(PWM2_ADD, 0x00);//red
            writeStatus &= IIC0_Master_2_Byte_Write(PWM3_ADD, 0xFF);//blue
            writeStatus &= IIC0_Master_2_Byte_Write(DATA_UPDATE_ADD, WRITE_DATA_UPDATE_REG);
            break;
        }
        case GREEN:
        {
            writeStatus &= IIC0_Master_2_Byte_Write(LED_MODE_ADD, WRITE_LED_MODE_REG_2);
            writeStatus &= IIC0_Master_2_Byte_Write(PWM1_ADD, 0xFF);//green
            writeStatus &= IIC0_Master_2_Byte_Write(PWM2_ADD, 0x00);//red
            writeStatus &= IIC0_Master_2_Byte_Write(PWM3_ADD, 0x00);//blue
            writeStatus &= IIC0_Master_2_Byte_Write(DATA_UPDATE_ADD, WRITE_DATA_UPDATE_REG);
            break;
        }

        case NO_LIGHT:
        {
            writeStatus &= IIC0_Master_2_Byte_Write(LED_MODE_ADD, WRITE_LED_MODE_REG_2);
            writeStatus &= IIC0_Master_2_Byte_Write(PWM1_ADD, 0x00);//green
            writeStatus &= IIC0_Master_2_Byte_Write(PWM2_ADD, 0x00);//red
            writeStatus &= IIC0_Master_2_Byte_Write(PWM3_ADD, 0x00);//blue
            writeStatus &= IIC0_Master_2_Byte_Write(DATA_UPDATE_ADD, WRITE_DATA_UPDATE_REG);
            break;
        }
        case BLUE_GLOWING:
        {
            writeStatus &= IIC0_Master_2_Byte_Write(LED_MODE_ADD, WRITE_LED_MODE_REG_2);
            writeStatus &= IIC0_Master_2_Byte_Write(PWM1_ADD, 0x77);//green
            writeStatus &= IIC0_Master_2_Byte_Write(PWM2_ADD, 0x00);//red
            writeStatus &= IIC0_Master_2_Byte_Write(PWM3_ADD, 0xFF);//blue
            writeStatus &= IIC0_Master_2_Byte_Write(DATA_UPDATE_ADD, WRITE_DATA_UPDATE_REG);
            break;
        }
        case BLUE_PULSING:
        {
            writeStatus &= IIC0_Master_2_Byte_Write(PWM1_ADD, 0x77);//green
            writeStatus &= IIC0_Master_2_Byte_Write(PWM2_ADD, 0x00);//red
            writeStatus &= IIC0_Master_2_Byte_Write(PWM3_ADD, 0xFF);//blue
            writeStatus &= IIC0_Master_2_Byte_Write(DATA_UPDATE_ADD, WRITE_DATA_UPDATE_REG);

            writeStatus &= IIC0_Master_2_Byte_Write(LED_MODE_ADD, WRITE_LED_MODE_REG_1);
            writeStatus &= IIC0_Master_2_Byte_Write(BREATHING_CONTROL_ADD, WRITE_BREATHING_CONTROL_REG);
            writeStatus &= IIC0_Master_2_Byte_Write(T1T21_ADD, 0x24);
            writeStatus &= IIC0_Master_2_Byte_Write(T1T22_ADD, 0x24);
            writeStatus &= IIC0_Master_2_Byte_Write(T1T23_ADD, 0x24);
            writeStatus &= IIC0_Master_2_Byte_Write(T3T41_ADD, 0x24);
            writeStatus &= IIC0_Master_2_Byte_Write(T3T42_ADD, 0x24);
            writeStatus &= IIC0_Master_2_Byte_Write(T3T43_ADD, 0x24);
            writeStatus &= IIC0_Master_2_Byte_Write(TIME_UPDATE_ADD, WRITE_TIME_UPDATE_REG); //time update register
            break;
        }
        case WHITE_GLOWING:
        {
            writeStatus &= IIC0_Master_2_Byte_Write(LED_MODE_ADD, WRITE_LED_MODE_REG_2);
            writeStatus &= IIC0_Master_2_Byte_Write(PWM1_ADD, 0xAA);//green
            writeStatus &= IIC0_Master_2_Byte_Write(PWM2_ADD, 0xAA);//red
            writeStatus &= IIC0_Master_2_Byte_Write(PWM3_ADD, 0xAA);//blue
            writeStatus &= IIC0_Master_2_Byte_Write(DATA_UPDATE_ADD, WRITE_DATA_UPDATE_REG);
            break;
        }
        case WHITE_PULSING:
        {
            writeStatus &= IIC0_Master_2_Byte_Write(PWM1_ADD, 0xAA);//green
            writeStatus &= IIC0_Master_2_Byte_Write(PWM2_ADD, 0xAA);//red
            writeStatus &= IIC0_Master_2_Byte_Write(PWM3_ADD, 0xAA);//blue
            writeStatus &= IIC0_Master_2_Byte_Write(DATA_UPDATE_ADD, WRITE_DATA_UPDATE_REG);

            writeStatus &= IIC0_Master_2_Byte_Write(LED_MODE_ADD, WRITE_LED_MODE_REG_1);
            writeStatus &= IIC0_Master_2_Byte_Write(BREATHING_CONTROL_ADD, WRITE_BREATHING_CONTROL_REG);
            writeStatus &= IIC0_Master_2_Byte_Write(T1T21_ADD, 0x68);
            writeStatus &= IIC0_Master_2_Byte_Write(T1T22_ADD, 0x68);
            writeStatus &= IIC0_Master_2_Byte_Write(T1T23_ADD, 0x68);
            writeStatus &= IIC0_Master_2_Byte_Write(T3T41_ADD, 0x68);
            writeStatus &= IIC0_Master_2_Byte_Write(T3T42_ADD, 0x68);
            writeStatus &= IIC0_Master_2_Byte_Write(T3T43_ADD, 0x68);
            writeStatus &= IIC0_Master_2_Byte_Write(TIME_UPDATE_ADD, WRITE_TIME_UPDATE_REG); //time update register
            break;
        }
        case ORANGE_GLOWING:
        {
            writeStatus &= IIC0_Master_2_Byte_Write(LED_MODE_ADD, WRITE_LED_MODE_REG_2);
            writeStatus &= IIC0_Master_2_Byte_Write(PWM1_ADD, 0x66);//green
            writeStatus &= IIC0_Master_2_Byte_Write(PWM2_ADD, 0xFF);//red
            writeStatus &= IIC0_Master_2_Byte_Write(PWM3_ADD, 0x00);//blue
            writeStatus &= IIC0_Master_2_Byte_Write(DATA_UPDATE_ADD, WRITE_DATA_UPDATE_REG);
            break;
        }
        case ORANGE_PULSING:
        {
            writeStatus &= IIC0_Master_2_Byte_Write(PWM1_ADD, 0x66);//green
            writeStatus &= IIC0_Master_2_Byte_Write(PWM2_ADD, 0xFF);//red
            writeStatus &= IIC0_Master_2_Byte_Write(PWM3_ADD, 0x00);//blue
            writeStatus &= IIC0_Master_2_Byte_Write(DATA_UPDATE_ADD, WRITE_DATA_UPDATE_REG);

            writeStatus &= IIC0_Master_2_Byte_Write(LED_MODE_ADD, WRITE_LED_MODE_REG_1);
            writeStatus &= IIC0_Master_2_Byte_Write(BREATHING_CONTROL_ADD, WRITE_BREATHING_CONTROL_REG);
            writeStatus &= IIC0_Master_2_Byte_Write(T1T21_ADD, 0x68);
            writeStatus &= IIC0_Master_2_Byte_Write(T1T22_ADD, 0x68);
            writeStatus &= IIC0_Master_2_Byte_Write(T1T23_ADD, 0x68);
            writeStatus &= IIC0_Master_2_Byte_Write(T3T41_ADD, 0x68);
            writeStatus &= IIC0_Master_2_Byte_Write(T3T42_ADD, 0x68);
            writeStatus &= IIC0_Master_2_Byte_Write(T3T43_ADD, 0x68);
            writeStatus &= IIC0_Master_2_Byte_Write(TIME_UPDATE_ADD, WRITE_TIME_UPDATE_REG); //time update register
            break;
        }
    } // end switch

    /* Update <<LED_PREV>> */
    LED_PREV = inColour; // Update

    /* Update LED to Cloud */
    // TODO: YY to complete the publishing of RGB to Logic
    if (internet_state_get(G_MQTT))
        publishSysLed(DNAME,LED_color_get(),8, 4);

    return writeStatus;
} // end RGB_Write_Colour()


/*******************************************************************************************************************//**
 *  @brief       Get the current RGB Color value
 *  @param[in]   None
 *  @retval      RGB_Colour <<LED_CURR>>  Expect: PURPLE/GREEN/CYAN
 **********************************************************************************************************************/
RGB_Colour LED_color_get(void){
    return LED_CURR;
} // LED_color_get()


/*******************************************************************************************************************//**
 *  @brief       Master API to write to the RGB I2C Lines
 *  @param[in]   uint8_t    <<inRegAdd>>  I2C's Destination Address
 *  @param[in]   uint8_t    <<inRegData>> I2C's Destination Data
 *  @retval      fsp_err_t <<status>>  Expect: FSP_SUCCESS
 *  @Remark      Make sure the IIC_MASTER is open first
 **********************************************************************************************************************/
volatile fsp_err_t status= FSP_ERR_ASSERTION;
fsp_err_t IIC0_Master_2_Byte_Write(uint8_t inRegAdd, uint8_t inRegDat) {
    uint8_t write_buff[I2C_BUF_LEN]={0};
    write_buff[0] = inRegAdd;
    write_buff[1] = inRegDat;

    memcpy(g_master_buf, write_buff, I2C_BUF_LEN);
    g_master_event = (i2c_master_event_t)I2C_RESET_VALUE;

    status = R_IIC_MASTER_Write(&g_i2c_rgb_master_ctrl, write_buff, I2C_BUF_LEN, false);
    vTaskDelay(10);//Delay to show in oscilloscope
    //vTaskDelay(1000); // Delay for 1.0 sec
    return status;
} // end IIC0_Master_2_Byte_Write()
