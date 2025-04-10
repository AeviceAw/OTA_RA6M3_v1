/* generated common source file - do not edit */
#include "common_data.h"

flash_hp_instance_ctrl_t g_flash1_ctrl;
const flash_cfg_t g_flash1_cfg =
{ .data_flash_bgo = false, .p_callback = NULL, .p_context = NULL,
#if defined(VECTOR_NUMBER_FCU_FRDYI)
    .irq                 = VECTOR_NUMBER_FCU_FRDYI,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_FCU_FIFERR)
    .err_irq             = VECTOR_NUMBER_FCU_FIFERR,
#else
  .err_irq = FSP_INVALID_VECTOR,
#endif
  .err_ipl = (BSP_IRQ_DISABLED),
  .ipl = (BSP_IRQ_DISABLED), };
/* Instance structure to use this module. */
const flash_instance_t g_flash1 =
{ .p_ctrl = &g_flash1_ctrl, .p_cfg = &g_flash1_cfg, .p_api = &g_flash_on_flash_hp };
void *const gp_mcuboot_flash_ctrl = &g_flash1_ctrl;
flash_cfg_t const *const gp_mcuboot_flash_cfg = &g_flash1_cfg;
flash_instance_t const *const gp_mcuboot_flash_instance = &g_flash1;

#ifndef vLoggingPrintf
#include <stdarg.h>

void vLoggingPrintf(const char *pcFormat, ...)
{
    FSP_PARAMETER_NOT_USED (pcFormat);
}
#endif

#ifndef vLoggingPrint
void vLoggingPrint(const char *pcFormat);

void vLoggingPrint(const char *pcFormat)
{
    FSP_PARAMETER_NOT_USED (pcFormat);
}
#endif
flash_hp_instance_ctrl_t g_flash0_ctrl;
const flash_cfg_t g_flash0_cfg =
{ .data_flash_bgo = false, .p_callback = NULL, .p_context = NULL,
#if defined(VECTOR_NUMBER_FCU_FRDYI)
    .irq                 = VECTOR_NUMBER_FCU_FRDYI,
#else
  .irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_FCU_FIFERR)
    .err_irq             = VECTOR_NUMBER_FCU_FIFERR,
#else
  .err_irq = FSP_INVALID_VECTOR,
#endif
  .err_ipl = (BSP_IRQ_DISABLED),
  .ipl = (BSP_IRQ_DISABLED), };
/* Instance structure to use this module. */
const flash_instance_t g_flash0 =
{ .p_ctrl = &g_flash0_ctrl, .p_cfg = &g_flash0_cfg, .p_api = &g_flash_on_flash_hp };
rm_littlefs_flash_instance_ctrl_t g_rm_littlefs0_ctrl;

#ifdef LFS_NO_MALLOC
static uint8_t g_rm_littlefs0_read[64];
static uint8_t g_rm_littlefs0_prog[64];
static uint8_t g_rm_littlefs0_lookahead[16];
#endif

struct lfs g_rm_littlefs0_lfs;

const struct lfs_config g_rm_littlefs0_lfs_cfg =
{ .context = &g_rm_littlefs0_ctrl,
  .read = &rm_littlefs_flash_read,
  .prog = &rm_littlefs_flash_write,
  .erase = &rm_littlefs_flash_erase,
  .sync = &rm_littlefs_flash_sync,
  .read_size = 1,
  .prog_size = 4,
  .block_size = 128,
  .block_count = (0x1000 / 128),
  .block_cycles = 1024,
  .cache_size = 64,
  .lookahead_size = 16,
#ifdef LFS_NO_MALLOC
    .read_buffer = (void *) g_rm_littlefs0_read,
    .prog_buffer = (void *) g_rm_littlefs0_prog,
    .lookahead_buffer = (void *) g_rm_littlefs0_lookahead,
#endif
#if LFS_THREAD_SAFE
    .lock   = &rm_littlefs_flash_lock,
    .unlock = &rm_littlefs_flash_unlock,
#endif
        };

