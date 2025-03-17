/*
 * dataHandler.c
 *
 *  Created on: 13 Aug 2024
 *      Author: yyang
 */

#include "initialisations.h"
#include "Algorithm\DSP\ADPCM_Algo.h"
#include "Algorithm\DSP\ADPCM_Algo.h"
#include "Algorithm\DSP\frequency_test.h"
#include "Algorithm\DSP\offline_validate.h"
#include <Algorithm\DSP\KMDC\getTestData.h>
#include <Algorithm\DSP\Vitals\hr_algo.h>
#include <Algorithm\DSP\offline_validate.h>
#include <Algorithm\DSP\Vitals\rr_algo.h>

uint32_t DATA_SUCCESS_COUNT = 0;


// ===============================================
// Variables For Testing KMDC()
// ===============================================
float d_err_sum=0.0f; // For Testing KMDC()
extern float d_results[3];
extern const float d_results_expected[3];

// ===============================================
// Variables For Transferring from Uart to Dataprocess
// ===============================================
uint8_t com_data[UART_DATA_SIZE] = {0};

// ===============================================
// Variables For Storing 5.12-sec Sensor Recording
// ===============================================
float32_t fdata[DATASIZE]={0.0f};

/******************************************************************************************************************//**
 *  @brief       Get the address location of   <<(uint8_t)  com_data[61440]>>
 *  @param       none     <<(uint8_t)  com_data[61440]>>
 *  @retval      (uint8_t*)  com_data[61440]
 **********************************************************************************************************************/
uint8_t* getComDataAddr(void){
    return (uint8_t*) com_data;
} // end getComDataAddr()


/******************************************************************************************************************//**
 *  @brief       Test the KMDC algorithm with a pre-known fWheeze
 *  @param       None   (float)   fdata[20480]
 *  @param       None   (float)   fWheeze[20480]
 *  @retval      float  d_err_sum   should as close to zero as possible
 **********************************************************************************************************************/
float testKMDC(void){
    // Test with Data from getTestData.c
    memset(fdata,0,20480*sizeof(float));
    memcpy(fdata,(void*)fWheeze,(size_t)(20480*sizeof(float)));
    vitals_set(g_WZ,getKMDC());//g_wz = getKMDC(); // Perform KMDC computation and get wz results
    d_err_sum = d_diff(d_results,d_results_expected,d_results_err,3);
    return d_err_sum;
} // end testKMDC()

/******************************************************************************************************************//**
 *  @brief       Compute HR/RR and Store into <<HR_DISPLAY>> AND <<RR_DISPLAY>> <<fdata>>
 *  @param       none    <<(float)  fdata[20480]>> // Global Variable
 *  @retval      none    <<(float)    HR/RR/HR_DISPLAY/RR_DISPLAY>>,   // Global Variables
 **********************************************************************************************************************/
/* Global Variables for Vitals Value */
float32_t g_hr = -2.0f; // Fixme: Change back to -2.0f after Algorithm logic is available;
float32_t g_rr = -2.0f; // Fixme: Change back to -2.0f after Algorithm logic is available;
float32_t g_wz = -2.0f;  // Fixme: Change back to -2.0f after Algorithm logic is available;
float32_t hr_display = -2.0f;
float32_t rr_display = -2.0f;
float32_t wz_display = -2.0f;
float32_t hr_publish = -2.0f;
float32_t rr_publish = -2.0f;
float32_t wz_publish = -2.0f;
void vitals_compute(void){

    //g_tic();
    // ---------------------------------------------------------------------------------------------------------------------
    // Test with C001_WZ_001 Dr. Sunit's Annotation
    //memcpy(fdata,(void*)fdata1,(size_t)(DATA*sizeof(float)));

    /* Calculate HR at every 5 second and output the latest HR with the consideration of past value */
    display_HR();
    //g_toc();

    //g_tic();
    /* Concatenate the RR_entropy for a consecutive of 5 second*/
    RR_entropy_15();

    /* Calculate HR at every 5 second and output the latest RR */
    display_RR();
    //g_toc();

    //tx_thread_sleep(1);
} // vitals_compute()


