/*
 * publishHandler.c
 *
 *  Created on: 15 Jun 2024
 *      Author: yyang
 */

#include "initialisations.h"

/* SRAM Functions */
//#include "Aevice_SRAM/Aevice_SRAM_SPI_API.h" //for SRAM test

#define AUDIO_SRAM

/**********************************************************************************************************************
 * Some Publishing HR/RR/WZ Values
 ***********************************************************************************************************************/
//char p_dname[]="M815F070";//char p_dname[]="DA16200MOD_PUB";
//char p_ID[]="1258426693";
char p_ID[]="0123456789";
//char p_hr[]="58.9";
//char p_rr[]="12.4";
//char p_wz[]="0";
//float p_batt= 4.12f;//char p_batt[]="4.12";
//float p_bp=99.9f;//char p_bp[]="99.9";
char p_audio_val[3072] ="0F0F0F0F0F0C080008000808000808000800000008000000000000000800090909090909090000010008020201010000030B0B0B0C09090103030203030301090104040303030303090B0F0B0B0B0E0B0C0B0A0A0A090203030403020301010000000A0B0D0B0B0B0D090803040503060303010001010108080103000C0B0B0D0D0B0A0900000104020101000302010103040401000202010A0E0B0B0B0B0B0B0B09090B0001010303090B0B0C0A0B0D0B0A080A0A0A0A0A0C0C09090101060304020303040101030403020304040200090809090C0C0B0900090903060301080100000A0C0B0B0A090B0C010203030A0B0B0C0B0B0B0B010301090B0B090B0D0B08010304040403040302080B0C0B0C0B0B0A090101030503050305020200010809090A0B0C0A0B0C0B080800030301010307010102020401080A090909000B0A01010A0B0D0B0908090A0B0900010A0D0A090B0C0C0B090008010A030307040202040303080809000909090A0002010B0A0001000B0D0A0B0A090B0B090003020201090003030303070303030101090A0B0D0B0A0A090B0D0B0D0A0A0A0B0A0003030303040302090B0B0B0C0B0C0D0B0B0A0B0001030402020305030304050302000808090B0B0B0003030304020203010109010303020A0009090C0B0A090C0A09090009090A090B0B0B0B0B0E0B0B09090B00050501010003030201000205020008000305010000000303090B0A0909000903040100010101040001020A0D0A0901000203050309090A0D0B0B0B0A0B09030501010102090B0E0B090B0C0A0B0101000102040109010402030704040303040100090A0B0B0D0B0A080900090A0A0B0B090305050302030009090909000102020B0A09090B0B0D0B0A0B0D090B090808000B0E0B0A0B0C090104030405030200080A0A0B0C0A090808010103030503050202020302080A00010009000204000800000B090B000102000303010A0D0B0B0C0C0B0C0A0A09090900010202020102030301000B0B0A0C0C0C0C0A0B0C0A00020405030304020109090A09090A0800090109010307040303030102080B09090104040202090B0C0B0A0103070301000102010A0C0A0B0D0C0C0A090A0B0A0B0A0B0A0909090305050302020008090801000105010202020303020309000A0A09000100090B09090B0A0A0A02050303040303010B0A0B090B0F0B0B0B0B0B01030305020901010903070203000A0B0E0B090B0C0002010100010303030403010B0F0B090B0B09010909090B040009030401010B0003050304030403040208080003020B0D0A0001000B030704080B0B00030B0D0A0D0A080103060303000C0D0C0B09000102030402010009090B0B0A0B0A09090904020C0C090A01010A07040302000C0B0A0801010103050102090C080201090F0D0000080909";
//float wz12[12]={0.0f};
//char p_dname[8]={'\0'};
#define p_dname DNAME

/* Variable for Auscultation Publishing */
uint32_t publish_count=0;
uint32_t publish_audio=0;
char str_encoded[SIZE] = {0};  // for encoded audio
uint8_t p_seq = 0;		//Tracks the current sequence # published
uint8_t byteArray[BYTEARRAYSIZE] = {0};
uint32_t count_pub = 0;
//extern char encoded[DATA];
//char *gcvt(double number, int ndigit, char *buf);

/* Variables for SRAM */
#define SRAM_SEQ_SIZE 2048
uint8_t seq_CS[60][2] = {0};//0x00;