const rm_littlefs_flash_cfg_t g_rm_littlefs0_ext_cfg =
{ .p_flash = &g_flash0, };

const rm_littlefs_cfg_t g_rm_littlefs0_cfg =
{ .p_lfs_cfg = &g_rm_littlefs0_lfs_cfg, .p_extend = &g_rm_littlefs0_ext_cfg, };

/* Instance structure to use this module. */
const rm_littlefs_instance_t g_rm_littlefs0 =
{ .p_ctrl = &g_rm_littlefs0_ctrl, .p_cfg = &g_rm_littlefs0_cfg, .p_api = &g_rm_littlefs_on_flash, };

dtc_instance_ctrl_t g_transfer0_ctrl;

#if (1 == 1)
transfer_info_t g_transfer0_info =
{ .transfer_settings_word_b.dest_addr_mode = TRANSFER_ADDR_MODE_FIXED,
  .transfer_settings_word_b.repeat_area = TRANSFER_REPEAT_AREA_SOURCE,
  .transfer_settings_word_b.irq = TRANSFER_IRQ_END,
  .transfer_settings_word_b.chain_mode = TRANSFER_CHAIN_MODE_DISABLED,
  .transfer_settings_word_b.src_addr_mode = TRANSFER_ADDR_MODE_INCREMENTED,
  .transfer_settings_word_b.size = TRANSFER_SIZE_1_BYTE,
  .transfer_settings_word_b.mode = TRANSFER_MODE_NORMAL,
  .p_dest = (void*) NULL,
  .p_src = (void const*) NULL,
  .num_blocks = 0,
  .length = 0, };

#elif (1 > 1)
/* User is responsible to initialize the array. */
transfer_info_t g_transfer0_info[1];
#else
/* User must call api::reconfigure before enable DTC transfer. */
#endif

const dtc_extended_cfg_t g_transfer0_cfg_extend =
{ .activation_source = VECTOR_NUMBER_SCI9_TXI, };

const transfer_cfg_t g_transfer0_cfg =
{
#if (1 == 1)
  .p_info = &g_transfer0_info,
#elif (1 > 1)
    .p_info              = g_transfer0_info,
#else
    .p_info = NULL,
#endif
  .p_extend = &g_transfer0_cfg_extend, };

/* Instance structure to use this module. */
const transfer_instance_t g_transfer0 =
{ .p_ctrl = &g_transfer0_ctrl, .p_cfg = &g_transfer0_cfg, .p_api = &g_transfer_on_dtc };
sci_uart_instance_ctrl_t g_uart0_ctrl;

baud_setting_t g_uart0_baud_setting =
        {
        /* Baud rate calculated with 1.725% error. */.semr_baudrate_bits_b.abcse = 0,
          .semr_baudrate_bits_b.abcs = 0, .semr_baudrate_bits_b.bgdm = 1, .cks = 0, .brr = 7, .mddr = (uint8_t) 256, .semr_baudrate_bits_b.brme =
                  false };

/** UART extended configuration for UARTonSCI HAL driver */
const sci_uart_extended_cfg_t g_uart0_cfg_extend =
{ .clock = SCI_UART_CLOCK_INT, .rx_edge_start = SCI_UART_START_BIT_FALLING_EDGE, .noise_cancel =
          SCI_UART_NOISE_CANCELLATION_DISABLE,
  .rx_fifo_trigger = SCI_UART_RX_FIFO_TRIGGER_MAX, .p_baud_setting = &g_uart0_baud_setting, .flow_control =
          SCI_UART_FLOW_CONTROL_RTS,
#if 0xFF != 0xFF
                .flow_control_pin       = BSP_IO_PORT_FF_PIN_0xFF,
                #else
  .flow_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
  .rs485_setting =
  { .enable = SCI_UART_RS485_DISABLE, .polarity = SCI_UART_RS485_DE_POLARITY_HIGH,
#if 0xFF != 0xFF
                    .de_control_pin = BSP_IO_PORT_FF_PIN_0xFF,
                #else
    .de_control_pin = (bsp_io_port_pin_t) UINT16_MAX,
#endif
          }, };