/*********************************************************************************************************************
 *  @brief get the global vital variable <<HR/RR/WZ_DISPLAY>> or <<HR/RR/WZ_PUBLISH>> or <<G_HR/G_RR/G_WZ>>
 *  @param[in] vital_t [HR/RR/WZ;_Display;_Publish]
 *  @retval    float32_t Value
 *********************************************************************************************************************/
float32_t vitals_get(vital_t inType){
    switch (inType){
        case (g_HR):{
            return g_hr;
            break;
        }   // (HR)
        case (g_RR):{
            return g_rr;
            break;
        }   // (RR)
        case (g_WZ):{
            return g_wz;
            break;
        }   // (WZ)
        case (HR_DISPLAY):{
            return hr_display;
            break;
        }   // (HR_DISPLAY)
        case (RR_DISPLAY):{
            return rr_display;
            break;
        }   // (RR_DISPLAY)
        case (WZ_DISPLAY):{
            return wz_display;
            break;
        }   // (WZ_DISPLAY)
        case (HR_PUBLISH):{
            return hr_publish;
            break;
        }   // (HR_PUBLISH)
        case (RR_PUBLISH):{
            return rr_publish;
            break;
        }   // (RR_PUBLISH)
        case (WZ_PUBLISH):{
            return wz_publish;
            break;
        }   // (WZ_PUBLISH)
        default:{
            return -2.0f; // Error Variable dun exist
        }
    } // end (type)
} // vitals_get()


/*********************************************************************************************************************
 *  @brief set the global vital variable <<HR/RR/WZ_DISPLAY>> or <<HR/RR/WZ_PUBLISH>> or <<G_HR/G_RR/G_WZ>>
 *  @param[in] vital_t      [HR/RR/WZ;_Display;_Publish]
 *  @param[in] float32_t    Value of the global variable
 *  @retval    void
 *********************************************************************************************************************/
void vitals_set(vital_t inType,float32_t inVal){
    switch (inType){
        case (g_HR):{
            g_hr = inVal;
            break;
        }   // (HR)
        case (g_RR):{
            g_rr = inVal;
            break;
        }   // (RR)
        case (g_WZ):{
            g_wz = inVal;
            break;
        }   // (WZ)
        case (HR_DISPLAY):{
            hr_display = inVal;
            break;
        }   // (HR_DISPLAY)
        case (RR_DISPLAY):{
            rr_display = inVal;
            break;
        }   // (RR_DISPLAY)
        case (WZ_DISPLAY):{
            wz_display = inVal;
            break;
        }   // (WZ_DISPLAY)
        case (HR_PUBLISH):{
            hr_publish = inVal;
            break;
        }   // (HR_PUBLISH)
        case (RR_PUBLISH):{
            rr_publish = inVal;
            break;
        }   // (RR_PUBLISH)
        case (WZ_PUBLISH):{
            wz_publish = inVal;
            break;
        }   // (WZ_PUBLISH)
    } // end (type)
} // vitals_set()


/******************************************************************************************************************//**
 *  @brief       Extract <<fdata>> from <<com_data>> and perform moving averaging filtering
 *  @param       none    <<(uint8_t)  com_data[61440]>> // Global Variable
 *  @retval      none    <<(float)    fdata[20480]>>,   // Global Variables
 *                       <<(imu_status_t) g_imuOnbody>>, <<(float) batt>>, <<(float bp>>
 **********************************************************************************************************************/
void datapreprocess_compute(void){
    // ---------------------------------------------------------------------------------------------------------------------
    /* Process ADC data: Output fdata */
    getData();

    /* //Turn on this function and turn off getData to validate the HR measurements using offline data stored in flash memory */
    ////offline_Validate(); /* TODO: DO NOT UNCOMMENT FOR RLEASE VERSION */

    /* Removing impulsive noise */
    //stft_filt(fdata);

    // ---------------------------------------------------------------------------------------------------------------------
    /* Pre-Processing Denoise */
    getMed(fdata,13);

    //getDenoise(fdata,fdata);
    //g_toc();
    // ---------------------------------------------------------------------------------------------------------------------
}// end datapreprocess_compute()

