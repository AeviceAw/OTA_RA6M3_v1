/* generated common header file - do not edit */
#ifndef COMMON_DATA_H_
#define COMMON_DATA_H_
#include <stdint.h>
#include "bsp_api.h"
#include "FreeRTOS.h"
#include "queue.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "FreeRTOS.h"
#include "semphr.h"
#include "FreeRTOS.h"
#include "event_groups.h"
#include "r_icu.h"
#include "r_external_irq_api.h"

#include "r_flash_hp.h"
#include "r_flash_api.h"
#include "bootutil/bootutil.h"

#include "aws_dev_mode_key_provisioning.h"
#include "rm_littlefs_api.h"
#include "rm_littlefs_flash.h"
#include "lfs_util.h"
#include "lfs_util.h"
#include "mbedtls/platform.h"
#include "psa/crypto.h"
#include "psa/crypto_extra.h"
#include "rm_psa_crypto.h"
#include "r_dtc.h"
#include "r_transfer_api.h"
#include "r_sci_uart.h"
#include "r_uart_api.h"
#include "rm_at_transport_da16xxx_uart.h"
#include "rm_at_transport_da16xxx.h"
#include "rm_wifi_da16xxx.h"
#include "tcp_sockets_wrapper.h"
#include "transport_mbedtls_pkcs11.h"
#include "core_mqtt.h"
#include "ota_config.h"
#include "ota_appversion32.h"
#include "r_cgc.h"
#include "r_cgc_api.h"
#include "r_ioport.h"
#include "bsp_pin_cfg.h"
FSP_HEADER
/** External IRQ on ICU Instance. */
extern const external_irq_instance_t g_external_irq1;

/** Access the ICU instance using these structures when calling API functions directly (::p_api is not used). */
extern icu_instance_ctrl_t g_external_irq1_ctrl;
extern const external_irq_cfg_t g_external_irq1_cfg;

#ifndef external_irq1_callback
void external_irq1_callback(external_irq_callback_args_t *p_args);
#endif
/* Flash on Flash HP Instance */
extern const flash_instance_t g_flash1;

/** Access the Flash HP instance using these structures when calling API functions directly (::p_api is not used). */
extern flash_hp_instance_ctrl_t g_flash1_ctrl;
extern const flash_cfg_t g_flash1_cfg;

#ifndef NULL
void NULL(flash_callback_args_t *p_args);
#endif
void mcuboot_image_utilities_quick_setup();

/* Flash on Flash HP Instance */
extern const flash_instance_t g_flash0;

/** Access the Flash HP instance using these structures when calling API functions directly (::p_api is not used). */
extern flash_hp_instance_ctrl_t g_flash0_ctrl;
extern const flash_cfg_t g_flash0_cfg;

#ifndef NULL
void NULL(flash_callback_args_t *p_args);
#endif
/** LittleFS on Flash Instance. */
extern const rm_littlefs_instance_t g_rm_littlefs0;
extern rm_littlefs_flash_instance_ctrl_t g_rm_littlefs0_ctrl;
extern const rm_littlefs_cfg_t g_rm_littlefs0_cfg;

extern struct lfs g_rm_littlefs0_lfs;
extern const struct lfs_config g_rm_littlefs0_lfs_cfg;

/* Transfer on DTC Instance. */
extern const transfer_instance_t g_transfer0;

/** Access the DTC instance using these structures when calling API functions directly (::p_api is not used). */
extern dtc_instance_ctrl_t g_transfer0_ctrl;
extern const transfer_cfg_t g_transfer0_cfg;
/** UART on SCI Instance. */
extern const uart_instance_t g_uart0;

/** Access the UART instance using these structures when calling API functions directly (::p_api is not used). */
extern sci_uart_instance_ctrl_t g_uart0_ctrl;
extern const uart_cfg_t g_uart0_cfg;
extern const sci_uart_extended_cfg_t g_uart0_cfg_extend;

#ifndef rm_at_transport_da16xxx_uart_callback
void rm_at_transport_da16xxx_uart_callback(uart_callback_args_t *p_args);
#endif
extern const char *g_at_transport_da16xxx_uart_cmd_baud;
extern at_transport_da16xxx_instance_ctrl_t g_at_transport_da16xxx_ctrl;
extern at_transport_da16xxx_cfg_t g_at_transport_da16xxx_cfg;
extern const at_transport_da16xxx_instance_t g_at_transport_da16xxx;
extern const wifi_da16xxx_cfg_t g_wifi_da16xxx_cfg;
/** CGC Instance */
extern const cgc_instance_t g_cgc0;

/** Access the CGC instance using these structures when calling API functions directly (::p_api is not used). */
extern cgc_instance_ctrl_t g_cgc0_ctrl;
extern const cgc_cfg_t g_cgc0_cfg;

#ifndef NULL
void NULL(cgc_callback_args_t *p_args);
#endif
#define IOPORT_CFG_NAME g_bsp_pin_cfg
#define IOPORT_CFG_OPEN R_IOPORT_Open
#define IOPORT_CFG_CTRL g_ioport_ctrl

/* IOPORT Instance */
extern const ioport_instance_t g_ioport;

/* IOPORT control structure. */
extern ioport_instance_ctrl_t g_ioport_ctrl;
extern QueueHandle_t g_topic_queue;
extern SemaphoreHandle_t g_console_out_mutex;
extern QueueHandle_t g_hs3001_queue;
extern QueueHandle_t g_iaq_queue;
extern QueueHandle_t g_oaq_queue;
extern QueueHandle_t g_icm_queue;
extern QueueHandle_t g_icp_queue;
extern QueueHandle_t g_ob1203_queue;
extern SemaphoreHandle_t g_data_semaphore;
extern SemaphoreHandle_t g_wifi_semaphore;
extern SemaphoreHandle_t g_rgb_semaphore;
extern SemaphoreHandle_t g_publish_semaphore;
extern EventGroupHandle_t g_wifi_event;
extern EventGroupHandle_t g_sync_event;
extern SemaphoreHandle_t g_ota_semaphore;
void g_common_init(void);
FSP_FOOTER
#endif /* COMMON_DATA_H_ */
