/*
 * bleHandler.h
 *
 *  Created on: 7 Aug 2024
 *      Author: awlir
 */

#ifndef BLEHANDLER_H_
#define BLEHANDLER_H_
#include "initialisations.h"
//#include "rgbHandler.h"
//#include "SystemVariables.h"
////#include "Algorithm\imuHandler.h"


//#define UART_DATA_SIZE      61440               //5sec UART Buffer
#define MAX_UART_INIT_DATA_COUNT                (12 - MAX_UART_VITALS_DATA_COUNT)//early computed vitals
#define MAX_UART_VITALS_DATA_COUNT              (3)
#define MAX_UART_VITALS_ON_DATA_COUNT_SEC       (10)
#define MAX_UART_VITALS_OFF_DATA_COUNT_SEC      (30)
#define MAX_UART_VITALS_OFF_DATA_TIMEOUT_SEC    (10 + MAX_UART_VITALS_OFF_DATA_COUNT_SEC)
typedef enum ble_controller_state {
    BLEC_OFF = 0,                               //real-time chart display purposes
    BLEC_VITALS  = 1,      //real-time chart display purposes (BLE_ON: 15-sec; BLE_OFF: 45-sec)
    BLEC_VITALS_OFF=2,
    BLEC_INIT    = 3,        //real-time chart display purposes (BLE_ON: 60-sec)
    BLEC_ALL_ON  = 4,    //real-time chart display purposes
} blec_state_t;


uint8_t* getUartDataAddr(void);

blec_state_t BLEC_state_get(void);
void BLEC_state_set(blec_state_t inState);
blec_state_t BLEC_prev_get(void);
void BLEC_prev_set(blec_state_t inprev);
void blec_state_update(RGB_Colour inLED, imu_status_t inOnbody, blec_state_t inBlec_state, trueCheck_t in_true_check,uint32_t* counter);

void BLE_Set_On_Always(bool inBool);
bool BLE_Get_On_Always(void);
uint8_t BLE_check(void);
uint8_t BLE_connect(void);
uint8_t BLE_disconnect(void);
bool BLE_On_Max_Wait(uint8_t timeout_in_sec);

#endif /* BLEHANDLER_H_ */
