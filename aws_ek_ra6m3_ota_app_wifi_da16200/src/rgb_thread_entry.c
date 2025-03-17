#include "initialisations.h"

/* RGB Thread entry function */
/* pvParameters contains TaskHandle_t */
void rgb_thread_entry(void *pvParameters) {
    FSP_PARAMETER_NOT_USED (pvParameters);
    fsp_err_t status = FSP_ERR_ASSERTION;

    /* Open I2C and Initialize the LED's Registers*/
    status = RGB_Init();

    /* Open RTC */
    RTC_Init();

    /* Open IWDT and Initialize the Struct*/
    status = iwdt_init();

    /* Echo Progress */
    APP_PRINT("\r\n%s Priority 4: rgb_thread_entry()",RTT_TIMESTAMP());

    /* TODO: add your own code here */
    while (1)  {
        vTaskDelay (1);
        xSemaphoreTake(g_rgb_semaphore,1000);

        /* Refresh Watch dog Timer */
        status = iwdt_refresh();
        //APP_PRINT("\r\n%s watch_dog refreshed! Count: %d",RTT_TIMESTAMP(),*iwdt_counter_addr());

        /* current detection read adc0 */
        CD_Read_ADC();

        /* TODO: A more comprehensive Logic*/
        if (1>10){

        }
        else if (getProcessingCallback() && g_feedbackState_get()==fb_READY){//else if (getProcessingCallback()&&g_aws_flags!=CONNECTING){
            RGB_Write_Colour(PURPLE_GLOWING);
        }
        else if (internet_state_get(G_MQTT)!=MQTT_ON){
            RGB_Write_Colour(PURPLE_PULSING);
        }
        else if (g_adc_mA_get()>4.0f){// Current > 4.0 mA = Battery still charging
            RGB_Write_Colour(GREEN_PULSING);
        }
        else if (g_adc_mA_get()>1.0f){// Current > 1.0 mA = Battery Fully charged
            RGB_Write_Colour(GREEN_GLOWING);
        }
        else if (BLEC_state_get()==BLEC_VITALS_OFF){
            RGB_Write_Colour(CYAN_GLOWING);
        }
        else if (!BLE_check()&&(BLEC_state_get()!=BLEC_VITALS_OFF)){
            RGB_Write_Colour(CYAN_PULSING);
        }
        else if (BLE_check()||(BLEC_state_get()==BLEC_VITALS_OFF)){
            RGB_Write_Colour(CYAN_GLOWING);
        }
    } // end while(1)
    FSP_PARAMETER_NOT_USED(status);
} // end rgb_thread_entry()

