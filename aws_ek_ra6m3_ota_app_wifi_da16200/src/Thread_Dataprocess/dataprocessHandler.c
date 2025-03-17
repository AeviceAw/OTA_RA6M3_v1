/*
 * dataprocessHandler.c
 *
 *  Created on: 7 Aug 2024
 *      Author: yyang
 */
#include "Initialisations.h"
#include <Algorithm/DSP/ADPCM_Algo.h>

/* Variable for AUS Countdown Timer */
#define AUS_TIMER_SEC 35
aus_timer_state_t g_aus_timer_state = AUS_TIMER_INIT;
uint32_t g_aus_timer_count=0;

/* Variables for CWD Timer */
cwd_state_t g_cwd_state = CWD_INIT;
uint8_t g_cwd_count =0;

/* Variables for VPT */
uint32_t g_vpt_sec =0;
//const uint32_t g_vpt_threshold[NUM_VPT_STATES]={3,8,10,12,13,15};
const uint32_t g_vpt_threshold[NUM_VPT_STATES]={1,5,7,9,12,15};
vpt_thres_t g_vpt_state = 0; // to access g_vpt_threshold[g_vpt_state]
vpt_status_t g_vpt = VPT_OFF; // to track the

/* Variables for dataprocess_state_t */
dataprocess_state_t g_dataprocess_state = AUDIO_INIT;

// ===============================================
// Variables to Schedule Publishing of HR/RR/WZ
// ===============================================
uint8_t Pub_Idx ;//= 0; // index of Buffer for publishing
uint8_t Buf_Idx ;//= 0;// HR/RR/Wz Publish-Buffer Index (Total number Buffer can store)
float Buf_hr[BUF_LEN] ;//={0.0f};
float Buf_rr[BUF_LEN] ;//={0.0f};
float Buf_wz[BUF_LEN] ;//={0.0f};
float Buf_batt[BUF_LEN] ;//={0.0f};
float Buf_bp[BUF_LEN] ;//={0.0f};

/* Variables for auscultation data */
char encoded[DATA_LEN] = {0}; // ADDED from mqtt_publish
extern float32_t fdata[DATASIZE];
uint32_t pub_too_slow = 0;
/*******************************************************************************************************************//**
 *  @brief       Functions for pushing values into buffer for publishing hr/rr/wz at interval of 15-sec
 *  @param       none   float        HR_DISPLAY. RR_DISPLAY, WZ_DISPLAY, BP_DISPLAY, BATT_DISPALY
 *  @param       none   vpt_status_t <<VPT_ON>> or <<VPT_OFF>>
 *  @retval      none
 **********************************************************************************************************************/
void vpt_buf_write(void){
    fsp_err_t status=FSP_ERR_ASSERTION;

//    /* (A) Reset vpt Buf Ptr if overflow */
//    if(Buf_Idx>=BUF_LEN) {
//        Buf_Idx = 0;
//    } // end if(Buf_Idx>=BUF_LEN

    /* (A) Check if Buffer is Empty */
    if (-0.1f<Buf_hr[Buf_Idx]&&Buf_hr[Buf_Idx]<0.1f){
        /* (B1) write the hr/rr/wz values to vpt buffer */
        Buf_hr[Buf_Idx] = vitals_get(HR_DISPLAY);//hr_display;
        Buf_rr[Buf_Idx] = vitals_get(RR_DISPLAY);//rr_display;
        Buf_wz[Buf_Idx] = vitals_get(WZ_DISPLAY);//wz_display;
        Buf_batt[Buf_Idx] = batt_get();
        Buf_bp[Buf_Idx] = batt_percentage_get();

        /* (B2) Increase the Ptr Value */
        Buf_Idx=(uint8_t)((Buf_Idx+1)%BUF_LEN);//++;
    } // end if (Buf_hr[Buf_idx] is empty)
    else{
        pub_too_slow++;
    }

    /* (C) Open & start the VPT timer */
    if (vpt_state_get()==VPT_OFF){
        status=vpt_timer_setup(VPT_OPEN);
        status=vpt_timer_setup(VPT_START);
    } // end if (!VPT_ON)

    FSP_PARAMETER_NOT_USED(status);
}// end vpt_buf_write();

