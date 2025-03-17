/*
 * SystemVariables.h
 *
 *  Created on: 5 Jun 2024
 *      Author: WilsonQuah
 */

#ifndef SYSTEMFUNCTIONS_SYSTEMVARIABLES_H_
#define SYSTEMFUNCTIONS_SYSTEMVARIABLES_H_

/* AWS Command State  */
typedef enum aws_cmd_enum {
    CMD_NONE           = 0, // Init
    CMD_WATCHDOG_TEST  = 13, // CMD 13
    CMD_KMDC_THRES_GET =33, // CMD 33
    CMD_BLE_ALWAYS_ON  = 81, // CMD 81
    CMD_BLE_NORMAL     = 82, // CMD 82
    CMD_WZ_NORMAL      = 83, // CMD 83
    CMD_WZ_ALWAYS_ON   = 84, // CMD 84
    CMD_WZ_ALWAYS_OFF  = 85, // CMD 85
    CMD_INIT_VAR       = 99, // CMD 99
} aws_cmd_t;

/* IMU State Variables */
typedef enum imu_status_enum {
    IMU_INIT        = 0,
    IMU_ON_BODY     = 1,
    IMU_OFF_BODY    = 2,
} imu_status_t;

/* SSIDPW */
typedef enum fbState_flag{
    fb_INIT = 0,
    fb_YES = 1,
    fb_CONNECTING= 2,
    fb_WRONG=3,
    fb_READY=4
}fbState_t;

void g_feedbackready_set(uint8_t inVal);
uint8_t g_feedbackready_get(void);
void g_feedbackState_set(fbState_t inVal);
fbState_t g_feedbackState_get(void);

/* SRAM Fit for Work Status */
bool g_sram_get(void);
void g_sram_set(bool inBool);

/*************************************************************
 * State Variables for INTERNET STATE
 ************************************************************/
/* Inputs for wifiHandler's internet_state_get */
typedef enum internet_state_flag{
    // Inputs option for internet_state_get()
    G_WIFI=(uint8_t)1,
    G_MQTT=(uint8_t)2,
    G_UNKNOWN
}internet_state_t;

/* Output for wifiHandler g_variable g_mqtt */
typedef enum mqtt_status {
    // Output for g_mqtt
    MQTT_ON  = 1,
    MQTT_OFF = 0
} mqtt_status_t;

/* Output for wifiHandler g_variable g_wifi */
typedef enum wifi_status {
    // Output for w_mqtt
    WIFI_ON  = 1,
    WIFI_OFF = 0
} wifi_status_t;


/* (1b) For setting/clearing g_variables: g_wifi/g_mqtt */
fsp_err_t internet_state_clear(void);
fsp_err_t internet_state_set(void);
fsp_err_t mqtt_state_set(void);
fsp_err_t wifi_state_set(void);
uint8_t internet_state_get(internet_state_t mode);


/*************************************************************
 * State Variables for AUSCULTATION
 ************************************************************/
typedef enum trueCheck_state{
    t_AUS_NONE = 0,
	t_AUS_MAN = 1,
	t_AUS_CWD = 2,
	t_AUS_STOP= 3,
	t_AUS_MAN_05 = 4,
	t_AUS_MAN_15 = 5,
	t_AUS_MAN_RESEND = 6,
	t_AUS_CWD_RESEND = 7,
}trueCheck_t;


trueCheck_t getTrueCheckVal(void);   // get TrueCheck State
trueCheck_t setTrueCheckVal(trueCheck_t val);// set TrueCheck State


/**************************************************************************************************************************
 * DATA PROCESS STATE TRANSITION
 **************************************************************************************************************************/
typedef enum dataprocess_state {
    AUDIO_SET0       = 10,/**< AUDIO_SET0 */
    AUDIO_SET1       = 11,/**< AUDIO_SET1 */
    AUDIO_SET2       = 12,/**< AUDIO_SET2 */
    AUDIO_SET3       = 13,/**< AUDIO_SET3 */
    AUDIO_DONE       = 14,/**< AUDIO_DONE */
    AUDIO_INIT       = 0, /**< AUDIO_INIT */
    AUDIO_ERR        = 1, /**< AUDIO_ERR */
}dataprocess_state_t;

/* API for publishing classifications */
float* getWz12dAddr(void);
void wz12_clear(void);
void wz12_set(dataprocess_state_t in_dataprocess_state,float inWZ);

/* API for reading and setting the AWS_CMD */
void g_awscmd_set(aws_cmd_t inCmd);
aws_cmd_t g_awscmd_get(void);
#endif /* SYSTEMFUNCTIONS_SYSTEMVARIABLES_H_ */