/**********************************************************************************************************************
 *  @brief      Sum all the elements inside array
 *  @param  	(void*) array, (int) size
 *  @retval     (int) sum(array)
 **********************************************************************************************************************/
int sumArray(char *array, int size) {
    int sum = 0;
    for (int idx = 0; idx < size; idx++)
        sum += array[idx];
    return sum;
} // end sumArray()

/**********************************************************************************************************************
 *  @brief      Clear the <<(uint8_t) p_seq>>, <<(float) wz12[12]>>, <<(uint32_t) publish_count>>
 *  @param  	None <<(uint8_t) p_seq>> = x
 *  @retval     None <<(uint8_t) p_seq>> = 0
 **********************************************************************************************************************/
void publish_seq_clear(void){
    p_seq = 0;
    publish_count=0;
    wz12_clear();
} // end publish_seq_clear()

/**********************************************************************************************************************
 *  @brief      Prepare <<(char) encoded[20480]>> for publishing to /AeviceMD/DeviceNAME/Waveform
 *  @param      (char*)       inDNAME       <<Device Nname>>
 *  @param  	(trueCheck_t) inTrueCheck   <<(char) encoded[20480]>> , <<(uint32_t) p_seq>>
 *  @retval      N.A.
 **********************************************************************************************************************/
//extern uint8_t readVal[SRAM_SEQ_SIZE];
int32_t pub_err=0;
void publish_str_encoded(char* inDname,trueCheck_t inTrueCheck){
    //	g_tic();
    volatile fsp_err_t status = FSP_ERR_ASSERTION;
    static uint32_t err_count = 0;
    uint32_t inCrc32Val_w=0;
    uint32_t inCrc32Val_r=0;

    // Encode fdata for transmission to Server
    memset(byteArray,0,sizeof(byteArray));

    // Prepare encoded data for transmission in size of 1024 bytes (~1sec)
    uint32_t dataset = 0;

    if (p_seq<60){ // dataset \in [0:19] Only
        for (dataset = 0; dataset<DATASIZE/AUDIOSIZE; dataset++) {

            /* (A) Convert each encoded to uint8_t */
            count_pub = 0;
            for (uint32_t idx = dataset*AUDIOSIZE; idx < (dataset*AUDIOSIZE)+AUDIOSIZE;idx++) {
                byteArray[count_pub] = (uint8_t)((getEncodedAddr()[idx]) & 0xFF);//(uint8_t)(encoded[idx] & 0xFF);
                count_pub++;
            }// end (A)

            /* (B) Convert byteArray to byteStr */
            count_pub = 0;
            memset(str_encoded, 0, sizeof(str_encoded));
            for (int idx = 0; idx < BYTEARRAYSIZE; idx++) {
                count_pub += (uint32_t)sprintf(&str_encoded[count_pub],"%02X",byteArray[idx]);
            } // end (B)

            /* (C) Write to SRAM */
#ifdef AUDIO_SRAM// #define TEST_AUS_WITHOUT_FDATA
            /* Write to SRAM */
#ifdef TEST_AUS_WITHOUT_FDATA
            strcpy(str_encoded,c_aus_val[p_seq]); // c_aus_val is a test-data from publishAusTester.c
#endif
            prepSEQ(p_seq,(uint8_t*)str_encoded);
            status = writeToSRAM();
            crcCompute(str_encoded,2048,&inCrc32Val_w);

            seq_CS[p_seq][0]=(uint8_t)inCrc32Val_w;
            if (status != FSP_SUCCESS)
                err_count++;

            /* Read from SRAM */
            uint32_t str_encoded_len = 0;
            static uint32_t str_encoded_len_err=0;
            str_encoded_len = strlen(str_encoded);
            if (str_encoded_len<2048)
                str_encoded_len_err++;
            memset(str_encoded, 0x00, sizeof(str_encoded));
            status  = readFromSRAM(p_seq,(uint8_t*)str_encoded);
            status |= crcCompute(str_encoded,2048,&inCrc32Val_r);

            seq_CS[p_seq][1]=(uint8_t)inCrc32Val_r;
            if (status != FSP_SUCCESS)
                err_count++;

            pub_err = (seq_CS[p_seq][0]==(seq_CS[p_seq][1]))? 0:1;
#endif

            /* (D) Publish Each Sequence */
            if ((inTrueCheck==t_AUS_MAN_15) || (inTrueCheck ==t_AUS_MAN_05))
                status= publishAudioSeq(inDname, "Waveform", p_seq,(char*)str_encoded,8,4);
            else if (inTrueCheck==t_AUS_CWD)
                status= publishAudioSeq(inDname, "Wheezing",p_seq,(char*)str_encoded,8,4);
            vTaskDelay(50); // sleep 50ms

            /* (E) Clean up str_encoded and byteArray Buffer*/
            memset(str_encoded, 0, sizeof(str_encoded));
            memset(byteArray,0,sizeof(byteArray));

            /* (F) Increase the Sequence Number of this set*/
            p_seq++;

        } // end for ii=1:20 Sequence
    }// end if (p_seq<60)
    //	g_toc();
    FSP_PARAMETER_NOT_USED(status);
} // end publish_str_encoded()


