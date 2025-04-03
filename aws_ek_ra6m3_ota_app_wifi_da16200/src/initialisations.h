/*
 * initialisations.h
 *
 *  Created on: 12 Nov 2024
 *      Author: awlir
 */

#ifndef INITIALISATIONS_H_
#define INITIALISATIONS_H_

#define MMP_MAJOR_VERSION      (1)
#define MMP_MINOR_VERSION      (0)
#define MMP_PATCH              (0)

#include "hal_data.h"
#include "bsp_pin_cfg.h"
#include "r_ioport.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <arm_math_types.h>
#include <arm_const_structs.h>

#include "hal_data.h"

#include "dataprocess_thread.h"
#include "ota_thread.h"
#include "publish_thread.h"
#include "rgb_thread.h"
#include "ssidpw_thread.h"
#include "subscribe_thread.h"
#include "uart_thread.h"
#include "wifi_thread.h"
#include "event_groups.h"

#include "System/eventFlags.h"
#include "System/RTC_Timer.h"
#include "System/RTT_Logger.h"
#include "System/SystemVariables.h"

#include "Thread_RGB/iwdtHandler.h"
#include "Thread_RGB/rgbHandler.h"

#include "aws.h"
#include "Algorithm\batteryHandler.h"
#include "Algorithm\data.h"
#include "Algorithm\dataHandler.h"
#include "Algorithm\crcHandler.h"
#include "Algorithm\imuHandler.h"
#include "Algorithm\DSP\vitalsHandler.h"
#include "BGX/bleHandler.h"
#include "BGX/uartHandler.h"
#include "common_utils.h"
#include "Thread_RGB/currentDetect_adc.h"
#include "Thread_RGB/iwdtHandler.h"
#include "Thread_RGB/rgbHandler.h"
#include "Dataflash/DataFlash.h"
#include "DA16x00MOD/wifiHandler.h"
#include "Thread_SSIDPW/ssidpwHandler.h"
#include "Thread_UART/uartTimers.h"
#include "Thread_Dataprocess/dataprocessHandler.h"
#include "Thread_Subscribe/subscribeHandler.h"
#include "Thread_Publish/publishAusTester.h"
#include "Thread_Publish/sramHandler.h"
#include "Thread_Publish/publishHandler.h"

#include "ota.h"

#endif /* INITIALISATIONS_H_ */