/********************************************************************************
 *  @brief global Variable for getData()
 ********************************************************************************/
uint32_t count_dat=0 ;
float32_t ADCmean=0.0f;
uint32_t  buffer_index=0 ;
uint16_t data16[DATASIZE] = {0};

// =======================================================================
// Functions for Data Extractions from UART
// =======================================================================
/*
 * ********************************************************************************************************
 *  @brief Handler function to decode/extract the AeviceMD Senor's Packet data
 *         v1.0: 30-Nov-2022 Initial Release for MVP_v5.7.5_250pieces
 *         v1.1: 05-Jun-2023 Algorithm Revision as per Minutes "20230601 Algorithm -getData() Up Version (signed).pdf"
 *
 *  @param  (global)   uint8_t com_data[61440]; // define in uart_thread_entry.c
 *                    uint16_t data[20480] ;    // define in dataprocess_thread_entry.c
 *                    uint32_t count_dat;       // define in AlgoHandler.c
 *                     float   batt;            // define in battToBP.c
 *                     float   bp;              // define in battToBP.c
 *                      bool   movement;        // define in Imu_Handler.c
 *                   float32_t ADCmean;         // define in AlgoHandler.c
 *                   float32_t fdata[20480]     // define in dataprocess_thead_entry.c
 *
 *  @return (global) float32_t fdata[20480];    // define in dataprocess_thead_entry.c
 *                     float   batt;            // define in battToBP.c
 *                     float   bp;              // define in battToBP.c
 *                      bool   movement;        // define in Imu_Handler.c
 *
 *  @example getData();
 *
 *  @notes: Each 16-bit sensor’s analog-to-digital (ADC) sample is arranged in a packet of 3 bytes, i.e.,
 *
 *              [<<uint8_t header>>, <<uint8_t LSB>>, <<uint8_t MSB>>]
 *
 *              Dock receives 61440 bytes corresponding to 5.12 s of sensor data
 *
 *                   [0]        [1]         [2]     ...    [61437] ,  [61438]  , [61439]
 *              [H_00,000],[LSB_00000],[MSB_00000], ... ,[H_79,255],[LSB_20479],[MSB_20479],
 *
 *          Review Highlights
 *          v1.0 Is sufficient for the MVP 250 pieces
 *          v1.1 Is necessary  for the MMP series as it will improve the overall audio quality and UX of the remote auscultation
 *
 * ********************************************************************************************************
 */
