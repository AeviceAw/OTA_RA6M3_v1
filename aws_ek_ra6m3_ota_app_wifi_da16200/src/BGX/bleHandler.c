/*
 * bleHandler.c
 *
 *  Created on: 7 Aug 2024
 *      Author: yyang
 */

#include "initialisations.h"
//#include "bleHandler.h"
//#include "rgbHandler.h"
//#include "SystemVariables.h"
//#include "Algorithm\imuHandler.h"

uint8_t g_ble= 0;
bool g_ble_on_always = false;


uint8_t uart_data[EUART_DATA_SIZE] = {0};

/******************************************************************************************************************//**
 *  @brief       Get the address location of   <<(uint8_t)  uart_data[61440]>>
 *  @param       none     <<(uint8_t*)  uart_data[61440]>>
 *  @retval      (uint8_t*)  uart_data[61440]
 **********************************************************************************************************************/
uint8_t* getUartDataAddr(void){
    return (uint8_t*) uart_data;
} // end getUartDataAddr()

blec_state_t g_blec_state = BLEC_INIT;
blec_state_t g_blec_prev = BLEC_INIT;

/******************************************************************************************************************//**
 *  @brief       get the value of the <<g_blec_state>>
 *  @retval      blec_state_t ALL_OFF/VITALS/INIT/ALL_ON
 **********************************************************************************************************************/
blec_state_t BLEC_state_get(void) {
    return g_blec_state;
} // end BLEC_state_get()

/******************************************************************************************************************//**
 *  @brief       set the value of the <<g_blec_state>>
 *  param[in]    blec_state_t ALL_OFF/VITALS/INIT/ALL_ON
 *  @retval      N.A.
 **********************************************************************************************************************/
void BLEC_state_set(blec_state_t inState) {
    g_blec_state= inState;
} // end BLEC_state_set()

/******************************************************************************************************************//**
 *  @brief       get the value of the <<g_blec_prev>>
 *  @retval      blec_prev_t ALL_OFF/VITALS/INIT/ALL_ON
 **********************************************************************************************************************/
blec_state_t BLEC_prev_get(void) {
    return g_blec_prev;
} // end BLEC_prev_get()

/******************************************************************************************************************//**
 *  @brief       set the value of the <<g_blec_prev>>
 *  param[in]    blec_prev_t ALL_OFF/VITALS/INIT/ALL_ON
 *  @retval      N.A.
 **********************************************************************************************************************/
void BLEC_prev_set(blec_state_t inprev) {
    g_blec_prev= inprev;
} // end BLEC_prev_set()


/******************************************************************************************************************//**
 *  @brief       update the value of the <<g_blec_state>>
 *  @param       blec_state_t Init/All_ON/All_off/Vitals
 *  @retval      N.A
 **********************************************************************************************************************/
void blec_state_update(RGB_Colour inLED, imu_status_t inOnbody, blec_state_t inBlec_state, trueCheck_t in_true_check,uint32_t* counter){

    // =====================================================================================
    /* (A) Update BLEC_Prev_state Value with Current value */
    BLEC_prev_set(BLEC_state_get());

    /* (B) Check of Sensor is on Dock*/
    if ((inLED==GREEN_GLOWING)||(inLED==GREEN_PULSING)){
        (*counter)=0;
        BLEC_state_set(BLEC_OFF);
        return;
    } // end if (SENSOR_ON_DOCK)

    /* (C) Check for Auscultation Logic */
    if (in_true_check!=t_AUS_NONE){
        (*counter)=0;
        BLE_On_Max_Wait(15); //BLE_connect();
        BLEC_state_set(BLEC_ALL_ON);
    } // if (true_check)
    else if (BLEC_state_get()==BLEC_ALL_ON){
        BLEC_state_set(BLEC_VITALS);
    } // end if (t_AUS_NONE && BLEC_ALL_ON)
    else if (inOnbody==IMU_OFF_BODY){
        //FIXME: ENABLE IMU_OFF_BODY Recovery Logic
        //BLE_disconnect();
        //imu_onbody_set(IMU_INIT);
    } // end if (t_AUS_NONE && SENSOR_OFF_BODY)

    //=========================================================================================

    /* (D) Initial or Vitals or Vitals_OFF Logic */
    switch (inBlec_state){
        case BLEC_VITALS_OFF:
            (*counter)++;
            BLE_disconnect();
            if ((*counter)>=3){
                (*counter)=0;
                BLE_On_Max_Wait(15);//                BLE_connect();
                BLEC_state_set(BLEC_VITALS);
            } // end switch (BLEC_VITALS_OFF && ii>2)
            break;
        case BLEC_VITALS:
            if(!BLE_check()){
                BLE_connect();
            } // end (!g_ble)
            else{
                (*counter)++;
                if ((*counter)>=3){
                    BLEC_state_set(BLEC_VITALS_OFF);
                    BLE_disconnect();
                    (*counter)=0;
                } // end switch (BLEC_VITALS && ii>2)
            } // end (g_ble)
            break;
        case BLEC_INIT:
            if(!BLE_check()){
                BLE_connect();
            } // end (!g_ble)
            else{
                (*counter)++;
                if (*counter>=12){
                    (*counter)=0;
                    BLEC_state_set(BLEC_VITALS);
                } // if (BLEC_INIT && ii>1)
            } // end (g_ble)
            break;
        case BLEC_OFF:
            if(BLE_check()){
                BLE_disconnect();
            } // end (!g_ble)
            break;
        case BLEC_ALL_ON:
            if(!BLE_check()){
                BLE_connect();
            } // end (!g_ble)
            break;
    } // end switch(inBlec_state)
} // end blec_state_update()

