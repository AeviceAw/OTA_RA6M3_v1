/* generated thread header file - do not edit */
#ifndef DATAPROCESS_THREAD_H_
#define DATAPROCESS_THREAD_H_
#include "bsp_api.h"
#include "FreeRTOS.h"
#include "task.h"
#include "semphr.h"
#include "hal_data.h"
#ifdef __cplusplus
                extern "C" void dataprocess_thread_entry(void * pvParameters);
                #else
extern void dataprocess_thread_entry(void *pvParameters);
#endif
#include "r_gpt.h"
#include "r_timer_api.h"
FSP_HEADER
/** Timer on GPT Instance. */
extern const timer_instance_t g_cwd_timer;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_cwd_timer_ctrl;
extern const timer_cfg_t g_cwd_timer_cfg;

#ifndef g_cwd_timer_callback
void g_cwd_timer_callback(timer_callback_args_t *p_args);
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_audioResend_timer;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_audioResend_timer_ctrl;
extern const timer_cfg_t g_audioResend_timer_cfg;

#ifndef g_audioResend_callback
void g_audioResend_callback(timer_callback_args_t *p_args);
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_aus_timer;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_aus_timer_ctrl;
extern const timer_cfg_t g_aus_timer_cfg;

#ifndef g_aus_timer_callback
void g_aus_timer_callback(timer_callback_args_t *p_args);
#endif
/** Timer on GPT Instance. */
extern const timer_instance_t g_vitals_pub_timer;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_vitals_pub_timer_ctrl;
extern const timer_cfg_t g_vitals_pub_timer_cfg;

#ifndef g_vitals_pub_timer_callback
void g_vitals_pub_timer_callback(timer_callback_args_t *p_args);
#endif
FSP_FOOTER
#endif /* DATAPROCESS_THREAD_H_ */
