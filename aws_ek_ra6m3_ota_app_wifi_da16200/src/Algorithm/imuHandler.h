/*
 * imuHandler.h
 *
 *  Created on: 14 Jun 2024
 *      Author: yyang
 */

#ifndef ALGORITHM_IMUHANDLER_H_
#define ALGORITHM_IMUHANDLER_H_

#include "initialisations.h"

//typedef enum imu_status_enum {
//    IMU_INIT        = 0,
//    IMU_ON_BODY     = 1,
//    IMU_OFF_BODY    = 2,
//} imu_status_t;

/* Prototype function to set -1 for HR/RR/WZ_Display values if !IMU_ONBODY */
void imu_detection(void);

/* Prototype function to Get/Set g_variable for <<imu_onbody>>  */
imu_status_t imu_onbody_get(void);
void imu_onbody_set(imu_status_t inMovement);

/* Prototype functions to actuate CMD from Subscribe Thread */
void imu_cmd_set(bool setState);
bool imu_cmd_get(void);

#endif /* ALGORITHM_IMUHANDLER_H_ */
