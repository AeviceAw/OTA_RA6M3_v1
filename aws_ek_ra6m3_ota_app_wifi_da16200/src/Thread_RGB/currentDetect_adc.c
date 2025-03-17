//#include "hal_data.h"
//#include "AeviceSystemConfig.h"
//#include "SystemFunctions/RTT_Logger/RTT_Logger.h"
//#include <CurrentDetect_ADC/Current_Detect_ADC.h>

#include "initialisations.h"

#define ADC_RESET_VALUE   		(0x00)
#define ADC_12_BIT    			(4096u)
#define V_REF         			(3.3f)
#define VOLTAGE_STR_SIZE		10

#define ADC_TEST_CYCLE_TIME		500//update time (500 ms)

volatile bool g_ready_to_read = false;
static uint16_t g_adc_data;
static float g_adc_volt = {ADC_RESET_VALUE};
static float g_adc_mA=-2.0f;
static bool waitADC = false;

/*******************************************************************************************************************//**
 * @brief    This function open the ADC, configures and starts the scan
 * @param[IN]   None
 * @retval FSP_SUCCESS                  Upon successful open,configure adc,on success or calibrate and start adc scan
 * @retval Any Other Error code apart from FSP_SUCCES  Unsuccessful open or configure or start
 ***********************************************************************************************************************/
static fsp_err_t adc_scan_start(void);

/*******************************************************************************************************************//**
 * @brief    This function stops the scanning of adc
 * @param[IN]   None
 * @retval FSP_SUCCESS                  Upon successful stops the adc scan and closes the adc
 * @retval Any Other Error code apart from FSP_SUCCES  Unsuccessful stop or close of adc
 ***********************************************************************************************************************/
//static fsp_err_t adc_scan_stop(void);

/*******************************************************************************************************************//**
 * @brief    This function reads the adc output data from the prescribed channel and checks adc status
 * @param[IN]   None
 * @retval FSP_SUCCESS                  Upon successful stops the adc scan and closes the adc
 * @retval Any Other Error code apart from FSP_SUCCES  Unsuccessful stop or close of adc
 ***********************************************************************************************************************/
static fsp_err_t adc_read_data(void);

void adc_callback(adc_callback_args_t *p_args) {
	if (p_args->event == ADC_EVENT_SCAN_COMPLETE) {
		waitADC = true;
		//////RTT_LOG("[CD ADC]: waitADC => TRUE");
	}
}

void Test_ADC_Current_Detect(void) {
    ////RTT_LOG("========================================");
    ////RTT_LOG("[Current Detect ADC]: I2C RGB Testing...");
	for(uint8_t i = 0; i < 10; i++) {
		CD_Read_ADC();
		vTaskDelay(ADC_TEST_CYCLE_TIME);
	}
    ////RTT_LOG("========================================");
}

void CD_Read_ADC(void) {
	adc_scan_start();
	waitADC = false;
	for(uint8_t i = 0; i < 10; i++) {
		vTaskDelay(10); // sleep 10 ms
		if(waitADC) break;
	}
	//if(!waitADC) RTT_ERROR_LOG("[Current Detect ADC]: ADC incompleted");

	adc_read_data();


#if DEBUG_CURRENT_DETECT_ADC
    char debugStr[128];
    snprintf(debugStr, sizeof(debugStr) - 1, "[Current Detect ADC]: ADC: %4u   %.2fV", g_adc_data, g_adc_volt);
    ////RTT_LOG(debugStr);
#endif
}

