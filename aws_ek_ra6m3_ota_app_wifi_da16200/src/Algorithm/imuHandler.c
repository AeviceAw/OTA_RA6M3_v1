/*
 * imuHandler.c
 *
 *  Created on: 14 Jun 2024
 *      Author: yyang
 */

#include "initialisations.h"

//IMU Internal Variables
imu_status_t g_imuOnbody = IMU_INIT;//System variable
imu_status_t imu_onbody_prev = IMU_INIT;//System variable
static bool imu_cmd = false;//triggered by external subscribe

/************************************************************************************************************
 *  @brief      Check the <<IMU_ONBODY>>: Set -1 HR/RR/WZ_DISPLAY IF False
 *  @retval     N.A
 ************************************************************************************************************/
void imu_detection(void) {
    if(imu_cmd_get()) {
        //Force to ON_BODY always
        imu_onbody_set(IMU_ON_BODY);
    } // end of (CMD_IMU_ALWAYS_ON)
    else {
        /* IMU Check for onbody (movement == onbody) */
        if(imu_onbody_get() == IMU_OFF_BODY)      {
            vitals_set(HR_DISPLAY,-1.0f);
            vitals_set(RR_DISPLAY,-1.0f);
            vitals_set(WZ_DISPLAY,-1.0f);
        } // end if (IMU_OFF_BODY)
    } // end if (!CMD_IMU_ALWAYS_ON)
} // end imu_detection()


/************************************************************************************************************
 *  @brief      Get the current value of global variable <<g_imuOnbody>>
 *  @retval     imu_status_t IMU_INIT/ONBODY/OFFBODY
 ************************************************************************************************************/
imu_status_t imu_onbody_get(void) {
    return g_imuOnbody;
} // end imu_onbody_get()

/************************************************************************************************************
 *  @brief      Set the current value of global variable <<g_imuOnbody>> and save the previous value
 *  @param[in   imu_status_t inMovement     IMU_INIT/ONBODY/OFFBODY
 *  @retval     N.A.
 ************************************************************************************************************/
void imu_onbody_set(imu_status_t inMovement) {
    if(g_imuOnbody != inMovement) {
        //state change
        imu_onbody_prev = g_imuOnbody;//store last change
    } // if (IMU_ONBODY_PRVE!= inMovement)
    g_imuOnbody = inMovement;
} // end imu_onbody_set()


/************************************************************************************************************
 *  @brief      Set the CMD value for <<imu_cmd>>
 *  @param[in]  bool  FORCE_ONBODY or IMU_ONBODY
 *  @retval     N.A.
 ************************************************************************************************************/
void imu_cmd_set(bool setState) {
    imu_cmd = setState;
} // end imu_cmd_set()

/************************************************************************************************************
 *  @brief      Get the CMD value for <<imu_cmd>>
 *  @param[in]  N.A.
 *  @retval     bool  FORCE_ONBODY or IMU_ONBODY
 ************************************************************************************************************/
bool imu_cmd_get(void) {
    return imu_cmd;
} // end imu_cmd_get()