/*******************************************************************************************************************//**
 * @brief       Set the Flag for BLE always ON
 * @param[in]   (bool) state true/false
 * @retval      none  g_ble_on_always = state
 ***********************************************************************************************************************/
void BLE_Set_On_Always(bool inBool) {
    g_ble_on_always=inBool;
} // end BLE_Set_On_Always()

/*******************************************************************************************************************//**
 * @brief       Get the Flag for BLE always ON
 * @param[in]   None
 * @retval      (bool) state true/false
 ***********************************************************************************************************************/
bool BLE_Get_On_Always(void) {
    return  g_ble_on_always;
} // end BLE_Get_On_Always()

/*******************************************************************************************************************//**
 * @brief       Read P102->LYRA GPIO2 g_ble?
 * @param[in]   None
 * @retval      (uint8_t) g_ble = 1/0
 ***********************************************************************************************************************/
uint8_t BLE_check(void){
    bsp_io_level_t ble_state = BSP_IO_LEVEL_LOW;
    uint8_t bletemp = 9;
    ble_state=R_BSP_PinRead((bsp_io_port_pin_t) IOPORT_PORT_01_PIN_02);
    if (ble_state == BSP_IO_LEVEL_HIGH)
        bletemp = 1;
    else
        bletemp = 0;
    g_ble = bletemp;

    return g_ble;
} // end BLE_Check()

/*******************************************************************************************************************//**
 * @brief       Set P114->LYRA GPIO7 BLE_CONNECT
 * @param[in]   None
 * @retval      uint8_t g_ble
 ***********************************************************************************************************************/
uint8_t BLE_connect(void){
    bsp_io_level_t state = BSP_IO_LEVEL_LOW;
    R_BSP_PinWrite((bsp_io_port_pin_t) IOPORT_PORT_01_PIN_14,BSP_IO_LEVEL_HIGH);
    vTaskDelay(100); // sleep 100ms
    R_BSP_PinWrite((bsp_io_port_pin_t) IOPORT_PORT_01_PIN_14,BSP_IO_LEVEL_LOW);
    vTaskDelay(100); // sleep 100ms

    state=R_BSP_PinRead((bsp_io_port_pin_t) IOPORT_PORT_01_PIN_02);
    g_ble = state == BSP_IO_LEVEL_HIGH? 1:0;
    return g_ble;
} // end ble_connect()

/*******************************************************************************************************************//**
 * @brief       Set P115->LYRA GPIO6 BLE_DISCONNECT
 * @param[in]   None
 * @retval      uint8_t g_ble
 ***********************************************************************************************************************/
uint8_t BLE_disconnect(void){
    bsp_io_level_t state = BSP_IO_LEVEL_HIGH;
    if(!BLE_Get_On_Always()) {
        R_BSP_PinWrite((bsp_io_port_pin_t) IOPORT_PORT_01_PIN_15,BSP_IO_LEVEL_HIGH);
        vTaskDelay(100); // sleep 100ms
        R_BSP_PinWrite((bsp_io_port_pin_t) IOPORT_PORT_01_PIN_15,BSP_IO_LEVEL_LOW);
        vTaskDelay(100); // sleep 100ms

        state=R_BSP_PinRead((bsp_io_port_pin_t) IOPORT_PORT_01_PIN_02);
        g_ble = state == BSP_IO_LEVEL_HIGH? 1:0;
        vTaskDelay(250); // sleep 250ms
    } // end if(!BLE_Get_On_Always())
    return g_ble;
} // end ble_disconnect()


/*******************************************************************************************************************//**
 * @brief       This is a blocking function to connect and wait for g_ble ==1 for at most <<sec>>
 * @param[in]   uint8_t sec
 * @retval      bool (g_ble ==1 within timeout)? True: False
 ***********************************************************************************************************************/
bool BLE_On_Max_Wait(uint8_t timeout_in_sec){
    for (uint8_t ii =0;ii<timeout_in_sec;ii++){
        if (BLE_check()){
            return true;
            break;
        } // end if (break sleep since ble==1)
        else {
            BLE_connect();
            vTaskDelay(1000); // sleep 1-sec
        } // end if (sleep because ble==0)
    }   // end wait for ble == 1
    return false;
} // end BLE_Max_Wait()