void getData(void)
{
    uint8_t dataPrev = 0; // keep track from prev iter's datas[0] value
    uint8_t err = 0;     // keep track of the number of err detected
    uint8_t datas[4] = {0x00};
    count_dat = 0;                  // clear the count value

    //DEBUG_LOG(LOG_LEVEL_ERROR, "    com_data[] -> data[]");
    ////// 5sec_Recording + Battery_Level_Calculation
    for(volatile uint32_t num = 0; num < 61440-18; num++)
    {
        if (count_dat==256)
            count_dat=count_dat+err;

        if ((com_data[(num + 3)] == (com_data[num]+1))&&(com_data[(num + 6)] == (com_data[num + 3]+1))&&(com_data[(num + 9)] == (com_data[num + 6]+1))&&(com_data[(num + 12)] == (com_data[num + 9]+1))&&(com_data[(num + 15)] == (com_data[num + 12]+1))&&(com_data[(num + 18)] == (com_data[num + 15]+1)))
        {

            datas[0] = com_data[num];
            datas[1] = com_data[num+1];
            datas[2] = com_data[num+2];

            if ((((dataPrev+1)%256)!=datas[0]) && (count_dat>0) && (err<3)){
                err++;
                continue; // check datas[1] against dataPrev value
            } // end if (dataPrev+1 != datas[0])

            data16[count_dat] = (uint16_t) (((datas[2] & 0xFF) << 8 ) | ((datas[1] & 0xFF) << 0 ));
            count_dat++;
            dataPrev=datas[0]; //  update for next iter
            err=0;             // clear err for next iter

        }

        else if ((com_data[(num + 18)] == 0x00) && (com_data[(num + 15)] == 0xFF) && (com_data[(num + 12)] == 0xFE) && (com_data[(num + 9)] == 0xFD) && (com_data[(num + 6)] == 0xFC) && (com_data[(num)+3] == 0xFB) && (com_data[(num)] == 0xFA))
        {
            datas[0] = com_data[num];
            datas[1] = com_data[num+1];
            datas[2] = com_data[num+2];

            if ((((dataPrev+1)%256)!=datas[0]) && (count_dat>0) && (err<3)){
                err++;
                continue; // check datas[1] against dataPrev value
            } // end if (dataPrev+1 != datas[0])

            data16[count_dat] = (uint16_t) (((datas[2] & 0xFF) << 8 ) | ((datas[1] & 0xFF) << 0 ));
            count_dat++;
            dataPrev=datas[0]; //  update for next iter
            err=0;             // clear err for next iter
        }


        else if ((com_data[(num + 18)] == 0x01) && (com_data[(num + 15)] == 0x00) && (com_data[(num + 12)] == 0xFF) && (com_data[(num + 9)] == 0xFE) && (com_data[(num + 6)] == 0xFD) && (com_data[(num + 3)] == 0xFC) && (com_data[(num)] == 0xFB))
        {
            datas[0] = com_data[num];
            datas[1] = com_data[num+1];
            datas[2] = com_data[num+2];

            if ((((dataPrev+1)%256)!=datas[0]) && (count_dat>0) && (err<3)){
                err++;
                continue; // check datas[1] against dataPrev value
            } // end if (dataPrev+1 != datas[0])

            data16[count_dat] = (uint16_t) (((datas[2] & 0xFF) << 8 ) | ((datas[1] & 0xFF) << 0 ));
            count_dat++;
            dataPrev=datas[0]; //  update for next iter
            err=0;             // clear err for next iter
        }

        else if ((com_data[(num + 18)] == 0x02) && (com_data[(num + 15)] == 0x01) && (com_data[(num + 12)] == 0x00) && (com_data[(num + 9)] == 0xFF) && (com_data[(num + 6)] == 0xFE) && (com_data[(num + 3)] == 0xFD) && (com_data[(num)] == 0xFC))
        {
            datas[0] = com_data[num];
            datas[1] = com_data[num+1];
            datas[2] = com_data[num+2];

            if ((((dataPrev+1)%256)!=datas[0]) && (count_dat>0) && (err<3)){
                err++;
                continue; // check datas[1] against dataPrev value
            } // end if (dataPrev+1 != datas[0])

            data16[count_dat] = (uint16_t) (((datas[2] & 0xFF) << 8 ) | ((datas[1] & 0xFF) << 0 ));
            count_dat++;
            dataPrev=datas[0]; //  update for next iter
            err=0;             // clear err for next iter
        }

        else if ((com_data[(num + 18)] == 0x03) && (com_data[(num + 15)] == 0x02) && (com_data[(num + 12)] == 0x01) && (com_data[(num + 9)] == 0x00) && (com_data[(num + 6)] == 0xFF) && (com_data[(num + 3)] == 0xFE) && (com_data[(num)] == 0xFD))
        {
            datas[0] = com_data[num];
            datas[1] = com_data[num+1];
            datas[2] = com_data[num+2];

            if ((((dataPrev+1)%256)!=datas[0]) && (count_dat>0) && (err<3)){
                err++;
                continue; // check datas[1] against dataPrev value
            } // end if (dataPrev+1 != datas[0])

            data16[count_dat] = (uint16_t) (((datas[2] & 0xFF) << 8 ) | ((datas[1] & 0xFF) << 0 ));
            count_dat++;
            dataPrev=datas[0]; //  update for next iter
            err=0;             // clear err for next iter
        }

        else if ((com_data[(num + 18)] == 0x04) && (com_data[(num + 15)] == 0x03) && (com_data[(num + 12)] == 0x02) && (com_data[(num + 9)] == 0x01) && (com_data[(num + 6)] == 0x00) && (com_data[(num + 3)] == 0xFF) && (com_data[(num)] == 0xFE))
        {
            datas[0] = com_data[num];
            datas[1] = com_data[num+1];
            datas[2] = com_data[num+2];

            if ((((dataPrev+1)%256)!=datas[0]) && (count_dat>0) && (err<3)){
                err++;
                continue; // check datas[1] against dataPrev value
            } // end if (dataPrev+1 != datas[0])

            data16[count_dat] = (uint16_t) (((datas[2] & 0xFF) << 8 ) | ((datas[1] & 0xFF) << 0 ));
            count_dat++;
            dataPrev=datas[0]; //  update for next iter
            err=0;             // clear err for next iter
        }

        else if ((com_data[(num + 18)] == 0x05) && (com_data[(num + 15)] == 0x04) && (com_data[(num + 12)] == 0x03) && (com_data[(num + 9)] == 0x02) && (com_data[(num + 6)] == 0x01) && (com_data[(num + 3)] == 0x00) && (com_data[(num)] == 0xFF))
        {
            datas[0] = com_data[num];
            datas[1] = com_data[num+1];
            datas[2] = com_data[num+2];

            if ((((dataPrev+1)%256)!=datas[0]) && (count_dat>0) && (err<3)){
                err++;
                continue; // check datas[1] against dataPrev value
            } // end if (dataPrev+1 != datas[0])


            data16[count_dat] = (uint16_t) (((datas[2] & 0xFF) << 8 ) | ((datas[1] & 0xFF) << 0 ));
            count_dat++;
            dataPrev=datas[0]; //  update for next iter
            err=0;             // clear err for next iter
        }

#ifdef IMU
        else if ((com_data[(num)] == 0xFF) && (com_data[(num+1)] == 0x00) && (com_data[(num + 2)] != 0x00) && (com_data[(num + 3)] != 0x00) && (com_data[(num + 4)] != 0x00) && (com_data[(num + 5)] == 0x00)&&(com_data[(num + 6)] == 0xFF))
        {
            datas[1] = com_data[num+2];
            datas[2] = com_data[num+3];

            uint8_t ob_temp = 0;
            ob_temp = com_data[num+4];

#if !TEST_IMU_CMD
            if(imu_onbody_get() == IMU_ON_BODY) {
                imu_onbody_set(IMU_ON_BODY);
            }
            else{
                if (ob_temp == 7) {
                    imu_onbody_set(IMU_OFF_BODY);
                }
                else if (ob_temp == 8) {
                    imu_onbody_set(IMU_ON_BODY);
                }
            }
#endif

            uint16_t temp = 0;
            temp = (uint16_t) (((datas[2] & 0xFF) << 8 ) | ((datas[1] & 0xFF) << 0 ));
            float tempBatt = (temp*1.0f)/32768.0f * 2.0f * 4.088042f ;      // 100%  <- 4.088042 Volt
            batt_set(tempBatt);
            bp_get(&batt,&bp);
            bp_mu_get(&bp,&bp);

//#ifdef TEST
//            batt_update++;
//#endif
        }

#endif
#ifndef IMU
        else if ((com_data[(num)] == 0xFF) && (com_data[(num+1)] == 0x00) && (com_data[(num + 2)] != 0x00) && (com_data[(num + 3)] != 0x00) && (com_data[(num + 4)] == 0x00)&&(com_data[(num + 5)] == 0xFF))
        {
            datas[1] = com_data[num+2];
            datas[2] = com_data[num+3];

            uint16_t temp = 0;
            temp = (uint16_t) (((datas[2] & 0xFF) << 8 ) | ((datas[1] & 0xFF) << 0 ));

            batt = (temp*1.0f)/32768.0f * 2.0f * 4.088042f ;      // 100%  <- 4.088042 Volt

            if (batt > 4.17f)
                bp = 100.0f;
            else if (batt > 4.10f)
                bp = 100.0f-((4.17f-batt)/(4.17f-4.10f))*10.0f;
            else if (batt > 4.0f)
                bp = 90.0f-((4.1f-batt)/(4.1f-4.0f))*15.0f;
            else if (batt > 3.9f)
                bp = 75.0f-((4.0f-batt)/(4.0f-3.9f))*15.0f;
            else if (batt > 3.8f)
                bp = 60.0f-((3.9f-batt)/(3.9f-3.8f))*15.0f;
            else if (batt > 3.7f)
                bp = 45.0f-((3.8f-batt)/(3.8f-3.7f))*25.0f;
            else if (batt > 3.6f)
                bp = 20.0f-((3.7f-batt)/(3.7f-3.6f))*15.0f;
            else
                bp = 5.0f;


#ifdef TEST
            batt_update++;
#endif
        }
#endif
        else
        {
            //
        }

        if (count_dat>=DATASIZE){
            //count_dat=DATASIZE; // prevent count_dat going above DATASIZE
            break;
        }
    } //end for(uint32_t num = 0; num < UART_DATA_SIZE-18; num++)

    /* Track number of successful data extraction */
    DATA_SUCCESS_COUNT = count_dat;

    if (count_dat>=DATASIZE)
        count_dat=DATASIZE; // prevent count_dat going above DATASIZE

    ///// 5sec_Recording +  ADCMean
    ADCmean = 0.0f; // Clear the ADCmean value
    for(uint32_t num = 0; num <count_dat; num++)
        ADCmean = ADCmean + (((float32_t)data16[num])/((float32_t)count_dat));

    for(uint32_t num = 0; num <count_dat; num++)
        if(data16[num]>32768)
            data16[num] = (uint16_t)ADCmean;

    for(uint32_t idx = count_dat;idx<20480;idx++)
        data16[idx] = (uint16_t)ADCmean;

    for(uint32_t idx = 0;idx<20480;idx++)
        fdata[idx] = (((float32_t) data16[idx])-ADCmean)/1000.0f;

} //void getData(void)

