#include "initialisations.h"
#define DATAPROCESS_ON 1

#ifdef DO_THE_THING

uint32_t g_dataEntry_count=0;

// ===============================================
// Variables For Continuous Wheeze Detection (CWD)
// ===============================================
float wz_prev = 0.0f; // for continuous wheeze detection
const uint8_t CWD_COUNT_MAX = 2; // Number of CWD before Timeout

// ===============================================
// Variables For Auscultation WZ
// ===============================================
float WZ_aus=-2.0f;


/* Data Thread entry function */
/* pvParameters contains TaskHandle_t */
void dataprocess_thread_entry(void *pvParameters){
    volatile fsp_err_t status=FSP_ERR_ASSERTION;

    /* Initialize CWD 10-min Countdown timer  */
    //initCWDCowntdownTimer();

    /* Perform SRAM Test and Set g_sram_ok */
    g_sram_set(g_sram_test());

    /* Perform KMDC Test: Expect d_err_sum<1e-6 */
    //testKMDC();

    /* Echo Progress */
    APP_PRINT("\r\n%s Priority 1: dataprocess_thread_entry()",RTT_TIMESTAMP());

    /* TODO: add your own code here */
    while (1)  {
        xSemaphoreTake(g_data_semaphore,portMAX_DELAY);
        APP_PRINT("\r\n%s Inside Dataprocess_thread",RTT_TIMESTAMP());

        //g_uart_timer_diff();

        g_dataEntry_count++;

        /* (0) Update Thread State <<g_dataprocess_state>> && <<vpt_state>> */
        dataprocess_state_update(BLEC_state_get(), getTrueCheckVal(),dataprocess_state_get(),vpt_state_get(),cwd_state_get());

        /* (1) Extract <<fdata>>,<<IMU>>,<<batt>>   */
        datapreprocess_compute(); // 0.168 sec
        ///* (1b) Check if Full Buffer: Do not continue if NOT full buffer */
        //if (30<getDataSuccessCount() && getDataSuccessCount()<DATASIZE-128)
        //    continue;
        //// end (Not Full uartData buffer)
        //vTaskDelay (4000);

#ifdef DATAPROCESS_ON
        /* (2) Actuate the correct algorithm based on the state*/
        if (dataprocess_state_get()==AUDIO_INIT){
            /* (2a) Compute HR & RR -> Display */
            vitals_compute(); // 1.502 sec

            /* (2b) Compute WZ -> Display */
            adpcm_encode_decode(); // 0.069 s
            vitals_set(g_WZ,getKMDC()); // 3.418 sec
            vitals_set(WZ_DISPLAY,vitals_get(g_WZ));//vitals_set(WZ_DISPLAY,1.0f);//

            /* 2(c) Update masking of Vitals */
            imu_detection();

            /* 2(d) wz_prev is to transition from AUS_VITALS to AUS_CWD */
            wz_prev=vitals_get(WZ_DISPLAY);// wz_prev=wz_display; // update wz_prev value
            vpt_buf_write();               // Write to Vital Publish Timer & ON VPT_TIMER

            /* 2(e) Set(t_AUS_CWD), Start CWD in Next Iteration, if WZ_DISPLAY==1*/
            inspectCwdNextIterStartCondition(dataprocess_state_get(),cwd_state_get(),BLEC_state_get(), BLE_check(),vitals_get(HR_DISPLAY),vitals_get(WZ_DISPLAY));
        } // end if (DP_STATE_AUDIO_INIT)
        //=================================================================================
        // Putting Publish Semaphore
        //=================================================================================
        else if ((getTrueCheckVal()== t_AUS_MAN_15) && (dataprocess_state_get()==AUDIO_SET1 || dataprocess_state_get()==AUDIO_SET2 || dataprocess_state_get()==AUDIO_SET3) ){
            /* 3(a) ENCODE-Publish MAN Auscultation-Decode*/
            adpcm_encode_decode();

            /* 3(b) Compute WZ */
            vitals_set(g_WZ,getKMDC());
            vitals_set(WZ_DISPLAY,vitals_get(g_WZ));

            /* 3(c) Update WZ_aus */
            WZ_aus = vitals_get(WZ_DISPLAY);
            wz12_set(dataprocess_state_get(),WZ_aus);

        } // end put(AUSCULTATION)
        else if ((getTrueCheckVal()== t_AUS_CWD) && (dataprocess_state_get()==AUDIO_SET1 || dataprocess_state_get()==AUDIO_SET2 || dataprocess_state_get()==AUDIO_SET3) ){
            /* 4(a) ENCODE-Publish CWD Auscultation-Decode*/
            adpcm_encode_decode();

            /* 4(b) Compute WZ */
            vitals_set(g_WZ,getKMDC());//g_wz = getKMDC(); // Perform KMDC computation and get wz results
            vitals_set(WZ_DISPLAY,vitals_get(g_WZ));//vitals_set(WZ_DISPLAY,0.0f);//vitals_set(WZ_DISPLAY,vitals_get(g_WZ));

            /* 4(c) Update masking of Vitals */
            imu_detection();

            /* 4(d) Update WZ_aus */
            WZ_aus = vitals_get(WZ_DISPLAY);
            wz12_set(dataprocess_state_get(),WZ_aus);

            /* 4(e) Check if we should continue CWD Auscultation or stop Prematurely*/
            //(~) (check(wz_display)) ? Publish 19-59 ;  Stop after SEQ 19
            inspectCwdContinueCondition(dataprocess_state_get(),cwd_state_get(),BLEC_state_get(), BLE_check(),imu_onbody_get(),vitals_get(WZ_DISPLAY));

            // 4(f) Turn-off CWD and start CWD timeout-timer if we exceeded count
            inspectCwdTurnOffCondition(cwd_count_get());
        } // end (AUDIO_CWD)
        //=================================================================================
        else if(getTrueCheckVal()== t_AUS_STOP){
            xSemaphoreGive(g_publish_semaphore); // run publish_thread
        } // end (AUDIO_STOP)
        else if((getTrueCheckVal()==t_AUS_MAN_RESEND)||(getTrueCheckVal()==t_AUS_CWD_RESEND)){
            publishAuscultation();
        } // end (AUDIO_RESEND)
#else
        xSemaphoreGive(g_publish_semaphore); // run publish_thread
#endif
        vTaskDelay (1);
    } // end while(1);
    FSP_PARAMETER_NOT_USED (status);
    FSP_PARAMETER_NOT_USED (pvParameters);
} // end dataprocess_thread_entry()

#else
void dataprocess_thread_entry(void *pvParameters){
    volatile fsp_err_t status=FSP_ERR_ASSERTION;
    while (1)
    {
        vTaskDelay (1);
    }
    FSP_PARAMETER_NOT_USED (status);
    FSP_PARAMETER_NOT_USED (pvParameters);

} // end subscribe_thread_entry()
#endif