/*******************************************************************************************************************//**
 *  @brief       Check if vital_publish_timer count is above threshold
 *  @param[in]   uint32_t tick_sec,    i.e.,  Current Elasped Timer sec count
 *  @param[in]   uint8_t  tick_thres,  i.e.,  g_vpt_threshold[g_vpt_state]
 *  @retval      bool     true or false
 **********************************************************************************************************************/
bool vph_cross_threshold(uint32_t tick_sec,uint32_t tick_thres) {
    if(tick_sec >= tick_thres)
        return true;
    else
        return false;
} // end VPH_Cross_Threshold()

/*******************************************************************************************************************//**
 *  @brief       Get the current Vital Publish Timer's Threshold State
 *  @retval      vpt_thres_t_t   0: 01-sec, 1: 03-sec, 2: 05-sec, 3: 07-sec, 4: 11-sec, 5: 15-sec
 **********************************************************************************************************************/
vpt_thres_t vph_thresState_get(void) {
    return g_vpt_state;
} // end vph_thresState_get()

/*******************************************************************************************************************//**
 *  @brief       Check and Increase <<g_vpt_state>> if allowed
 *  @retval      bool_t   true: OK_SUCCESSFUL, false: NOT_SUCCESSFUL
 **********************************************************************************************************************/
bool vph_thresState_add(void) {
    if (g_vpt_state<NUM_VPT_STATES-1){
        g_vpt_state++;
        return true;
    } // end if (CAN ADD)
    else{
        return false;
    } // end if (CANNOT ADD)
} // end vph_thresState_get()

/*******************************************************************************************************************//**
 *  @brief       set <<g_vpt_state>> to 0
 *  @retval      N.A
 **********************************************************************************************************************/
void vph_thresState_clear(void) {
    g_vpt_state = 0;
} // end vph_thresState_clear()


// TODO: YY Double check sometimes publishing '0'
/*******************************************************************************************************************//**
 *  @brief       Give Publish Semaphore if <<g_vpt_sec>> le than <<State_Threshold>>
 *  @retval      N.A
 **********************************************************************************************************************/
