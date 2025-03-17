/*
 * sramHandler.c
 *
 *  Created on: 2 Aug 2024
 *      Author: yyang
 */

#include "initialisations.h"

#define SRAM_WRITE_LEN 			2048
#define SRAM_READ_LEN 			SRAM_WRITE_LEN+4 //because first 4 bytes are 0xFF
#define SRAM_PAYLOAD_LEN 		SRAM_READ_LEN //because first 4 bytes are 0xFF

const uint8_t RDMR  =      0x05;   // Read the Mode Register
const uint8_t WRMR  =      0x01;   // Write to the Mode Register
const uint8_t READ  =      0x03;   // Byte Read command
const uint8_t WRITE =      0x02;   // Byte Write command
const uint8_t RSTIO =      0xFF;   // Reset memory to SPI mode
const uint8_t ByteMode =   0x00;   // Byte mode (read/write one byte at a time)
const uint8_t Sequential = 0x40;   // Sequential mode (read/write blocks of memory)
const uint8_t Page =       0x80;   // Page mode (read/write blocks of memory)
const uint8_t READING    = 0x50;   // Sequential mode (read/write blocks of memory)

const uint8_t config_8[] ={0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0A};//, 0x05, 0xAA, 0x05, 0xAA, 0x05, 0xAA, 0x05, 0xAA, 0x05};
const uint8_t addr[]={0xFE,0x00,0x00};
uint8_t readSpi[] = {0x03,0xFE,0x00,0x00};
uint8_t modeRest[] = {0x01,0xFF}; // MODE: Reset to SPI
uint8_t modeByte[] = {0x01,0x00}; // MODE: Write Mode Register, MSG: Byte
uint8_t modeSeq_t[] = {0x01,0x40}; // MODE: Write Mode Register, MSG: Sequential
uint8_t modePage[] = {0x01,0x80}; // MODE: Write Mode Register, MSG: Paging
uint8_t payload[15] = {0};
uint8_t payloadArr[SRAM_PAYLOAD_LEN] = {0};//[mode,addr[0],addr[1],addr[2],ByteArray[1024]] Array to store auscultation's byteArray[1024]
uint8_t read_8[15] = {0};
uint8_t tmp = 0;
fsp_err_t sram_err = FSP_SUCCESS;

