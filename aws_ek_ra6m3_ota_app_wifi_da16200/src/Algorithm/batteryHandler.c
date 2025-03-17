/*
 * batteryHandler.c
 *
 *  Created on: 14 Jun 2024
 *      Author: yyang
 */


#include "initialisations.h"

/* Global Variables for Battery Information */
float batt = -3.0f;  ///< Compare against Battery Voltage threshold to stop disconnecting
float bp = -3.0f;    ///< Battery Percentage to change RGB from glowing-blue to pulsing-blue

float32_t bp_publish = -2.0f;
float32_t batt_publish = -2.0f;

// Perform 1-step pseudo-mean averaging of battery percentage
float bp_buff =0.0f;
uint8_t bp_use_mu = 0;



/**********************************************************************************************************
 * @brief  Sets the battery level
 * @param  inBatt - sets the software variable for battery level (in volts)
 * @return None
 *********************************************************************************************************/
void batt_set(float inBatt) {
    batt = inBatt;
} // end batt_set()

/**********************************************************************************************************
 * @brief  Gets the battery level
 * @param  None
 * @return batt - the software level of the battery
 *********************************************************************************************************/
float batt_get(void) {
    return batt;
} // end batt_get()

/**********************************************************************************************************
 * @brief  Gets the battery percentage value
 * @param  None
 * @return bp - the software % level of the battery
 *********************************************************************************************************/
float batt_percentage_get(void) {
    return bp;
} // end batt_percentage_get()


/**********************************************************************************************************
 * @brief  Sets the batt publish level
 * @param  inBatt - sets the software variable for battery level (in volts)
 * @return None
 *********************************************************************************************************/
void batt_publish_set(float inBatt) {
	batt_publish = inBatt;
} // batt_publish_set();

/**********************************************************************************************************
 * @brief  Sets the bp publish level
 * @param  inBp - sets the software variable for battery percentage (,i.e., unitless and <1)
 * @return None
 *********************************************************************************************************/
void bp_publish_set(float inBp) {
	bp_publish = inBp;
} // end bp_publish_set()

/**********************************************************************************************************
 * @brief  gets the batt publish level for MQTT publish
 * @param  None
 * @return (float32_t) batt_publish
 *********************************************************************************************************/
float batt_publish_get(void) {
	return batt_publish;
} // batt_publish_get();

/**********************************************************************************************************
 * @brief  gets the bp publish level  for MQTT publish
 * @param  None
 * @return (float32_t) bp_publish
 *********************************************************************************************************/
float bp_publish_get(void) {
	return bp_publish;
} // end bp_publish_get()



/**********************************************************************************************************
 * @brief  Computes the moving average of the battery percentage
 * @param  bp_in - input bp level
 * @param  bp_out - output bp level
 * @return bp - the software % level of the battery
 *********************************************************************************************************/
// called from getData() and define in battToBP.c
void bp_mu_get(float* bp_in, float* bp_out)
{

    // Compute pseudo-mean of BP value
    if (bp_use_mu ==1){
        bp_out[0] = ((float)floor((bp_in[0]+bp_buff)*0.5f));

        // If current average value is larger than previous (make no sense), use previous
        if (bp_out[0]>bp_buff)
            bp_out[0] = bp_buff;
    } // end if (bp_use_mu ==1) ; Have to reset everything b

    // First time - bp_out = bp_in + turn-on use_mu
    if (bp_use_mu==0){
        bp_out[0] = ((float)floor(bp_in[0]));
        bp_use_mu = 1;
    } // end if (bp_use_mu ==0 )

    bp_buff = bp_out[0];

}// end bp_mu_get()


/**********************************************************************************************************
 * @brief  Maps the battery voltage level to its percentage level
 * @param  inBatt - pointer to input battery address
 * @param  outBp - pointer to battery percentage address
 * @return None
 *********************************************************************************************************/