/** UART interface configuration */
const uart_cfg_t g_uart0_cfg =
{ .channel = 9, .data_bits = UART_DATA_BITS_8, .parity = UART_PARITY_OFF, .stop_bits = UART_STOP_BITS_1, .p_callback =
          rm_at_transport_da16xxx_uart_callback,
  .p_context = NULL, .p_extend = &g_uart0_cfg_extend,
#define RA_NOT_DEFINED (1)
#if (RA_NOT_DEFINED == g_transfer0)
                .p_transfer_tx       = NULL,
#else
  .p_transfer_tx = &g_transfer0,
#endif
#if (RA_NOT_DEFINED == RA_NOT_DEFINED)
  .p_transfer_rx = NULL,
#else
                .p_transfer_rx       = &RA_NOT_DEFINED,
#endif
#undef RA_NOT_DEFINED
  .rxi_ipl = (12),
  .txi_ipl = (12), .tei_ipl = (12), .eri_ipl = (12),
#if defined(VECTOR_NUMBER_SCI9_RXI)
                .rxi_irq             = VECTOR_NUMBER_SCI9_RXI,
#else
  .rxi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI9_TXI)
                .txi_irq             = VECTOR_NUMBER_SCI9_TXI,
#else
  .txi_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI9_TEI)
                .tei_irq             = VECTOR_NUMBER_SCI9_TEI,
#else
  .tei_irq = FSP_INVALID_VECTOR,
#endif
#if defined(VECTOR_NUMBER_SCI9_ERI)
                .eri_irq             = VECTOR_NUMBER_SCI9_ERI,
#else
  .eri_irq = FSP_INVALID_VECTOR,
#endif
        };

/* Instance structure to use this module. */
const uart_instance_t g_uart0 =
{ .p_ctrl = &g_uart0_ctrl, .p_cfg = &g_uart0_cfg, .p_api = &g_uart_on_sci };
const char *g_at_transport_da16xxx_uart_cmd_baud = "921600";
at_transport_da16xxx_instance_ctrl_t g_at_transport_da16xxx_ctrl;
const at_transport_da16xxx_extended_cfg_t g_at_transport_da16xxx_cfg_extend =
{
#define RA_NOT_DEFINED (1)
  .num_uarts = 1,
  .reset_pin = AT_TRANSPORT_DA16XXX_CFG_RESET_PIN, .uart_instances =
  {

#if (RA_NOT_DEFINED == g_uart0)
            NULL,
        #else
    &g_uart0,
#endif

    NULL,

#undef RA_NOT_DEFINED
          } };

at_transport_da16xxx_cfg_t g_at_transport_da16xxx_cfg =
{ .p_extend = &g_at_transport_da16xxx_cfg_extend, .p_callback = rm_wifi_da16xxx_callback, };

const at_transport_da16xxx_instance_t g_at_transport_da16xxx =
        { .p_ctrl = &g_at_transport_da16xxx_ctrl, .p_cfg = &g_at_transport_da16xxx_cfg, .p_api =
                  &g_at_transport_da16xxx_on_uart, };
/** WiFi DA16XXX configuration */
const wifi_da16xxx_cfg_t g_wifi_da16xxx_cfg =
{ .p_transport_instance = &g_at_transport_da16xxx,
  .num_sockets = WIFI_DA16XXX_CFG_NUM_CREATEABLE_SOCKETS,
  .country_code = (uint8_t*) "JP",
  .sntp_server_ip = (uint8_t*) "0.0.0.0",
  .sntp_utc_offset_in_hours = 0, };