fsp_err_t adc_scan_start(void) {
	fsp_err_t err = FSP_ERR_ASSERTION;
	if (!g_ready_to_read) {
		err = R_ADC_Open(&g_adc_current_detect_ctrl, &g_adc_current_detect_cfg);
		if (FSP_SUCCESS != err) {
            ////RTT_LOG("[Current Detect ADC]: *** R_ADC_Open API failed **");
            return err;
        }

        /* Configures the ADC scan parameters */
        err = R_ADC_ScanCfg (&g_adc_current_detect_ctrl, &g_adc_current_detect_channel_cfg);
		if (FSP_SUCCESS != err) {
            /* ADC Failure message */
			////RTT_LOG("[Current Detect ADC]: *** R_ADC_ScanCfg API failed ** \r\n");
            return err;
        }

        /* Start the ADC scan*/
		err = R_ADC_ScanStart(&g_adc_current_detect_ctrl);
		if (FSP_SUCCESS != err) {
            /* ADC Failure message */
			////RTT_LOG("[Current Detect ADC]: *** R_ADC_ScanStart API failed ** \r\n");
            return err;
        }
        g_ready_to_read = true;
    }
    return err;
}

#if 0
fsp_err_t adc_scan_stop(void) {
    fsp_err_t err = FSP_ERR_ASSERTION;

    /* Stop the scan if adc scan is started in continuous scan mode else ignore */
    if((ADC_MODE_SINGLE_SCAN != g_adc_current_detect_cfg.mode) && (true == g_ready_to_read )) {
		/* Stop ADC scan */
		err = R_ADC_ScanStop(&g_adc_current_detect_ctrl);
		if (FSP_SUCCESS != err) {
			////RTT_LOG("[Current Detect ADC]: *** R_ADC_ScanStop API failed **");
			return err;
		}

        /* reset to indicate stop reading the adc data */
        g_ready_to_read = false;

		err = R_ADC_Close(&g_adc_current_detect_ctrl);
		if (FSP_SUCCESS != err) {
			////RTT_LOG("[Current Detect ADC]: *** R_ADC_Close API failed **");
            return err;
        }
    }
    return err;
}
#endif


fsp_err_t adc_read_data(void) {
	volatile fsp_err_t err = FSP_ERR_ASSERTION;     // Error status

	err = R_ADC_Read(&g_adc_current_detect_ctrl, ADC_CHANNEL_16, &g_adc_data);
	if (FSP_SUCCESS != err) {
        /* ADC Failure message */
		////RTT_LOG("[Current Detect ADC]: *** R_ADC_Read API failed **");
        return err;
    }

    g_adc_volt = (float)((g_adc_data * V_REF)/ADC_12_BIT);
    g_adc_mA   = ((float)(g_adc_data))*0.02603f;

    /* In adc single scan mode after reading the data, it stops.So reset the g_ready_to_read state to
     * avoid reading unnecessarily. close the adc module as it gets opened in start scan command.*/
	if (ADC_MODE_SINGLE_SCAN == g_adc_current_detect_cfg.mode) {
        g_ready_to_read = false;

		err = R_ADC_ScanStop(&g_adc_current_detect_ctrl);
		if (FSP_SUCCESS != err) {
			/* ADC ScanStop message */
			////RTT_LOG("[Current Detect ADC]: *** R_ADC_ScanStop API failed **");
            ////APP_ERR
        }

		err = R_ADC_Close(&g_adc_current_detect_ctrl);
		if (FSP_SUCCESS != err) {
            /* ADC Failure message */
			////RTT_LOG("[Current Detect ADC]: *** R_ADC_Close API failed **");
            return err;
        }
    }
    return err;
}

/**********************************************************************************************************************
 * @brief       Get the value of the <<(uint16_t) g_adc_data>>
 * @param       None
 * @retval      (uint16_t) g_adc_data
 **********************************************************************************************************************/
uint16_t g_adcData_get(void){
    return g_adc_data ;
} // end g_adcData_get()

/**********************************************************************************************************************
 * @brief       Get the value of the <<(uint16_t) g_adc_volt>>
 * @param       None
 * @retval      (float_t) g_adc_volt
 **********************************************************************************************************************/
float g_adcVolt_get(void){
    return g_adc_volt ;
} // end g_adcVolt_get()


/**********************************************************************************************************************
 * @brief       Get the value of the <<(uint16_t) g_adc_volt>>
 * @param       None
 * @retval      (float_t) g_adc_volt
 **********************************************************************************************************************/
float g_adc_mA_get(void){
    return g_adc_mA; // emperical: average over 163 samples of adc
} // end g_adc_mA_get()