// Address Mapping for SEQ to SRAM Address Space (1024 Spacing)
//uint8_t seqAdd[60][3]={
//                       {0x00,0x04,0x00},// Address for SEQ0
//                       {0x00,0x08,0x00},// Address for SEQ1
//                       {0x00,0x0C,0x00},// Address for SEQ2
//                       {0x00,0x10,0x00},// Address for SEQ3
//                       {0x00,0x14,0x00},// Address for SEQ4
//                       {0x00,0x18,0x00},// Address for SEQ5
//                       {0x00,0x1C,0x00},// Address for SEQ6
//                       {0x00,0x20,0x00},// Address for SEQ7
//                       {0x00,0x24,0x00},// Address for SEQ8
//                       {0x00,0x28,0x00},// Address for SEQ9
//                       {0x00,0x2C,0x00},// Address for SEQ10
//                       {0x00,0x30,0x00},// Address for SEQ11
//                       {0x00,0x34,0x00},// Address for SEQ12
//                       {0x00,0x38,0x00},// Address for SEQ13
//                       {0x00,0x3C,0x00},// Address for SEQ14
//                       {0x00,0x40,0x00},// Address for SEQ15
//                       {0x00,0x44,0x00},// Address for SEQ16
//                       {0x00,0x48,0x00},// Address for SEQ17
//                       {0x00,0x4C,0x00},// Address for SEQ18
//                       {0x00,0x50,0x00},// Address for SEQ19
//                       {0x00,0x54,0x00},// Address for SEQ20
//                       {0x00,0x58,0x00},// Address for SEQ21
//                       {0x00,0x5C,0x00},// Address for SEQ22
//                       {0x00,0x60,0x00},// Address for SEQ23
//                       {0x00,0x64,0x00},// Address for SEQ24
//                       {0x00,0x68,0x00},// Address for SEQ25
//                       {0x00,0x6C,0x00},// Address for SEQ26
//                       {0x00,0x70,0x00},// Address for SEQ27
//                       {0x00,0x74,0x00},// Address for SEQ28
//                       {0x00,0x78,0x00},// Address for SEQ29
//                       {0x00,0x7C,0x00},// Address for SEQ30
//                       {0x00,0x80,0x00},// Address for SEQ31
//                       {0x00,0x84,0x00},// Address for SEQ32
//                       {0x00,0x88,0x00},// Address for SEQ33
//                       {0x00,0x8C,0x00},// Address for SEQ34
//                       {0x00,0x90,0x00},// Address for SEQ35
//                       {0x00,0x94,0x00},// Address for SEQ36
//                       {0x00,0x98,0x00},// Address for SEQ37
//                       {0x00,0x9C,0x00},// Address for SEQ38
//                       {0x00,0xA0,0x00},// Address for SEQ39
//                       {0x00,0xA4,0x00},// Address for SEQ40
//                       {0x00,0xA8,0x00},// Address for SEQ41
//                       {0x00,0xAC,0x00},// Address for SEQ42
//                       {0x00,0xB0,0x00},// Address for SEQ43
//                       {0x00,0xB4,0x00},// Address for SEQ44
//                       {0x00,0xB8,0x00},// Address for SEQ45
//                       {0x00,0xBC,0x00},// Address for SEQ46
//                       {0x00,0xC0,0x00},// Address for SEQ47
//                       {0x00,0xC4,0x00},// Address for SEQ48
//                       {0x00,0xC8,0x00},// Address for SEQ49
//                       {0x00,0xCC,0x00},// Address for SEQ50
//                       {0x00,0xD0,0x00},// Address for SEQ51
//                       {0x00,0xD4,0x00},// Address for SEQ52
//                       {0x00,0xD8,0x00},// Address for SEQ53
//                       {0x00,0xDC,0x00},// Address for SEQ54
//                       {0x00,0xE0,0x00},// Address for SEQ55
//                       {0x00,0xE4,0x00},// Address for SEQ56
//                       {0x00,0xE8,0x00},// Address for SEQ57
//                       {0x00,0xEC,0x00},// Address for SEQ58
//                       {0x00,0xF0,0x00},// Address for SEQ59
//}; // uint8_t seqAdd[60][3] Address Mapping for SEQ to SRAM Address Space