/* @brief  Write the first 5 checksum of the auscultation sequence to <<(char*) inBuf>>
 * @param  char*    inBuf            Pointer to array to store 1st 5-seq,checksum
 * @param  uint8_t  inSel            (0): values from sram-writing, (1): values from sram-reading
 * @param  uint8_t  seq_CS[60][2]    (Global) Array that stores the checksum from writing[:][0], and reading[:][1]
 * @return (char*) inBuf[10] = "123456789"  ID
 */
void g_generate_aur_ID(char* inBuf, uint8_t inSel){
    snprintf(inBuf,11,"%02X%02X%02X%02X%02X",seq_CS[0][inSel],seq_CS[1][inSel],
             seq_CS[2][inSel],seq_CS[3][inSel],
             seq_CS[4][inSel]);
} // end g_generate_aur_ID()
//void g_generate_aur_ID(char* inBuf, uint8_t* inCRC){
//  snprintf(inBuf,11,"%02X%02X%02X%02X%02X",inCRC[0],inCRC[1],inCRC[2],inCRC[3],inCRC[4]);
//} // end g_generate_aur_ID()

/**********************************************************************************************************************
 *  @brief      API to actuate the Publishing Activity based on the current states
 *  @param[in]  trueCheck_t         <inTrueCheck>
 *                       AUS_NONE/MAN/CWD/MAN_05/MAN_15/RESEND>
 *  @param[in]  dataprocess_state_t <inDataprocessState>
 *                      AUDIO_SET0/1/2/3/DONE/INIT/ERR
 *  @retval      N.A.
 *  @remarks     the dname,hr,rr,wz,wz12,ausVal are all global variables
 **********************************************************************************************************************/