void bp_get(float* inBatt, float* outBp)
{
    const float bMg[19] = {4.0330000f,4.0019999f,3.9519999f,3.9130001f,3.8900001f,3.8629999f,3.8380001f,3.8150001f,3.7950001f,3.7780001f,3.7570000f,3.7420001f,3.7240000f,3.7060001f,3.6840000f,3.6630001f,3.6440001f,3.6270001f,3.5940001f};

    if (inBatt[0] > bMg[0])
        outBp[0] = 100.0f;
    else if (inBatt[0] > bMg[1])
        outBp[0] = 100.0f-(( bMg[0]-inBatt[0])/( bMg[0]-bMg[1]))*5.0f;
    else if (inBatt[0] > bMg[2])
        outBp[0] = 095.0f-(( bMg[1]-inBatt[0])/( bMg[1]-bMg[2]))*5.0f;
    else if (inBatt[0] > bMg[3])
        outBp[0] = 090.0f-(( bMg[2]-inBatt[0])/( bMg[2]-bMg[3]))*5.0f;
    else if (inBatt[0] > bMg[4])
        outBp[0] = 085.0f-(( bMg[3]-inBatt[0])/( bMg[3]-bMg[4]))*5.0f;
    else if (inBatt[0] > bMg[5])
        outBp[0] = 080.0f-(( bMg[4]-inBatt[0])/( bMg[4]-bMg[5]))*5.0f;
    else if (inBatt[0] > bMg[6])
        outBp[0] = 075.0f-(( bMg[5]-inBatt[0])/( bMg[5]-bMg[6]))*5.0f;
    else if (inBatt[0] > bMg[7])
        outBp[0] = 070.0f-(( bMg[6]-inBatt[0])/( bMg[6]-bMg[7]))*5.0f;
    else if (inBatt[0] > bMg[8])
        outBp[0] = 065.0f-(( bMg[7]-inBatt[0])/( bMg[7]-bMg[8]))*5.0f;
    else if (inBatt[0] > bMg[9])
        outBp[0] = 060.0f-(( bMg[8]-inBatt[0])/( bMg[8]-bMg[9]))*5.0f;
    else if (inBatt[0] > bMg[10])
        outBp[0] = 055.0f-(( bMg[9]-inBatt[0])/( bMg[9]-bMg[10]))*5.0f;
    else if (inBatt[0] > bMg[11])
        outBp[0] = 050.0f-(( bMg[10]-inBatt[0])/( bMg[10]-bMg[11]))*5.0f;
    else if (inBatt[0] > bMg[12])
        outBp[0] = 045.0f-(( bMg[11]-inBatt[0])/( bMg[11]-bMg[12]))*5.0f;
    else if (inBatt[0] > bMg[13])
        outBp[0] = 040.0f-(( bMg[12]-inBatt[0])/( bMg[12]-bMg[13]))*5.0f;
    else if (inBatt[0] > bMg[14])
        outBp[0] = 035.0f-(( bMg[13]-inBatt[0])/( bMg[13]-bMg[14]))*5.0f;
    else if (inBatt[0] > bMg[15])
        outBp[0] = 030.0f-(( bMg[14]-inBatt[0])/( bMg[14]-bMg[15]))*5.0f;
    else if (inBatt[0] > bMg[16])
        outBp[0] = 025.0f-(( bMg[15]-inBatt[0])/( bMg[15]-bMg[16]))*5.0f;
    else if (inBatt[0] > bMg[17])
        outBp[0] = 020.0f-(( bMg[16]-inBatt[0])/( bMg[16]-bMg[17]))*5.0f;
    else if (inBatt[0] > bMg[18])
        outBp[0] = 015.0f-(( bMg[17]-inBatt[0])/( bMg[17]-bMg[18]))*10.0f;
    else
        outBp[0] = 5.0f;

    // Perform Moving average on the battery percentage
    //getBP_MA(bp,bp);

    // Perform pseudo-average on the battery percentage
    //getBP_mu(bp,bp);
} // end bp_get();