// Address Mapping for SEQ to SRAM Address Space (2048 Spacing)
uint8_t seqAdd[60][3]={
                       {0x00, 0x00, 0x00},// Address for SEQ0
                       {0x00, 0x08, 0x00},// Address for SEQ1
                       {0x00, 0x10, 0x00},// Address for SEQ2
                       {0x00, 0x18, 0x00},// Address for SEQ3
                       {0x00, 0x20, 0x00},// Address for SEQ4
                       {0x00, 0x28, 0x00},// Address for SEQ5
                       {0x00, 0x30, 0x00},// Address for SEQ6
                       {0x00, 0x38, 0x00},// Address for SEQ7
                       {0x00, 0x40, 0x00},// Address for SEQ8
                       {0x00, 0x48, 0x00},// Address for SEQ9
                       {0x00, 0x50, 0x00},// Address for SEQ10
                       {0x00, 0x58, 0x00},// Address for SEQ11
                       {0x00, 0x60, 0x00},// Address for SEQ12
                       {0x00, 0x68, 0x00},// Address for SEQ13
                       {0x00, 0x70, 0x00},// Address for SEQ14
                       {0x00, 0x78, 0x00},// Address for SEQ15
                       {0x00, 0x80, 0x00},// Address for SEQ16
                       {0x00, 0x88, 0x00},// Address for SEQ17
                       {0x00, 0x90, 0x00},// Address for SEQ18
                       {0x00, 0x98, 0x00},// Address for SEQ19
                       {0x00, 0xA0, 0x00},// Address for SEQ20
                       {0x00, 0xA8, 0x00},// Address for SEQ21
                       {0x00, 0xB0, 0x00},// Address for SEQ22
                       {0x00, 0xB8, 0x00},// Address for SEQ23
                       {0x00, 0xC0, 0x00},// Address for SEQ24
                       {0x00, 0xC8, 0x00},// Address for SEQ25
                       {0x00, 0xD0, 0x00},// Address for SEQ26
                       {0x00, 0xD8, 0x00},// Address for SEQ27
                       {0x00, 0xE0, 0x00},// Address for SEQ28
                       {0x00, 0xE8, 0x00},// Address for SEQ29
                       {0x00, 0xF0, 0x00},// Address for SEQ30
                       {0x00, 0xF8, 0x00},// Address for SEQ31
                       {0x01, 0x00, 0x00},// Address for SEQ32
                       {0x01, 0x08, 0x00},// Address for SEQ33
                       {0x01, 0x10, 0x00},// Address for SEQ34
                       {0x01, 0x18, 0x00},// Address for SEQ35
                       {0x01, 0x20, 0x00},// Address for SEQ36
                       {0x01, 0x28, 0x00},// Address for SEQ37
                       {0x01, 0x30, 0x00},// Address for SEQ38
                       {0x01, 0x38, 0x00},// Address for SEQ39
                       {0x01, 0x40, 0x00},// Address for SEQ40
                       {0x01, 0x48, 0x00},// Address for SEQ41
                       {0x01, 0x50, 0x00},// Address for SEQ42
                       {0x01, 0x58, 0x00},// Address for SEQ43
                       {0x01, 0x60, 0x00},// Address for SEQ44
                       {0x01, 0x68, 0x00},// Address for SEQ45
                       {0x01, 0x70, 0x00},// Address for SEQ46
                       {0x01, 0x78, 0x00},// Address for SEQ47
                       {0x01, 0x80, 0x00},// Address for SEQ48
                       {0x01, 0x88, 0x00},// Address for SEQ49
                       {0x01, 0x90, 0x00},// Address for SEQ50
                       {0x01, 0x98, 0x00},// Address for SEQ51
                       {0x01, 0xA0, 0x00},// Address for SEQ52
                       {0x01, 0xA8, 0x00},// Address for SEQ53
                       {0x01, 0xB0, 0x00},// Address for SEQ54
                       {0x01, 0xB8, 0x00},// Address for SEQ55
                       {0x01, 0xC0, 0x00},// Address for SEQ56
                       {0x01, 0xC8, 0x00},// Address for SEQ57
                       {0x01, 0xD0, 0x00},// Address for SEQ58
                       {0x01, 0xD8, 0x00},// Address for SEQ59
                       //{0x01, 0xE0, 0x00}//Address for SEQ60
}; // uint8_t seqAdd[60][3] Address Mapping for SEQ to SRAM Address Space (2048)


void payLoad( uint8_t MODE, uint8_t* ADDR,size_t numByte_ADDR,uint8_t* MSG,size_t numByte_MSG){
    volatile uint8_t i,ii ;
    //volatile size_t numByte_ADDR = 4; // 24-bit Address (3-Bytes)
    ii = 1; // store for next for loop
    memset(payload,0,sizeof(payload));

    // Define RAM's MODE onto payload array
    payload[0] =MODE;

    // Define RAM's Address onto payload array
    for (i=0;i<(uint8_t)numByte_ADDR;i++)
        payload[i+ii]=ADDR[i];

    ii = (uint8_t)(ii+(uint8_t)numByte_ADDR); // store for next for loop

    // Define RAM's Msg to be written to address
    for (i=0;i<(uint8_t)numByte_MSG;i++)
        payload[i+ii]=MSG[i];
}// payLoad()


