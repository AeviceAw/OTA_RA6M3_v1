#include "initialisations.h"
#define PUBLISH_ON 1
uint32_t g_pubEntry_count=0;

#ifdef DO_THE_THING
/* MQpub Thread entry function */
/* pvParameters contains TaskHandle_t */
void publish_thread_entry(void *pvParameters){
    volatile fsp_err_t status = FSP_ERR_ASSERTION;

    /* Echo Progress */
    APP_PRINT("\r\n%s Priority 2: publish_thread_entry()",RTT_TIMESTAMP());

    /* TODO: add your own code here */
    while (1)  {
        xSemaphoreTake(g_publish_semaphore,portMAX_DELAY);
        APP_PRINT("\r\n%s Inside Publish",RTT_TIMESTAMP());
        g_pubEntry_count++;

#ifdef PUBLISH_ON
        /* Get the system states and perform the publishing*/
        publish_state_actuator(getTrueCheckVal(), dataprocess_state_get());
        //publish_state_actuator_tester(getTrueCheckVal(), dataprocess_state_get());

#endif
        vTaskDelay (1);
    } // end while(1)
    FSP_PARAMETER_NOT_USED (pvParameters);
    FSP_PARAMETER_NOT_USED(status);
} // end publish_thread_entry()
#else
void publish_thread_entry(void *pvParameters){
    volatile fsp_err_t status=FSP_ERR_ASSERTION;
    while (1)
    {
        vTaskDelay (1);
    }
    FSP_PARAMETER_NOT_USED (status);
    FSP_PARAMETER_NOT_USED (pvParameters);

}
#endif