uint32_t pub_too_fast=0;
void g_vitals_pub_timer_callback(timer_callback_args_t *p_args){
    volatile uint32_t thres = (volatile uint32_t)  g_vpt_threshold[vph_thresState_get()];

    /* Reset Pub_Idx */
    if(Pub_Idx >= BUF_LEN)
        Pub_Idx = 0;

    /* Increase g_vitals_pub_timer_callback counter  */
    if (g_vpt_sec<15)
        g_vpt_sec++;

    /* Check and actuate State: 1,3,5,7,11,15 sec */
    //if (Buf_hr[Pub_Idx] != 0.0f){
    if (fabs(Buf_hr[Pub_Idx])>0.000001f){
        if(vph_cross_threshold(g_vpt_sec,thres)){
            /* (A) POP Buffer Ptr Value in the <<PUBLISH variables>> for Publishing*/
            vitals_set(HR_PUBLISH,Buf_hr[Pub_Idx]);//hr_publish=Buf_hr[Pub_Idx];
            vitals_set(RR_PUBLISH,Buf_rr[Pub_Idx]);////rr_publish=Buf_rr[Pub_Idx];
            vitals_set(WZ_PUBLISH,Buf_wz[Pub_Idx]);////wz_publish=Buf_wz[Pub_Idx];
            batt_publish_set(Buf_batt[Pub_Idx]);//batt_publish=Buf_batt[Pub_Idx];
            bp_publish_set(Buf_bp[Pub_Idx]);//bp_publish=Buf_bp[Pub_Idx];

            /* (B) clear the current buffer value */
            Buf_hr[Pub_Idx] = 0.0f;
            Buf_rr[Pub_Idx] = 0.0f;
            Buf_wz[Pub_Idx] = 0.0f;
            Buf_batt[Pub_Idx] = 0.0f;
            Buf_bp[Pub_Idx] = 0.0f;

            /* (C) Increase buffer ptr index */
            Pub_Idx=(uint8_t)((Pub_Idx+1)%BUF_LEN);//++;

            /* (D) Check if we still have values in Buffer  */
            if (-0.1f<(Buf_hr[(Pub_Idx+1)%12])&&(Buf_hr[(Pub_Idx+1)%12]<+0.1f)){
                BLEC_state_set(BLEC_INIT);
                pub_too_fast++;
            }

            /* (E) Check if Wifi is still available and give Publish Semaphore */
            if (internet_state_get(G_MQTT)){
                //xSemaphoreGive(g_publish_semaphore);
                BaseType_t xHigherPriorityTaskWoken = pdFALSE;

                // Give the semaphore
                xSemaphoreGiveFromISR(g_publish_semaphore, &xHigherPriorityTaskWoken);

                // Request a context switch if a higher-priority task was woken
                portYIELD_FROM_ISR(xHigherPriorityTaskWoken);
            }
            else{
                vpt_timer_off();
            }

            /* (F) Update VPT State */
            vph_thresState_add();   // Increase state;
            ////APP_PRINT("(4) g_vpt_sec = %d\n",g_vpt_sec);
            g_vpt_sec = 0; // Reset and count to 15-sec
        } // end if (counter>= Threshold)
    } // end if (Vitals to publish)

    /* Error Handling: in-case Counter Overflow */
    if (g_vpt_sec>=15)
        g_vpt_sec = 0; // Reset and count to 15-sec
    FSP_PARAMETER_NOT_USED(p_args);
} // g_vitals_pub_timer_callback()

/******************************************************************************************************************//**
 *  @brief       Vital Publish Timer Controller (VPT_START/VPT_STOP: Updates g_variable <<g_vpt>>)
 *  @retval      fsp_err_t Expect FSP_SUCCESS
 **********************************************************************************************************************/
fsp_err_t vpt_timer_setup(vpt_setup_t opts){
    volatile fsp_err_t status=FSP_ERR_ASSERTION;
    switch (opts){
        case (VPT_OPEN):{
            status = R_GPT_Open(&g_vitals_pub_timer_ctrl, &g_vitals_pub_timer_cfg);
            break;
        } // end VPT_OPEN()
        case (VPT_CLOSE):{
            status = R_GPT_Close(&g_vitals_pub_timer_ctrl);
            break;
        } // end VPT_CLOSE()
        case (VPT_START):{
            status = R_GPT_Start(&g_vitals_pub_timer_ctrl);
            g_vpt = VPT_ON; // Update Global Flag
            break;
        } // end VPT_START()
        case (VPT_STOP):{
            status = R_GPT_Stop(&g_vitals_pub_timer_ctrl);
            g_vpt = VPT_OFF; // Update Global Flag
            break;
        } // end VPT_STOP()
        case (VPT_RESET):{
            status = R_GPT_Reset(&g_vitals_pub_timer_ctrl);
            break;
        } // end VPT_RESET()
        case (VPT_ENABLE):{
            status = R_GPT_Enable(&g_vitals_pub_timer_ctrl);
            break;
        } // end VPT_ENABLE()
        case (VPT_DISABLE):{
            status = R_GPT_Disable(&g_vitals_pub_timer_ctrl);
            break;
        } // end VPT_DISABLE()
    } // end (Options)
    return status;
}// end vpt_timer_setup()

/**********************************************************************************************************************
 * @brief  Turn off VPT
 * @retval fsp_err_t status
 **********************************************************************************************************************/
