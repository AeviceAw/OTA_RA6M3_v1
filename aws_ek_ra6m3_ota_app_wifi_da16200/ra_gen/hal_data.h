/* generated HAL header file - do not edit */
#ifndef HAL_DATA_H_
#define HAL_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "common_data.h"
#include "r_rtc.h"
#include "r_rtc_api.h"
#include "r_iwdt.h"
#include "r_wdt_api.h"
#include "r_adc.h"
#include "r_adc_api.h"
#include "r_dtc.h"
#include "r_transfer_api.h"
#include "r_sci_spi.h"
#include "r_spi_api.h"
#include "r_iic_master.h"
#include "r_i2c_master_api.h"
#include "r_crc.h"
#include "r_crc_api.h"
#include "r_gpt.h"
#include "r_timer_api.h"
FSP_HEADER
/* RTC Instance. */
extern const rtc_instance_t g_rtc;

/** Access the RTC instance using these structures when calling API functions directly (::p_api is not used). */
extern rtc_instance_ctrl_t g_rtc_ctrl;
extern const rtc_cfg_t g_rtc_cfg;

#ifndef rtc_callback
void rtc_callback(rtc_callback_args_t *p_args);
#endif
/** WDT on IWDT Instance. */
extern const wdt_instance_t g_iwdt;

/** Access the IWDT instance using these structures when calling API functions directly (::p_api is not used). */
extern iwdt_instance_ctrl_t g_iwdt_ctrl;
extern const wdt_cfg_t g_iwdt_cfg;

#ifndef NULL
void NULL(wdt_callback_args_t *p_args);
#endif
/** ADC on ADC Instance. */
extern const adc_instance_t g_adc_current_detect;

/** Access the ADC instance using these structures when calling API functions directly (::p_api is not used). */
extern adc_instance_ctrl_t g_adc_current_detect_ctrl;
extern const adc_cfg_t g_adc_current_detect_cfg;
extern const adc_channel_cfg_t g_adc_current_detect_channel_cfg;

#ifndef adc_callback
void adc_callback(adc_callback_args_t *p_args);
#endif

#ifndef NULL
#define ADC_DMAC_CHANNELS_PER_BLOCK_NULL  0
#endif
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer2;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer2_ctrl;
extern const transfer_cfg_t g_transfer2_cfg;
/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer1;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer1_ctrl;
extern const transfer_cfg_t g_transfer1_cfg;
/** SPI on SCI Instance. */
extern const spi_instance_t g_sram;

/** Access the SCI_SPI instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_spi_instance_ctrl_t g_sram_ctrl;
extern const spi_cfg_t g_sram_cfg;

/** Called by the driver when a transfer has completed or an error has occurred (Must be implemented by the user). */
#ifndef sram_callback
void sram_callback(spi_callback_args_t *p_args);
#endif
/* I2C Master on IIC Instance. */
extern const i2c_master_instance_t g_i2c_rgb_master;

/** Access the I2C Master instance using these structures when calling API functions directly (::p_api is not used). */
extern iic_master_instance_ctrl_t g_i2c_rgb_master_ctrl;
extern const i2c_master_cfg_t g_i2c_rgb_master_cfg;

#ifndef i2c_rgb_callback
void i2c_rgb_callback(i2c_master_callback_args_t *p_args);
#endif
extern const crc_instance_t g_crc;
extern crc_instance_ctrl_t g_crc_ctrl;
extern const crc_cfg_t g_crc_cfg;
/** Timer on GPT Instance. */
extern const timer_instance_t g_timer1;

/** Access the GPT instance using these structures when calling API functions directly (::p_api is not used). */
extern gpt_instance_ctrl_t g_timer1_ctrl;
extern const timer_cfg_t g_timer1_cfg;

#ifndef g_user_timer_cb
void g_user_timer_cb(timer_callback_args_t *p_args);
#endif
void hal_entry(void);
void g_hal_init(void);
FSP_FOOTER
#endif /* HAL_DATA_H_ */
