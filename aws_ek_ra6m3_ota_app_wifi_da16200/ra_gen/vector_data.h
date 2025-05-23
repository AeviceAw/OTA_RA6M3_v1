/* generated vector header file - do not edit */
#ifndef VECTOR_DATA_H
#define VECTOR_DATA_H
#ifdef __cplusplus
        extern "C" {
        #endif
/* Number of interrupts allocated */
#ifndef VECTOR_DATA_IRQ_COUNT
#define VECTOR_DATA_IRQ_COUNT    (11)
#endif
/* ISR prototypes */
void gpt_counter_overflow_isr(void);
void sci_uart_rxi_isr(void);
void sci_uart_txi_isr(void);
void sci_uart_tei_isr(void);
void sci_uart_eri_isr(void);
void fcu_frdyi_isr(void);
void fcu_fiferr_isr(void);

/* Vector table allocations */
#define VECTOR_NUMBER_GPT1_COUNTER_OVERFLOW ((IRQn_Type) 0) /* GPT1 COUNTER OVERFLOW (Overflow) */
#define GPT1_COUNTER_OVERFLOW_IRQn          ((IRQn_Type) 0) /* GPT1 COUNTER OVERFLOW (Overflow) */
#define VECTOR_NUMBER_SCI9_RXI ((IRQn_Type) 1) /* SCI9 RXI (Receive data full) */
#define SCI9_RXI_IRQn          ((IRQn_Type) 1) /* SCI9 RXI (Receive data full) */
#define VECTOR_NUMBER_SCI9_TXI ((IRQn_Type) 2) /* SCI9 TXI (Transmit data empty) */
#define SCI9_TXI_IRQn          ((IRQn_Type) 2) /* SCI9 TXI (Transmit data empty) */
#define VECTOR_NUMBER_SCI9_TEI ((IRQn_Type) 3) /* SCI9 TEI (Transmit end) */
#define SCI9_TEI_IRQn          ((IRQn_Type) 3) /* SCI9 TEI (Transmit end) */
#define VECTOR_NUMBER_SCI9_ERI ((IRQn_Type) 4) /* SCI9 ERI (Receive error) */
#define SCI9_ERI_IRQn          ((IRQn_Type) 4) /* SCI9 ERI (Receive error) */
#define VECTOR_NUMBER_SCI7_RXI ((IRQn_Type) 5) /* SCI7 RXI (Receive data full) */
#define SCI7_RXI_IRQn          ((IRQn_Type) 5) /* SCI7 RXI (Receive data full) */
#define VECTOR_NUMBER_SCI7_TXI ((IRQn_Type) 6) /* SCI7 TXI (Transmit data empty) */
#define SCI7_TXI_IRQn          ((IRQn_Type) 6) /* SCI7 TXI (Transmit data empty) */
#define VECTOR_NUMBER_SCI7_TEI ((IRQn_Type) 7) /* SCI7 TEI (Transmit end) */
#define SCI7_TEI_IRQn          ((IRQn_Type) 7) /* SCI7 TEI (Transmit end) */
#define VECTOR_NUMBER_SCI7_ERI ((IRQn_Type) 8) /* SCI7 ERI (Receive error) */
#define SCI7_ERI_IRQn          ((IRQn_Type) 8) /* SCI7 ERI (Receive error) */
#define VECTOR_NUMBER_FCU_FRDYI ((IRQn_Type) 9) /* FCU FRDYI (Flash ready interrupt) */
#define FCU_FRDYI_IRQn          ((IRQn_Type) 9) /* FCU FRDYI (Flash ready interrupt) */
#define VECTOR_NUMBER_FCU_FIFERR ((IRQn_Type) 10) /* FCU FIFERR (Flash access error interrupt) */
#define FCU_FIFERR_IRQn          ((IRQn_Type) 10) /* FCU FIFERR (Flash access error interrupt) */
#ifdef __cplusplus
        }
        #endif
#endif /* VECTOR_DATA_H */