void publish_state_actuator(trueCheck_t inTrueCheck, dataprocess_state_t inDataprocessState){
    volatile fsp_err_t status = FSP_ERR_ASSERTION;
    uint32_t inCrc32Val = 0;
    static uint32_t audio_set_err = 0;

    /* (1) Stop Auscultation */
    if (inTrueCheck==t_AUS_STOP){
        vTaskDelay(3000); // sleep 3000ms
        /* 1a: <<t_AUS_STOP>> can happen during AUS_MAN || AUS_CWD */
        uint8_t isCWD = inTrueCheck==t_AUS_CWD?1:0;
        status= publishClassification(p_dname,10,wz12,isCWD,p_ID,8,10);
        publish_seq_clear();
        setTrueCheckVal(t_AUS_NONE);
    } // end (t_AUS_STOP)
    /* (2) 5-sec Auscultation */
    else if (inTrueCheck==t_AUS_MAN_05){
        publish_str_encoded(p_dname, inTrueCheck);
        vTaskDelay(3000); // sleep 3000ms
        /* 2a: <<t_AUS_MAN_05>> is user triggered so isCWD=0 */
        status= publishClassification(p_dname,5,wz12,0,p_ID,8,10);
        publish_seq_clear();
        setTrueCheckVal(t_AUS_NONE);
    } // end end if (AUS_MAN_05)
    /* (3) 15-sec Auscultation: AUS_MAN || AUS_CWD */
    else if (inTrueCheck==t_AUS_MAN_15||inTrueCheck==t_AUS_CWD){
        switch (inDataprocessState){
            case AUDIO_SET1:{
                /* 3a: Publish SEQ 00 to SEQ 19 */
                publish_str_encoded(p_dname, inTrueCheck);
                publish_count=1;
                break;
            } // end AUDIO_SET1
            case AUDIO_SET2:{
                /* 3b: Publish SEQ 20 to SEQ 39 */
                publish_str_encoded(p_dname, inTrueCheck);
                publish_count=2;
                break;
            }// end AUDIO_SET2
            case AUDIO_SET3:{
                /* 3c(1): Publish SEQ 40 to SEQ 59 */
                publish_str_encoded(p_dname, inTrueCheck);
                publish_count=3;

                /* 3c(2): Extract SEQ01 to SEQ09's Checksum String as <<p_ID>> */
                g_generate_aur_ID(p_ID,0);// generate CS-ID from Writing to SRAM
                vTaskDelay(3000); // sleep 3000ms

                /* 3c(3): Publish classification <<wz12>>, <<p_ID>>, and <<rTime>> */
                uint8_t isCWD = inTrueCheck==t_AUS_CWD?1:0;
                status= publishClassification(p_dname,15,wz12,isCWD,p_ID,8,10);

                /* DEPRECIATED: Wait for AUS_STOP from Cloud via DUR 11 */
                //setTrueCheckVal(t_AUS_MAN_15);//setTrueCheckVal(AUS_NONE);
                break;
            }// end AUDIO_SET3
            default:{
                /* 3d: Logging of any out of sync publishing*/
                audio_set_err++;
                break;
            } // end default do nothing
        } // end switch (dataprocess_state_get())
    } // end if (AUS_MAN_15 || t_AUS_CWD)v
    /* (4) 15-sec AudioResend: AUS_MAN_RESEND || AUS_CWD_RESEND */
    else if ((inTrueCheck==t_AUS_MAN_RESEND)||(inTrueCheck==t_AUS_CWD_RESEND)){
        /* 4a: Compare sramID with requestID*/
        char sramID[10]={'\0'};
        bool p_aur_matchID = true;                 // pointer to sramID that matches ID from Cloud
        g_generate_aur_ID((char*)sramID,1);        // Extract sramID into <<sramID>>
        for (uint8_t ii=0;ii<sizeof(sramID);ii++)
            p_aur_matchID &= sramID[ii]==AudioResendID_ptr()[ii]?true:false;
        /* 4b(i): Logic for matching ID */
        if (p_aur_matchID){
            for (uint8_t seq=0;seq<60;seq++){
                if((AudioResendSelector_ptr()[seq])=='1'){
#ifdef AUDIO_SRAM
                    // (A) Read from SRAM
                    memset(str_encoded, 0x00, sizeof(str_encoded));
                    status = readFromSRAM(seq,(uint8_t*)str_encoded);
#ifdef TEST_AUS_RESEND_WITHOUT_SRAM
                    strcpy(str_encoded,c_aus_val[seq]); // c_aus_val is a test-data from publishAusTester.c
#endif

                    // (B) Check CRC
                    status |= crcCompute(str_encoded,2048,&inCrc32Val);

                    seq_CS[seq][1]=(uint8_t)inCrc32Val;
                    pub_err = (seq_CS[seq][0]==(seq_CS[seq][1]))? 0:1;

                    // (C) Publish Audio Resend Sequence
                    if (getTrueCheckVal()==t_AUS_MAN_RESEND)
                        status= publishResendSeq(p_dname,"Waveform",seq,(char*)str_encoded,8,4);
                    else if (getTrueCheckVal()==t_AUS_CWD_RESEND)
                        status= publishResendSeq(p_dname,"Wheezing",seq,(char*)str_encoded,8,4);
                    vTaskDelay(300); // sleep 300ms

                    // (D) Clear Error Array
                    AudioResendSelector_ptr()[seq]='0'; // clear Error array
                } // end pub(RESEND)
#endif
            } // end for seq=0:59
            vTaskDelay(3000); // sleep 3000ms
            /* Send AUDIO_RESEND Completed to Cloud */
            publishResendStatus(p_dname,p_ID,"200",8, 10);
        } // end if (p_aur_matchID)
        /* 4b(ii): Logic for mismatch ID */
        else {
            vTaskDelay(3000); // sleep 3000ms
            publishResendStatus(p_dname,p_ID,"404",8, 10);
        } // end if (!p_aur_matchID)

        /* 4c: Clean Up AUS_RESEND MODE */
        setTrueCheckVal(t_AUS_NONE);
        /* Wait for AUS_STOP from Cloud via DUR 11 */
        //setTrueCheckVal(t_AUS_MAN_15);//setTrueCheckVal(AUS_NONE);
    }// end if (AUS_RESEND)
    /* (5) Default Vitals Mode: Publish HR/RR */
    else{
        status= publishVital(p_dname, vitals_get(HR_PUBLISH), vitals_get(RR_PUBLISH), vitals_get(WZ_PUBLISH),
                             batt_publish_get(), bp_publish_get(),8,4);
    } // end if (AUS_NONE)

    FSP_PARAMETER_NOT_USED(status);
} // end publish_state_actuator()