fsp_err_t vpt_timer_off(void){
    volatile fsp_err_t status = FSP_SUCCESS;
    if (vpt_state_get()==VPT_ON){
        status|=vpt_timer_setup(VPT_STOP);
        status|=vpt_timer_setup(VPT_CLOSE);
    } // end if (VPT_ON)
    return status;
} // turnOffVpt()

/******************************************************************************************************************//**
 *  @brief       get the value of the <<g_vpt>>
 *  @retval      vpt_status_t <<VPT_ON>> or <<VPT_OFF>>
 **********************************************************************************************************************/
vpt_status_t vpt_state_get(void){
    return g_vpt;
} // vpt_state_get()

/******************************************************************************************************************//**
 *  @brief       get the value of the <<g_dataprocess_state>>
 *  @retval      dataprocess_state_t Init/SET0/SET1/SET2/SET3/Done/Err
 **********************************************************************************************************************/
dataprocess_state_t dataprocess_state_get(void){
    return g_dataprocess_state;
}// end dataprocess_Get_State()

/******************************************************************************************************************//**
 *  @brief       set the value of the <<g_dataprocess_state>>
 *  @param       dataprocess_state_t Init/SET0/SET1/SET2/SET3/Done/Err
 *  @retval      N.A
 **********************************************************************************************************************/
void dataprocess_state_set(dataprocess_state_t in_dataprocess_state) {
    g_dataprocess_state = in_dataprocess_state;
}// end dataprocess_state_set()

/******************************************************************************************************************//**
 *  @brief       update the value of the <<g_dataprocess_state>>
 *  @param       dataprocess_state_t Init/SET0/SET1/SET2/SET3/Done/Err
 *  @retval      N.A
 **********************************************************************************************************************/
#define FOUR_SEC 4000 // thread sleep 4-sec
void dataprocess_state_update(blec_state_t in_blec_State, trueCheck_t in_true_check, dataprocess_state_t in_dataprocess_state, vpt_status_t in_vpt_state,cwd_state_t inCwdState){
    if ((in_true_check==t_AUS_MAN_RESEND)||(in_true_check==t_AUS_CWD_RESEND)){
        dataprocess_state_set(AUDIO_SET3);
        ////APP_PRINT("Data_Thread->AUS_RESEND_MODE: Retain AUDIO_SET3\n");//,*AudioResendSelector_ptr);
    }// end if (AUS_RESEND)
    else if (in_true_check!=t_AUS_NONE && in_blec_State==BLEC_ALL_ON){
        switch (in_dataprocess_state){
            case AUDIO_INIT:
                if(in_vpt_state==VPT_ON){
                    vpt_timer_setup(VPT_STOP);
                } // Turn-Off VPT
                dataprocess_state_set(AUDIO_SET0);
                aus_timer_state_set(AUS_TIMER_ON);
                //tx_thread_sleep(FOUR_SEC);
                break;
            case AUDIO_ERR:
                if (in_true_check!=t_AUS_NONE){
                    TickType_t xLastWakeTime = xTaskGetTickCount();
                    vTaskDelayUntil(&xLastWakeTime, FOUR_SEC); // sleep 4000ms
                } // end if AUDIO_ERR and TRUE_CHECK
                else{
                    dataprocess_state_set(AUDIO_INIT);
                }  // end if AUDIO_ERR and TRUE_CHECK=0
                break;
            case AUDIO_SET0:
                dataprocess_state_set(AUDIO_SET1);
                //tx_thread_sleep(FOUR_SEC);
                break;
            case AUDIO_SET1:
                dataprocess_state_set(AUDIO_SET2);
                break;
            case AUDIO_SET2:
                dataprocess_state_set(AUDIO_SET3);
                break;
            case AUDIO_SET3:
                dataprocess_state_set(AUDIO_DONE);
                break;
            default:
                dataprocess_state_set(AUDIO_INIT);
                setTrueCheckVal(t_AUS_NONE);
                aus_timer_state_set(AUS_TIMER_OFF);
                break;
        }// updateDateprocessState()
    } // end if (AUS_MAN or AUS_CWD) && (BLEC_ALL_ON)
    else{
        dataprocess_state_set(AUDIO_INIT);
        if ((in_blec_State==BLEC_VITALS) && (inCwdState==CWD_INIT)){
            cwd_state_set(CWD_ON);
        } // end IF(CWD_ON)
//        if(in_vpt_state==VPT_OFF){
//            vpt_timer_setup(VPT_START); // VPT_MODE: Start VPT
//            ////APP_PRINT("(4) g_vpt = %d\n",vpt_state_get());
//        } // end of AUS_NONE
    } // end if (AUS_NONED) || (!BLEC_ALL_ON)
}// end dataprocess_state_update()

