/* generated thread source file - do not edit */
#include "ssidpw_thread.h"

#if 1
static StaticTask_t ssidpw_thread_memory;
#if defined(__ARMCC_VERSION)           /* AC6 compiler */
                static uint8_t ssidpw_thread_stack[1024] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX ".stack.thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
                #else
static uint8_t ssidpw_thread_stack[1024] BSP_PLACE_IN_SECTION(BSP_UNINIT_SECTION_PREFIX ".stack.ssidpw_thread") BSP_ALIGN_VARIABLE(BSP_STACK_ALIGNMENT);
#endif
#endif
TaskHandle_t ssidpw_thread;
void ssidpw_thread_create(void);
static void ssidpw_thread_func(void *pvParameters);
void rtos_startup_err_callback(void *p_instance, void *p_data);
void rtos_startup_common_init(void);
flash_hp_instance_ctrl_t g_flash_ctrl;
const flash_cfg_t g_flash_cfg =
{ .data_flash_bgo = true, .p_callback = bgo_callback, .p_context = NULL,
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
  .err_ipl = (2),
  .ipl = (2), };
/* Instance structure to use this module. */
const flash_instance_t g_flash =
{ .p_ctrl = &g_flash_ctrl, .p_cfg = &g_flash_cfg, .p_api = &g_flash_on_flash_hp };
extern uint32_t g_fsp_common_thread_count;

const rm_freertos_port_parameters_t ssidpw_thread_parameters =
{ .p_context = (void*) NULL, };

void ssidpw_thread_create(void)
{
    /* Increment count so we will know the number of threads created in the RA Configuration editor. */
    g_fsp_common_thread_count++;

    /* Initialize each kernel object. */

#if 1
    ssidpw_thread = xTaskCreateStatic (
#else
                    BaseType_t ssidpw_thread_create_err = xTaskCreate(
                    #endif
                                       ssidpw_thread_func,
                                       (const char*) "SSIDPW thread", 1024 / 4, // In words, not bytes
                                       (void*) &ssidpw_thread_parameters, //pvParameters
                                       4,
#if 1
                                       (StackType_t*) &ssidpw_thread_stack,
                                       (StaticTask_t*) &ssidpw_thread_memory
#else
                        & ssidpw_thread
                        #endif
                                       );

#if 1
    if (NULL == ssidpw_thread)
    {
        rtos_startup_err_callback (ssidpw_thread, 0);
    }
#else
                    if (pdPASS != ssidpw_thread_create_err)
                    {
                        rtos_startup_err_callback(ssidpw_thread, 0);
                    }
                    #endif
}
static void ssidpw_thread_func(void *pvParameters)
{
    /* Initialize common components */
    rtos_startup_common_init ();

    /* Initialize each module instance. */

#if (1 == BSP_TZ_NONSECURE_BUILD) && (1 == 1)
                    /* When FreeRTOS is used in a non-secure TrustZone application, portALLOCATE_SECURE_CONTEXT must be called prior
                     * to calling any non-secure callable function in a thread. The parameter is unused in the FSP implementation.
                     * If no slots are available then configASSERT() will be called from vPortSVCHandler_C(). If this occurs, the
                     * application will need to either increase the value of the "Process Stack Slots" Property in the rm_tz_context
                     * module in the secure project or decrease the number of threads in the non-secure project that are allocating
                     * a secure context. Users can control which threads allocate a secure context via the Properties tab when
                     * selecting each thread. Note that the idle thread in FreeRTOS requires a secure context so the application
                     * will need at least 1 secure context even if no user threads make secure calls. */
                     portALLOCATE_SECURE_CONTEXT(0);
                    #endif

    /* Enter user code for this thread. Pass task handle. */
    ssidpw_thread_entry (pvParameters);
}
