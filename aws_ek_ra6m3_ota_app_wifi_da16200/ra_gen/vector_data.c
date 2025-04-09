/* generated vector source file - do not edit */
#include "bsp_api.h"
/* Do not build these data structures if no interrupts are currently allocated because IAR will have build errors. */
#if VECTOR_DATA_IRQ_COUNT > 0
        BSP_DONT_REMOVE const fsp_vector_t g_vector_table[BSP_ICU_VECTOR_MAX_ENTRIES] BSP_PLACE_IN_SECTION(BSP_SECTION_APPLICATION_VECTORS) =
        {
                        [0] = gpt_counter_overflow_isr, /* GPT1 COUNTER OVERFLOW (Overflow) */
            [1] = iic_master_rxi_isr, /* IIC0 RXI (Receive data full) */
            [2] = iic_master_txi_isr, /* IIC0 TXI (Transmit data empty) */
            [3] = iic_master_tei_isr, /* IIC0 TEI (Transmit end) */
            [4] = iic_master_eri_isr, /* IIC0 ERI (Transfer error) */
            [5] = sci_spi_rxi_isr, /* SCI6 RXI (Receive data full) */
            [6] = sci_spi_txi_isr, /* SCI6 TXI (Transmit data empty) */
            [7] = sci_spi_tei_isr, /* SCI6 TEI (Transmit end) */
            [8] = sci_spi_eri_isr, /* SCI6 ERI (Receive error) */
            [9] = adc_scan_end_isr, /* ADC0 SCAN END (End of A/D scanning operation) */
            [10] = rtc_alarm_periodic_isr, /* RTC PERIOD (Periodic interrupt) */
            [11] = rtc_carry_isr, /* RTC CARRY (Carry interrupt) */
            [12] = sci_uart_rxi_isr, /* SCI9 RXI (Receive data full) */
            [13] = sci_uart_txi_isr, /* SCI9 TXI (Transmit data empty) */
            [14] = sci_uart_tei_isr, /* SCI9 TEI (Transmit end) */
            [15] = sci_uart_eri_isr, /* SCI9 ERI (Receive error) */
            [16] = fcu_frdyi_isr, /* FCU FRDYI (Flash ready interrupt) */
            [17] = fcu_fiferr_isr, /* FCU FIFERR (Flash access error interrupt) */
            [18] = sci_uart_rxi_isr, /* SCI8 RXI (Receive data full) */
            [19] = sci_uart_txi_isr, /* SCI8 TXI (Transmit data empty) */
            [20] = sci_uart_tei_isr, /* SCI8 TEI (Transmit end) */
            [21] = sci_uart_eri_isr, /* SCI8 ERI (Receive error) */
            [22] = gpt_counter_overflow_isr, /* GPT0 COUNTER OVERFLOW (Overflow) */
            [23] = gpt_counter_overflow_isr, /* GPT2 COUNTER OVERFLOW (Overflow) */
            [24] = gpt_counter_overflow_isr, /* GPT3 COUNTER OVERFLOW (Overflow) */
            [25] = r_icu_isr, /* ICU IRQ1 (External pin interrupt 1) */
        };
        #if BSP_FEATURE_ICU_HAS_IELSR
        const bsp_interrupt_event_t g_interrupt_event_link_select[BSP_ICU_VECTOR_MAX_ENTRIES] =
        {
            [0] = BSP_PRV_VECT_ENUM(EVENT_GPT1_COUNTER_OVERFLOW,GROUP0), /* GPT1 COUNTER OVERFLOW (Overflow) */
            [1] = BSP_PRV_VECT_ENUM(EVENT_IIC0_RXI,GROUP1), /* IIC0 RXI (Receive data full) */
            [2] = BSP_PRV_VECT_ENUM(EVENT_IIC0_TXI,GROUP2), /* IIC0 TXI (Transmit data empty) */
            [3] = BSP_PRV_VECT_ENUM(EVENT_IIC0_TEI,GROUP3), /* IIC0 TEI (Transmit end) */
            [4] = BSP_PRV_VECT_ENUM(EVENT_IIC0_ERI,GROUP4), /* IIC0 ERI (Transfer error) */
            [5] = BSP_PRV_VECT_ENUM(EVENT_SCI6_RXI,GROUP5), /* SCI6 RXI (Receive data full) */
            [6] = BSP_PRV_VECT_ENUM(EVENT_SCI6_TXI,GROUP6), /* SCI6 TXI (Transmit data empty) */
            [7] = BSP_PRV_VECT_ENUM(EVENT_SCI6_TEI,GROUP7), /* SCI6 TEI (Transmit end) */
            [8] = BSP_PRV_VECT_ENUM(EVENT_SCI6_ERI,GROUP0), /* SCI6 ERI (Receive error) */
            [9] = BSP_PRV_VECT_ENUM(EVENT_ADC0_SCAN_END,GROUP1), /* ADC0 SCAN END (End of A/D scanning operation) */
            [10] = BSP_PRV_VECT_ENUM(EVENT_RTC_PERIOD,GROUP2), /* RTC PERIOD (Periodic interrupt) */
            [11] = BSP_PRV_VECT_ENUM(EVENT_RTC_CARRY,GROUP3), /* RTC CARRY (Carry interrupt) */
            [12] = BSP_PRV_VECT_ENUM(EVENT_SCI9_RXI,GROUP4), /* SCI9 RXI (Receive data full) */
            [13] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TXI,GROUP5), /* SCI9 TXI (Transmit data empty) */
            [14] = BSP_PRV_VECT_ENUM(EVENT_SCI9_TEI,GROUP6), /* SCI9 TEI (Transmit end) */
            [15] = BSP_PRV_VECT_ENUM(EVENT_SCI9_ERI,GROUP7), /* SCI9 ERI (Receive error) */
            [16] = BSP_PRV_VECT_ENUM(EVENT_FCU_FRDYI,GROUP0), /* FCU FRDYI (Flash ready interrupt) */
            [17] = BSP_PRV_VECT_ENUM(EVENT_FCU_FIFERR,GROUP1), /* FCU FIFERR (Flash access error interrupt) */
            [18] = BSP_PRV_VECT_ENUM(EVENT_SCI8_RXI,GROUP2), /* SCI8 RXI (Receive data full) */
            [19] = BSP_PRV_VECT_ENUM(EVENT_SCI8_TXI,GROUP3), /* SCI8 TXI (Transmit data empty) */
            [20] = BSP_PRV_VECT_ENUM(EVENT_SCI8_TEI,GROUP4), /* SCI8 TEI (Transmit end) */
            [21] = BSP_PRV_VECT_ENUM(EVENT_SCI8_ERI,GROUP5), /* SCI8 ERI (Receive error) */
            [22] = BSP_PRV_VECT_ENUM(EVENT_GPT0_COUNTER_OVERFLOW,GROUP6), /* GPT0 COUNTER OVERFLOW (Overflow) */
            [23] = BSP_PRV_VECT_ENUM(EVENT_GPT2_COUNTER_OVERFLOW,GROUP7), /* GPT2 COUNTER OVERFLOW (Overflow) */
            [24] = BSP_PRV_VECT_ENUM(EVENT_GPT3_COUNTER_OVERFLOW,GROUP0), /* GPT3 COUNTER OVERFLOW (Overflow) */
            [25] = BSP_PRV_VECT_ENUM(EVENT_ICU_IRQ1,GROUP1), /* ICU IRQ1 (External pin interrupt 1) */
        };
        #endif
        #endif