/******************************************************************************************************************//**
 *  @brief       Get the value of   <<(uint32_t)  DATA_SUCCESS_COUNT>>
 *  @param       none     <<(uint32_t)  DATA_SUCCESS_COUNT>>
 *  @retval      (uint32_t)  DATA_SUCCESS_COUNT
 *  @remarks     The number of Successful Extraction from <<com_data>>, see getData()
 **********************************************************************************************************************/
uint32_t getDataSuccessCount(void){
    return DATA_SUCCESS_COUNT;
} // end getDataSuccessCount()


/*********************************************************************************************************************
 *  @brief Compute the odr_val moving average to smooth out the ADC data
 *  @param[in] float32_t*  &fdata
 *  @param[in] uint32_t_t  13
 *  @retval    void
 *********************************************************************************************************************/
void getMed(float32_t *fin, uint32_t odr_val)
{
    // (0) Average FilterPerform Moving Average of Order 5: 20475 = 20480-5-floor(odr/2)
    //float32_t eps = 0.000000000001f;
    float32_t fTemp = 0.0f;         // Temp Vairable to store fdata val
    uint32_t odr  = odr_val;        // Order of MA filter
    //float32_t frms[1] = {0.0};      // rms Value of fdata

    //arm_rms_f32 (fin,pT, frms);
    //uint32_t frmsIdx[1]=0;
    //arm_max_f32(fin,pT,&frms,&frmsIdx);

    //for (uint32_t tt = 0;tt<pT;tt++)
    //    fin[tt]=fin[tt]/(frms[0]+eps);

    for (uint32_t ii = (odr-(uint32_t) floor(odr/2)-1); ii<DATA_LEN-(uint32_t)floor(odr/2) ; ii++) // 8 order moving average filter
    {
        fTemp = 0.0f;
        for (uint32_t iii =ii-(uint32_t)floor(odr/2); iii< ii+(uint32_t)floor(odr/2)+1;iii++)
            fTemp=fTemp+(fin[iii])/((float32_t) odr*1.0f);
        fin[ii]= fTemp;
        if (ii>=19997)
            odr=odr_val;
    }
} // getMed()