/*
 * Structure used for encoding firmware version.
 */
const AppVersion32_t appFirmwareVersion =
        { .u.x.major = otaconfigOTA_VERION_MAJOR, .u.x.minor = otaconfigOTA_VERION_MINOR, .u.x.build =
                  otaconfigOTA_VERION_BUILD, };
#if BSP_FEATURE_CGC_HAS_OSTDCSE
const cgc_extended_cfg_t g_cgc0_cfg_extend =
{
    .ostd_enable = (1),
    .mostd_enable = (0),
    .sostd_enable = (0),
#if defined(VECTOR_NUMBER_CGC_MOSTD_STOP)
    .mostd_irq            = VECTOR_NUMBER_CGC_MOSTD_STOP,
#else
    .mostd_irq            = FSP_INVALID_VECTOR,
#endif
    .mostd_ipl            = (BSP_IRQ_DISABLED),
#if defined(VECTOR_NUMBER_CGC_SOSC_STOP)
    .sostd_irq            = VECTOR_NUMBER_CGC_SOSC_STOP,
#else
    .sostd_irq            = FSP_INVALID_VECTOR,
#endif
    .sostd_ipl            = (BSP_IRQ_DISABLED),
    .sdadc_b_clock_switch_enable = (0),
    .mostd_detection_time = 0,
    .sostd_detection_time = 0,
};
#endif

#if BSP_TZ_NONSECURE_BUILD
 #if defined(BSP_CFG_CLOCKS_SECURE) && BSP_CFG_CLOCKS_SECURE
  #error "The CGC registers are only accessible in the TrustZone Secure Project."
 #endif
#endif

const cgc_cfg_t g_cgc0_cfg =
{ .p_callback = NULL,
#if BSP_FEATURE_CGC_HAS_OSTDCSE
    .p_extend   = &g_cgc0_cfg_extend,
#else
  .p_extend = NULL,
#endif
        };

cgc_instance_ctrl_t g_cgc0_ctrl;
const cgc_instance_t g_cgc0 =
{ .p_api = &g_cgc_on_cgc, .p_ctrl = &g_cgc0_ctrl, .p_cfg = &g_cgc0_cfg, };
ioport_instance_ctrl_t g_ioport_ctrl;
const ioport_instance_t g_ioport =
{ .p_api = &g_ioport_on_ioport, .p_ctrl = &g_ioport_ctrl, .p_cfg = &g_bsp_pin_cfg, };

