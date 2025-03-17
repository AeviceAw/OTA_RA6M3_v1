/*
 * batteryHandler.h
 *
 *  Created on: 14 Jun 2024
 *      Author: awlir
 */

#ifndef ALGORITHM_BATTERYHANDLER_H_
#define ALGORITHM_BATTERYHANDLER_H_

#include "initialisations.h"


/* Global Variables for Battery Information */
extern float batt;  // Battery Voltage
extern float bp;    // Battery Percentage


void batt_set(float inBatt);

float batt_get(void);
float batt_percentage_get(void);

void batt_publish_set(float inBatt);
void bp_publish_set(float inBp) ;

float batt_publish_get(void);
float bp_publish_get(void);

void bp_mu_get(float* bp_in, float* bp_out);
void bp_get(float* inBatt, float* outBp);

#endif /* ALGORITHM_BATTERYHANDLER_H_ */