//extern const char c_aus_val[60][2049];
char str_encoded_test[SIZE]; // To hold the str_encoded_test
uint8_t seq_CS_t[60][2];  // To hold the computed crc
bool g_sram_test(void){

    volatile fsp_err_t status = FSP_ERR_ASSERTION;
    bool ok_status=false;

    uint32_t inCrc32Val_w=0;
    uint32_t inCrc32Val_r=0;
    volatile uint32_t err_count = 0;
    for (uint8_t seq=0;seq<60;seq++){
        strcpy(str_encoded_test,c_aus_val[seq]);
        prepSEQ(seq,(uint8_t*)str_encoded_test);
        status = writeToSRAM();

        status |= crcCompute(str_encoded_test,2048,&inCrc32Val_w);

        seq_CS_t[seq][0]=(uint8_t)inCrc32Val_w;
        if (status != FSP_SUCCESS)
            err_count++;

        /* Read from SRAM */
        memset(str_encoded_test, 0x00, sizeof(str_encoded_test));
        status  = readFromSRAM(seq,(uint8_t*)str_encoded_test);
        status |= crcCompute(str_encoded_test,2048,&inCrc32Val_r);

        seq_CS_t[seq][1]=(uint8_t)inCrc32Val_r;
        if (status != FSP_SUCCESS)
            err_count++;

        ok_status = (seq_CS_t[seq][0]==(seq_CS_t[seq][1]))? true:false;
    } // for seq=0:59
    return (bool) ok_status;
}// g_sram_test()


void prepSEQ(uint8_t seqIdx,uint8_t* MSG){

    memset(payloadArr,0,sizeof(payloadArr));

    // Define RAM's Write MODE onto payload array
    payloadArr[0] =WRITE;

    // Define RAM's Address onto payload array
    //for (uint8_t addrIdx=0;addrIdx<(uint8_t)3;addrIdx++)
    //    payloadArr[1+addrIdx]=seqAdd[seqIdx][addrIdx];
    payloadArr[1] = (uint8_t)((((uint32_t)seqIdx*SRAM_WRITE_LEN) & 0xFF0000) >> 16);
    payloadArr[2] = (uint8_t)((((uint32_t)seqIdx*SRAM_WRITE_LEN) & 0x00FF00) >> 8);
    payloadArr[3] = (uint8_t)((((uint32_t)seqIdx*SRAM_WRITE_LEN) & 0x0000FF) >> 0);

    // Define RAM's Msg to be written to address
    for (uint16_t byteIdx=0;byteIdx<(uint16_t)SRAM_WRITE_LEN;byteIdx++)
        payloadArr[4+byteIdx]=MSG[byteIdx];
} // prepSEQ()

fsp_err_t g_sram_init(void){
    fsp_err_t status=FSP_ERR_ASSERTION;
    status = R_SCI_SPI_Open(&g_sram_ctrl, &g_sram_cfg);

    return status;
} // end initSramSpi();

fsp_err_t g_sram_deinit(void){
    fsp_err_t status=FSP_ERR_ASSERTION;
    status = R_SCI_SPI_Close(&g_sram_ctrl);

    return status;
} // end g_sram_deinit()

fsp_err_t writeToSRAM(void){

    // Using SPI Framework
    //    sram_err  = g_sf_sram_spi.p_api->open(g_sf_sram_spi.p_ctrl,g_sf_sram_spi.p_cfg);
    //    sram_err &= g_sf_sram_spi.p_api->write(g_sf_sram_spi.p_ctrl, &modeRest, sizeof(modeRest), SPI_BIT_WIDTH_8_BITS, TX_WAIT_FOREVER);
    //    sram_err &= g_sf_sram_spi.p_api->write(g_sf_sram_spi.p_ctrl, &modeSeq_t, sizeof(modeSeq_t), SPI_BIT_WIDTH_8_BITS, TX_WAIT_FOREVER);
    //    sram_err &= g_sf_sram_spi.p_api->write(g_sf_sram_spi.p_ctrl, payloadArr, sizeof(payloadArr), SPI_BIT_WIDTH_8_BITS, TX_WAIT_FOREVER);
    //    sram_err &= g_sf_sram_spi.p_api->close(g_sf_sram_spi.p_ctrl);

    sram_err = R_SCI_SPI_Open(&g_sram_ctrl, &g_sram_cfg);

    SRAM_ChipSelect_Low();
    sram_err = R_SCI_SPI_Write(&g_sram_ctrl, &modeRest, sizeof(modeRest), SPI_BIT_WIDTH_8_BITS);
    SRAM_Handle_TransferComplete(sizeof(modeRest));
    SRAM_ChipSelect_Low();
    sram_err = R_SCI_SPI_Write(&g_sram_ctrl, &modeSeq_t, sizeof(modeSeq_t), SPI_BIT_WIDTH_8_BITS);
    SRAM_Handle_TransferComplete(sizeof(modeSeq_t));
    SRAM_ChipSelect_Low();
    sram_err = R_SCI_SPI_Write(&g_sram_ctrl, payloadArr, sizeof(payloadArr), SPI_BIT_WIDTH_8_BITS);
    SRAM_Handle_TransferComplete(sizeof(payloadArr));
    sram_err = R_SCI_SPI_Close(&g_sram_ctrl);

    return sram_err;
}; //writeToSRAM()