/******************************************************************************************************************//**
 *  @brief       Get the address location of   <<(char)  encoded[20480]>>
 *  @param       none    <<(char)  encoded[20480]>>
 *  @retval      none    <<(char*)  &encoded[0]>>
 **********************************************************************************************************************/
char* getEncodedAddr(void){
    return encoded;//return &encoded[0];
} // end getEncodedAddr()


/******************************************************************************************************************//**
 *  @brief       Perform ADPCM encode and decode
 *               put <g_publish_semaphore> if its <<!t_AUS_NONE>>
 *  @param       none    <<(float)  fdata[20480]>> // Global Variable
 *  @retval      none    <<(float)  decode(encode(fdata[20480]))>>,   // Global Variables
 *                       <<(char)          encode(fdata[20480]) >>,
 *                       <<g_publish_semaphore>>
 **********************************************************************************************************************/
//#define DOENCODING_INIT
void adpcm_encode_decode(void){

    #ifndef DOENCODING_INIT
        memset(encoded,0,sizeof(encoded));
        for (uint32_t idx = 0; idx<DATA_LEN;idx++) {
            encoded[idx] = (char) ADPCMEncoder((int16_t)(fdata[idx]*1000.0f));
        }
    #else
        doEncoding(fdata, encoded,DATA);
    #endif

        // Normal Auscultation Publishing (wout cWD)
        if ( (dataprocess_state_get()>=AUDIO_SET1) && (BLE_check()) && (getTrueCheckVal()!=t_AUS_NONE) ) {//if ((publish_audio == 1))//if ((publish_audio == 1)&&(ble==1))
            publishAuscultation();
        } // end PublishEncoded()

    #ifndef DOENCODING_INIT
        for (uint32_t idx = 0; idx<DATA_LEN;idx++) {
            fdata[idx] = ((float32_t) ADPCMDecoder((uint8_t)(encoded[idx])))/1000.0f;
        }
    #else
        doDecoding(encoded,fdata, DATA);
    #endif
    } // end adpcm_encode_decode()

/******************************************************************************************************************//**
 *  @brief       Function to put publish semaphore to publish encoding:
 *               We can add additional logic in future, e.g., audio resend
 *  @param       None
 *  @retval      None
 **********************************************************************************************************************/
void publishAuscultation (void){
    xSemaphoreGive(g_publish_semaphore); // run publish_thread
} // end publishAuscultation()


/******************************************************************************************************************//**
 *  @brief       Function to check if next iteration needs to perform CWD auscultation
 *               1st check if current iteration is doing anything (e.g., AUS_MAN?)
 *               2nd check if current iteration WZ_DISPLAY=1
 *  @param       None   g_dataprocess_state, g_blec_state, g_ble, g_imuOnbodyWZ_DISPLAY,
 *  @retval      None   t_AUS_CWD/t_AUS_NONE
 *  @Remark      Only call this function when dataprocess_State == <<AUDIO_INIT>>
 **********************************************************************************************************************/