QueueHandle_t g_topic_queue;
#if 1
StaticQueue_t g_topic_queue_memory;
uint8_t g_topic_queue_queue_memory[65 * 16];
#endif
void rtos_startup_err_callback(void *p_instance, void *p_data);
SemaphoreHandle_t g_console_out_mutex;
#if 1
StaticSemaphore_t g_console_out_mutex_memory;
#endif
void rtos_startup_err_callback(void *p_instance, void *p_data);
QueueHandle_t g_hs3001_queue;
#if 1
StaticQueue_t g_hs3001_queue_memory;
uint8_t g_hs3001_queue_queue_memory[8 * 1];
#endif
void rtos_startup_err_callback(void *p_instance, void *p_data);
QueueHandle_t g_iaq_queue;
#if 1
StaticQueue_t g_iaq_queue_memory;
uint8_t g_iaq_queue_queue_memory[12 * 1];
#endif
void rtos_startup_err_callback(void *p_instance, void *p_data);
QueueHandle_t g_oaq_queue;
#if 1
StaticQueue_t g_oaq_queue_memory;
uint8_t g_oaq_queue_queue_memory[4 * 1];
#endif
void rtos_startup_err_callback(void *p_instance, void *p_data);
QueueHandle_t g_icm_queue;
#if 1
StaticQueue_t g_icm_queue_memory;
uint8_t g_icm_queue_queue_memory[72 * 1];
#endif
void rtos_startup_err_callback(void *p_instance, void *p_data);
QueueHandle_t g_icp_queue;
#if 1
StaticQueue_t g_icp_queue_memory;
uint8_t g_icp_queue_queue_memory[16 * 1];
#endif
void rtos_startup_err_callback(void *p_instance, void *p_data);
QueueHandle_t g_ob1203_queue;
#if 1
StaticQueue_t g_ob1203_queue_memory;
uint8_t g_ob1203_queue_queue_memory[10 * 1];
#endif
void rtos_startup_err_callback(void *p_instance, void *p_data);
void g_common_init(void)
{
    g_topic_queue =
#if 1
            xQueueCreateStatic (
#else
                xQueueCreate(
                #endif
                                16,
                                65
#if 1
                                ,
                                &g_topic_queue_queue_memory[0], &g_topic_queue_memory
#endif
                                );
    if (NULL == g_topic_queue)
    {
        rtos_startup_err_callback (g_topic_queue, 0);
    }
    g_console_out_mutex =
#if 0
                #if 1
                xSemaphoreCreateRecursiveMutexStatic(&g_console_out_mutex_memory);
                #else
                xSemaphoreCreateRecursiveMutex();
                #endif
                #else
#if 1
            xSemaphoreCreateMutexStatic (&g_console_out_mutex_memory);
#else
                xSemaphoreCreateMutex();
                #endif
#endif
    if (NULL == g_console_out_mutex)
    {
        rtos_startup_err_callback (g_console_out_mutex, 0);
    }
    g_hs3001_queue =
#if 1
            xQueueCreateStatic (
#else
                xQueueCreate(
                #endif
                                1,
                                8
#if 1
                                ,
                                &g_hs3001_queue_queue_memory[0], &g_hs3001_queue_memory
#endif
                                );
    if (NULL == g_hs3001_queue)
    {
        rtos_startup_err_callback (g_hs3001_queue, 0);
    }
    g_iaq_queue =
#if 1
            xQueueCreateStatic (
#else
                xQueueCreate(
                #endif
                                1,
                                12
#if 1
                                ,
                                &g_iaq_queue_queue_memory[0], &g_iaq_queue_memory
#endif
                                );
    if (NULL == g_iaq_queue)
    {
        rtos_startup_err_callback (g_iaq_queue, 0);
    }
    g_oaq_queue =
#if 1
            xQueueCreateStatic (
#else
                xQueueCreate(
                #endif
                                1,
                                4
#if 1
                                ,
                                &g_oaq_queue_queue_memory[0], &g_oaq_queue_memory
#endif
                                );
    if (NULL == g_oaq_queue)
    {
        rtos_startup_err_callback (g_oaq_queue, 0);
    }
    g_icm_queue =
#if 1
            xQueueCreateStatic (
#else
                xQueueCreate(
                #endif
                                1,
                                72
#if 1
                                ,
                                &g_icm_queue_queue_memory[0], &g_icm_queue_memory
#endif
                                );
    if (NULL == g_icm_queue)
    {
        rtos_startup_err_callback (g_icm_queue, 0);
    }
    g_icp_queue =
#if 1
            xQueueCreateStatic (
#else
                xQueueCreate(
                #endif
                                1,
                                16
#if 1
                                ,
                                &g_icp_queue_queue_memory[0], &g_icp_queue_memory
#endif
                                );
    if (NULL == g_icp_queue)
    {
        rtos_startup_err_callback (g_icp_queue, 0);
    }
    g_ob1203_queue =
#if 1
            xQueueCreateStatic (
#else
                xQueueCreate(
                #endif
                                1,
                                10
#if 1
                                ,
                                &g_ob1203_queue_queue_memory[0], &g_ob1203_queue_memory
#endif
                                );
    if (NULL == g_ob1203_queue)
    {
        rtos_startup_err_callback (g_ob1203_queue, 0);
    }
}
