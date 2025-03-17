/*
 * dataprocessHandler.h
 *
 *  Created on: 7 Aug 2024
 *      Author: yyang
 */

#include "Initialisations.h"

/* CWD Timer */
#define MAX_CWD_COUNT 2
typedef enum cwd_state_enum{
    CWD_OFF=0,
    CWD_ON =1,
    CWD_INIT=2
}cwd_state_t;

typedef enum aus_timer_state_enum{
    AUS_TIMER_OFF=0,
    AUS_TIMER_ON =1,
    AUS_TIMER_INIT=2
}aus_timer_state_t;

/* Options for vpt_setup()*/
typedef enum vpt_setup_options{
    VPT_CLOSE = 0,
    VPT_STOP =1,
    VPT_DISABLE = 2,
    VPT_RESET = 3,
    VPT_OPEN = 4,
    VPT_START = 5,
    VPT_ENABLE = 6
}vpt_setup_t;

/* VPT Flag to indicate whether its ON or OFF*/
typedef enum vpt_status {
    // Output for g_vpt
    VPT_OFF = 0,
    VPT_ON  = 1,
    VPT_INIT= 2,
} vpt_status_t;

/* Values inside vpt_threshold[vpt_thres_t] */
#define NUM_VPT_STATES 6
typedef enum vpt_thres_val{
    SEC_ONE = 0,
    SEC_FIVE = 1,
    SEC_SEVEN = 2,
    SEC_NINE =3,
    SEC_TWELVE = 4,
    SEC_FIFTEEN = 5
//    SEC_THREE = 0,
//    SEC_EIGHT = 1,
//    SEC_TEN = 2,
//    SEC_TWELVE =3,
//    SEC_THREETEEN = 4,
//    SEC_FIFTEEN = 5
}vpt_thres_t;


/* Prototype to Write Vital values to vpt buffer */
void vpt_buf_write(void);

/* Prototype to setup  vital publish timer (VPT)  */
fsp_err_t vpt_timer_setup(vpt_setup_t opts);
vpt_status_t vpt_state_get(void);
fsp_err_t vpt_timer_off(void);

/* Prototype to process vital publish timer (VPT) Threshold  */
bool vph_cross_threshold(uint32_t tick_sec,uint32_t tick_thres);
bool vph_thresState_add(void) ; // ++Index of <<g_vpt_threshold>>
void vph_thresState_clear(void);// clear Index of <<g_vpt_threshold>>
vpt_thres_t vph_thresState_get(void); // get Index of <<vph_thresState_add>>

/* Dataprocess State Machine */
dataprocess_state_t dataprocess_state_get(void);
void dataprocess_state_set(dataprocess_state_t in_dataprocess_state);
void dataprocess_state_update(blec_state_t in_blec_State, trueCheck_t in_true_check, dataprocess_state_t in_dataprocess_state, vpt_status_t in_vpt_state,cwd_state_t inCwdState);

/* Handlers for AUSCULTATION MANUAL  */
void adpcm_encode_decode(void);
void publishAuscultation (void);

/* Handlers for AUSCULTATION CWD */
bool inspectCwdNextIterStartCondition(dataprocess_state_t inData_t,cwd_state_t inCwdState,blec_state_t inBlec_t, uint8_t inBle, float inHR,float inWz);
bool inspectCwdContinueCondition(dataprocess_state_t inData_t,cwd_state_t inCwdState,blec_state_t inBlec_t, uint8_t inBle, float inHR,float inWz);
bool inspectCwdTurnOffCondition(uint8_t in_cwdCount);
uint8_t cwd_count_get(void);
void cwd_state_set(cwd_state_t inState);
cwd_state_t cwd_state_get(void);
void aus_timer_state_set(aus_timer_state_t inState);
aus_timer_state_t aus_timer_state_get(void);