bool inspectCwdNextIterStartCondition(dataprocess_state_t in_dataprocess_state,cwd_state_t inCwdState,blec_state_t inBlecState, uint8_t inBle, float inHR,float inWZ){
    if ((in_dataprocess_state==AUDIO_INIT) && (inCwdState==CWD_ON)){
        if ((inBle==1) && (inBlecState == BLEC_VITALS)){
            if (inHR > 0){
                if (inWZ>=0.5f){
                    setTrueCheckVal(t_AUS_CWD);
                    return true; // Only true when ALL conditions met
                } // end if (inWZ>0.5f)
            } // end if (inHR > 0)
        } // end if (inBle && BLEC_VITALS)
    } // end if (AUDIO_INIT && CWD_ON)
    return false;
} // inspectCwdNextIterStartCondition()


/******************************************************************************************************************//**
 *  @brief       Function to check if next iteration needs to continue perform CWD auscultation
 *               1st check if current iteration is doing anything (e.g., AUS_MAN?)
 *               2nd check if current iteration WZ_DISPLAY=1
 *  @param       None   g_dataprocess_state, g_blec_state, g_ble, g_imuOnbodyWZ_DISPLAY,
 *  @retval      None   t_AUS_CWD/t_AUS_NONE
 *  @Remark      Only call this function when dataprocess_State == <<AUDIO_INIT>>
 **********************************************************************************************************************/
bool inspectCwdContinueCondition(dataprocess_state_t in_dataprocess_state, cwd_state_t inCwdState, blec_state_t inBlecState, uint8_t inBle, float inHR,float inWZ){
    if ((in_dataprocess_state==AUDIO_SET1) && (inCwdState==CWD_ON)){
        if (inBle==1 && inBlecState == BLEC_ALL_ON){
            if ((inHR > 0) &&  (inWZ>=0.5f)) {
                // increase WZ counter
                g_cwd_count++;
                return true; // Only true when ALL conditions met
            } // end if (inHR > 0 && inWZ>0.5f)
            else{
                setTrueCheckVal(t_AUS_STOP); // Stop Prematurely - Otherwise
                return false;
            } // end if Not (2 consecutive WZ)
        } // end if (inBle && BLEC_VITALS)
        else{
            setTrueCheckVal(t_AUS_STOP); // Stop Prematurely - Otherwise
            return false;
        } // end if Not (BLE Connected)
    } // end if (AUDIO_SET1 && CWD_ON)
    return false;
} // inspectCwdContOrStopCondition(

/******************************************************************************************************************//**
 *  @brief       Function to check if we need to turn off CWD auscultation (2 cwd / 10 mins)
 *               If its the 1st CWD, increase count
 *               If its the 2nd CWD, increase count, and turn-off CWD for 10-min
 *  @param       None   g_cwd_count, cwd_timer_on
 *  @retval      None   g_cwd_count++ and/or turn-off and start cwd_timer
 **********************************************************************************************************************/
bool inspectCwdTurnOffCondition(uint8_t in_cwdCount){
    if (in_cwdCount>=MAX_CWD_COUNT){
        cwd_state_set(CWD_OFF);
        return true;
    } // end if (g_cwd_count>)
    else{
        return false;
    } // end if (g_cwd_count<)
} // end inspectCwdTurnOffCondition()

/**********************************************************************************************************************
 * @brief       Get the g_cwd_count value
 * @param       None
 * @retval      (uint8_t) g_cwd_count
 **********************************************************************************************************************/
uint8_t cwd_count_get(void){
    return g_cwd_count;
}// end cwd_count_get()

/**********************************************************************************************************************
 * @brief       Get the status of the cwd  - on/off/init
 * @param       None
 * @retval      (cwd_state_t) g_cwd_state = CWD_ON/CWD_OFF/CWD_INIT
 **********************************************************************************************************************/
cwd_state_t cwd_state_get(void){
    return g_cwd_state;
}// end cwd_state_get()

/**********************************************************************************************************************
 * @brief       set the status of the cwd  - on/off/init
 * @param       (cwd_state_t) inState = CWD_ON/CWD_OFF/CWD_INIT
 * @retval      g_cwd_state
 **********************************************************************************************************************/