uint8_t dstBuff[SRAM_READ_LEN]={0};
fsp_err_t readFromSRAM(uint8_t seqIdx, uint8_t* dst){
    uint8_t readAddr[] = {0x03,0x00,0x00,0x00}; //0x03 is command for read


    // Define RAM's Address onto payload array
    //for (uint8_t addrIdx=0;addrIdx<(uint8_t)3;addrIdx++)
    //    readAddr[1+addrIdx]=seqAdd[seqIdx][addrIdx];
    readAddr[1] = (uint8_t)((((uint32_t)seqIdx*SRAM_WRITE_LEN) & 0xFF0000) >> 16);
    readAddr[2] = (uint8_t)((((uint32_t)seqIdx*SRAM_WRITE_LEN) & 0x00FF00) >> 8);
    readAddr[3] = (uint8_t)((((uint32_t)seqIdx*SRAM_WRITE_LEN) & 0x0000FF) >> 0);

//    sram_err  = g_sf_sram_spi.p_api->open(g_sf_sram_spi.p_ctrl,g_sf_sram_spi.p_cfg);
//    sram_err &= g_sf_sram_spi.p_api->writeRead (g_sf_sram_spi.p_ctrl,&readAddr, &dstBuff, sizeof(dstBuff), SPI_BIT_WIDTH_8_BITS,TX_WAIT_FOREVER);
//    sram_err &= g_sf_sram_spi.p_api->close(g_sf_sram_spi.p_ctrl);

    sram_err = R_SCI_SPI_Open(&g_sram_ctrl, &g_sram_cfg);
    SRAM_ChipSelect_Low();
    sram_err = R_SCI_SPI_WriteRead(&g_sram_ctrl, &readAddr, &dstBuff, sizeof(dstBuff), SPI_BIT_WIDTH_8_BITS);
    SRAM_Handle_TransferComplete(sizeof(dstBuff));
    sram_err = R_SCI_SPI_Close(&g_sram_ctrl);

    // The first four byte values for dstBuff is 0xFF (so Ignore)
    for (uint16_t idx = 0;idx<SRAM_WRITE_LEN;idx++)
        dst[idx]=dstBuff[idx+4];

    return sram_err;
}

uint32_t spi_count=0;
bool spi_transfer_done = false;
void sram_callback(spi_callback_args_t *p_args) {
    FSP_PARAMETER_NOT_USED(p_args);
    if(p_args->event == SPI_EVENT_TRANSFER_COMPLETE) {
        spi_transfer_done = true;
        R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_07, BSP_IO_LEVEL_HIGH);
        spi_count++;

        //if(spi_count == 128) {
        //    RTT_LOG("Completed 128");
        //}

        spi_count %= 128;
    }
    else {

    }
}


void SRAM_ChipSelect_Low(void) {
    R_IOPORT_PinWrite(&g_ioport_ctrl, BSP_IO_PORT_02_PIN_07, BSP_IO_LEVEL_LOW);
    spi_transfer_done = false;
}

void SRAM_Handle_TransferComplete(uint16_t transferSize) {
    for(uint16_t i = 0; i < transferSize; i++) {
        if(!spi_transfer_done) {
            TickType_t xLastWakeTime = xTaskGetTickCount();
            vTaskDelayUntil(&xLastWakeTime, 10); // sleep 10ms
        }
        else break;
    }
}
