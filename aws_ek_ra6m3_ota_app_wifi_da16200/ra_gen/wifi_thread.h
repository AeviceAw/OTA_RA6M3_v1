/* generated thread header file - do not edit */
#ifndef WIFI_THREAD_H_
#define WIFI_THREAD_H_
#include "bsp_api.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void wifi_thread_entry(void * pvParameters);
                #else
extern void wifi_thread_entry(void *pvParameters);
#endif
#include "r_flash_hp.h"
#include "r_flash_api.h"
FSP_HEADER
/* Flash on Flash HP Instance */
extern const flash_instance_t user_flash;

/** Access the Flash HP instance using these structures when calling API functions directly (::p_api is not used). */
extern flash_hp_instance_ctrl_t user_flash_ctrl;
extern const flash_cfg_t user_flash_cfg;

#ifndef flash_callback
void flash_callback(flash_callback_args_t *p_args);
#endif
FSP_FOOTER
#endif /* WIFI_THREAD_H_ */