void cwd_state_set(cwd_state_t inState){
    volatile fsp_err_t status=FSP_ERR_ASSERTION;
    g_cwd_state = inState;
    switch (inState){
        case CWD_INIT:
            break;
        case CWD_ON:
            g_cwd_count = 0; // Reset count
            status = R_GPT_Stop(&g_cwd_timer_ctrl);
            status = R_GPT_Close(&g_cwd_timer_ctrl);
            break;
        case CWD_OFF:
            /* Start CWD Off Timer for 10-min */
            status = R_GPT_Open(&g_cwd_timer_ctrl, &g_cwd_timer_cfg);
            status = R_GPT_Start(&g_cwd_timer_ctrl);
            break;
    } // end switch (inState)
    FSP_PARAMETER_NOT_USED(status);
}// end cwd_state_set()


/*******************************************************************************************************************//**
 *  @brief       Restart CWD when Timer has elasped
 *  @retval      N.A
 **********************************************************************************************************************/
uint16_t g_cwd_min = 0;
void g_cwd_timer_callback(timer_callback_args_t *p_args){
    static uint16_t cwd_callback_count = 0;
    g_cwd_min++;
    // 10-min has elasped! Turn-On CWD
    if (g_cwd_min%10==0){
        cwd_state_set(CWD_ON);
        cwd_callback_count++;
    } // end if s
    FSP_PARAMETER_NOT_USED(p_args);
}// end g_cwd_timer_callback()


/*******************************************************************************************************************//**
 *  @brief       g_aus_timer callback
 *  @param[in]   global uint32_t g_aus_timer_count
 *  @param[in]   define AUS_TIMER_SEC 35 sec
 *  @Remark      When >g_aus_timer_count Reset State to <<AUDIO_INIT>> and <<t_AUS_NONE>>
 **********************************************************************************************************************/
void g_aus_timer_callback(timer_callback_args_t *p_args) {
    FSP_PARAMETER_NOT_USED(p_args);
    g_aus_timer_count++;
    if (AUS_TIMER_SEC<g_aus_timer_count){
        dataprocess_state_set(AUDIO_INIT);
        setTrueCheckVal(t_AUS_NONE);
        aus_timer_state_set(AUS_TIMER_OFF);
    } // end if(>AUS_TIMER_SEC)
} // end g_aus_timer_callback()

/**********************************************************************************************************************
 * @brief       set the status of the aus countdown timer  - on/off/init
 * @param       (aus_timer_state_t) inState = AUS_TIMER_ON/AUS_TIMER_OFF/AUS_TIMER_INIT
 * @retval      none
 **********************************************************************************************************************/
void aus_timer_state_set(aus_timer_state_t inState){
    volatile fsp_err_t status=FSP_ERR_ASSERTION;
    g_aus_timer_state = inState;
    switch (inState){
        case AUS_TIMER_INIT:
            break;
        case AUS_TIMER_OFF:
            g_cwd_count = 0; // Reset count
            status = R_GPT_Stop(&g_aus_timer_ctrl);
            status = R_GPT_Close(&g_aus_timer_ctrl);
            break;
        case AUS_TIMER_ON:
            /* Start CWD Off Timer for 10-min */
            status = R_GPT_Open(&g_aus_timer_ctrl, &g_cwd_timer_cfg);
            status = R_GPT_Start(&g_aus_timer_ctrl);
            g_aus_timer_count = 0;
            break;
    } // end switch (inState)
    FSP_PARAMETER_NOT_USED(status);
}// end aus_timer_state_set()


/**********************************************************************************************************************
 * @brief       Get the status of the cwd  - on/off/init
 * @param       None
 * @retval      (cwd_state_t) g_cwd_state = CWD_ON/CWD_OFF/CWD_INIT
 **********************************************************************************************************************/
aus_timer_state_t aus_timer_state_get(void){
    return g_aus_timer_state;
}// end cwd_state_get()
