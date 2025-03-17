#include "initialisations.h"
#define SUBSCRIBE_ON 1
uint32_t g_subcribeEntry_count=0;

#ifdef DO_THE_THING
/* MQsub Thread entry function */
/* pvParameters contains TaskHandle_t */
void subscribe_thread_entry(void *pvParameters) {
    volatile int8_t DUR = 0;
    volatile float kmdc_thres_val = -1.0f;
    int status=-1;
    EventBits_t uxBits;

    /* Echo Progress */
    APP_PRINT("\r\n%s Priority 3: subscribe_thread_entry()",RTT_TIMESTAMP());

    /* TODO: add your own code here */
    while (1)  {
        /* Wait for <<g_wifi_even>> == <<MQTT_FLAG_CONNECTED>> */
        uxBits = xEventGroupWaitBits(  g_wifi_event,           // The event group handle
                                       MQTT_FLAG_CONNECTED, // Bits to wait for
                                       pdFALSE,                 // Clear bits on exit
                                       pdTRUE,                 // Wait for both bits (logical AND)
                                       portMAX_DELAY           // Wait indefinitely
                                       );

        //APP_PRINT("\r\n%s Inside Subscribe_thread",RTT_TIMESTAMP());
        //g_subcribeEntry_count++;

#ifdef SUBSCRIBE_ON
        /* Wi-Fi Status Check*/
        assertDa16200_disconnected(); // check if wifi_uart received AP Disconnect
        //assertDa16200_connected();  // check if wifi_uart received AP Connect (MQTT does not connect back automatically)

        /* Assert ANY MQTT MESSAGE? */
        DUR = assertDa16200_MqttMsg("\"DURATION\":",4);
        if (DUR>0){
            /* MAP DUR to state */
            decodeDurState(DUR);
        } // end if (STATUS_CMD)
        else{
            /* Get Err Sequence[60] of Audio Resend Request */
            status=assertDa16200_AudioResend("\"ERROR\":",AudioResendSelector_ptr(),60,0);
            if (status == FSP_SUCCESS){
                /* Get the checksum ID[11] of Audio Resend Request */
                status=assertDa16200_AudioResend("\"ID\":",AudioResendID_ptr(),10,1);
            } // end if (AUDIO_RESEND)
        } // end if (!STATUS_CMD)

        /* Assert ANY KMDC MESSAGE? */
        kmdc_thres_val = assertDa16200_MqttMsgF("\"KMDC_THRESHOLD\": ",6);
        if (kmdc_thres_val>0.0f){
            Set_KMDC_Threshold(kmdc_thres_val);
            char kmdc_thres_str[7];  // Buffer to hold the string representation
            snprintf(kmdc_thres_str, sizeof(kmdc_thres_str), "%.2f", kmdc_thres_val);
            APP_PRINT("\r\n%s New kmdc_thres= %s",RTT_TIMESTAMP(),kmdc_thres_str);
        }
        vStartOtaDemo();
#endif
        vTaskDelay (10);
    } // end while(1)
    FSP_PARAMETER_NOT_USED(pvParameters);
    FSP_PARAMETER_NOT_USED(uxBits);
    FSP_PARAMETER_NOT_USED(status);
} // end subscribe_thread_entry()
#else
void subscribe_thread_entry(void *pvParameters){
    volatile fsp_err_t status=FSP_ERR_ASSERTION;
    while (1)
    {
        vTaskDelay (1);
    }
    FSP_PARAMETER_NOT_USED (status);
    FSP_PARAMETER_NOT_USED (pvParameters);

} // end subscribe_thread_entry()
#endif