/**********************************************************************************************************************
 *  @brief      API to test the Publishing Activity based on the current states
 *  @param[in]  trueCheck_t         <inTrueCheck>
 *                       AUS_NONE/MAN/CWD/MAN_05/MAN_15/RESEND>
 *  @param[in]  dataprocess_state_t <inDataprocessState>
 *                      AUDIO_SET0/1/2/3/DONE/INIT/ERR
 *  @retval      N.A.
 *  @remarks     the dname,hr,rr,wz,wz12,ausVal are all global variables
 **********************************************************************************************************************/
void publish_state_actuator_tester(trueCheck_t inTrueCheck, dataprocess_state_t inDataprocessState){
    volatile fsp_err_t status = FSP_ERR_ASSERTION;
    uint32_t inCrc32Val = 0;
    static uint32_t audio_set_err = 0;

    /* (1) Stop Auscultation */
    if (inTrueCheck==t_AUS_STOP){
        setTrueCheckVal(t_AUS_NONE);
        vTaskDelay(3000); // sleep 3000ms
        /* 1a: <<t_AUS_STOP>> can happen during AUS_MAN || AUS_CWD */
        uint8_t isCWD = inTrueCheck==t_AUS_CWD?1:0;
        status= publishClassification(p_dname,10,wz12,isCWD,p_ID,8,10);
        publish_seq_clear();
    } // end (t_AUS_STOP)
    /* (2) 5-sec Auscultation */
    else if (inTrueCheck==t_AUS_MAN_05){
        publish_str_encoded(p_dname, inTrueCheck);
        setTrueCheckVal(t_AUS_NONE);
        vTaskDelay(3000); // sleep 3000ms
        /* 2a: <<t_AUS_MAN_05>> is user triggered so isCWD=0 */
        status= publishClassification(p_dname,5,wz12,0,p_ID,8,10);
        publish_seq_clear();
    } // end end if (AUS_MAN_05)
    /* (3) 15-sec Auscultation: AUS_MAN || AUS_CWD */
    else if (inTrueCheck==t_AUS_MAN_15||inTrueCheck==t_AUS_CWD){
        switch (inDataprocessState){
            case AUDIO_SET1:{
                /* 3a: Publish SEQ 00 to SEQ 19 */
                for (uint8_t seq=0;seq<20;seq++)
                    status= publishAudioSeq(p_dname,"Waveform",seq,(char*)c_aus_val[seq],8,4);
                publish_count=1;
                break;
            } // end AUDIO_SET1
            case AUDIO_SET2:{
                /* 3b: Publish SEQ 20 to SEQ 39 */
                for (uint8_t seq=20;seq<40;seq++)
                    status= publishAudioSeq(p_dname,"Waveform",seq,(char*)c_aus_val[seq],8,4);
                publish_count=2;
                break;
            }// end AUDIO_SET2
            case AUDIO_SET3:{
                /* 3c(1): Publish SEQ 40 to SEQ 59 */
                for (uint8_t seq=40;seq<60;seq++)
                    status= publishAudioSeq(p_dname,"Waveform",seq,(char*)c_aus_val[seq],8,4);
                publish_count=3;

                /* 3c(2): Extract SEQ01 to SEQ09's Checksum String as <<p_ID>> */
                g_generate_aur_ID(p_ID,0);// generate CS-ID from Writing to SRAM
                vTaskDelay(3000); // sleep 3000ms

                /* 3c(3): Publish classification <<wz12>>, <<p_ID>>, and <<rTime>> */
                uint8_t isCWD = inTrueCheck==t_AUS_CWD?1:0;
                status= publishClassification(p_dname,15,wz12,isCWD,p_ID,8,10);

                /* DEPRECIATED: Wait for AUS_STOP from Cloud via DUR 11 */
                //setTrueCheckVal(t_AUS_MAN_15);//setTrueCheckVal(AUS_NONE);
                break;
            }// end AUDIO_SET3
            default:{
                /* 3d: Logging of any out of sync publishing*/
                audio_set_err++;
                break;
            } // end default do nothing
        } // end switch (dataprocess_state_get())
    } // end if (AUS_MAN_15 || t_AUS_CWD)v
    /* (4) 15-sec AudioResend: AUS_MAN_RESEND || AUS_CWD_RESEND */
    else if ((inTrueCheck==t_AUS_MAN_RESEND)||(inTrueCheck==t_AUS_CWD_RESEND)){
        /* 4a: Compare sramID with requestID*/
        char sramID[10]={'\0'};
        char *p_aur_matchID = &sramID[0];// pointer to sramID that matches ID from Cloud
        g_generate_aur_ID((char*)sramID,1);        // Extract sramID into <<sramID>>
        p_aur_matchID = strstr((char*)sramID,AudioResendID_ptr()); // Compare with Requested ID from Cloud
        /* 4b(i): Logic for matching ID */
        if (p_aur_matchID){
            for (uint8_t seq=0;seq<60;seq++){
                if((AudioResendSelector_ptr()[seq])=='1'){
#ifdef AUDIO_SRAM
                    // (A) Read from SRAM
                    memset(str_encoded, 0x00, sizeof(str_encoded));
                    status = readFromSRAM(seq,(uint8_t*)str_encoded);
#ifdef TEST_AUS_RESEND_WITHOUT_SRAM
                    strcpy(str_encoded,c_aus_val[seq]); // c_aus_val is a test-data from publishAusTester.c
#endif

                    // (B) Check CRC
                    status |= crcCompute(str_encoded,SIZE,&inCrc32Val);

                    seq_CS[seq][1]=(uint8_t)inCrc32Val;
                    pub_err = (seq_CS[seq][0]==(seq_CS[seq][1]))? 0:1;

                    // (C) Publish Audio Resend Sequence
                    if (getTrueCheckVal()==t_AUS_MAN_RESEND)
                        status= publishResendSeq(p_dname,"Waveform",seq,(char*)str_encoded,8,4);
                    else if (getTrueCheckVal()==t_AUS_CWD_RESEND)
                        status= publishResendSeq(p_dname,"Wheezing",seq,(char*)str_encoded,8,4);
                    vTaskDelay(300); // sleep 300ms

                    // (D) Clear Error Array
                    AudioResendSelector_ptr()[seq]='0'; // clear Error array
                } // end pub(RESEND)
#endif
            } // end for seq=0:59
            vTaskDelay(3000); // sleep 3000ms
            /* Send AUDIO_RESEND Completed to Cloud */
            publishResendStatus(p_dname,p_ID,"200",8, 10);
        } // end if (p_aur_matchID)
        /* 4b(ii): Logic for mismatch ID */
        else {
            vTaskDelay(3000); // sleep 3000ms
            publishResendStatus(p_dname,p_ID,"404",8, 10);
        } // end if (!p_aur_matchID)

        /* 4c: Clean Up AUS_RESEND MODE */
        setTrueCheckVal(t_AUS_NONE);
        /* Wait for AUS_STOP from Cloud via DUR 11 */
        //setTrueCheckVal(t_AUS_MAN_15);//setTrueCheckVal(AUS_NONE);
    }// end if (AUS_RESEND)
    /* (5) Default Vitals Mode: Publish HR/RR */
    else{
        status= publishVital(p_dname, vitals_get(HR_PUBLISH), vitals_get(RR_PUBLISH), vitals_get(WZ_PUBLISH),
                             batt_publish_get(), bp_publish_get(),8,4);
    } // end if (AUS_NONE)

    FSP_PARAMETER_NOT_USED(status);
} // end publish_state_actuator_tester()
