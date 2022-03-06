/*
 * bridgeControlPanel_PSoC6.c
 *
 *  Created on: Mar 6, 2022
 *      Author: Waskevich
 *
 *  Note - PSoC 6 CapSense code examples use HAL to configure EZI2C (not Device Configurator)
 *
 *  Usage:
 *  	- Include header in main.c (#include "bridgeControlPanel_PSoC6.h")
 *  	- Define contents of register map in bridgeControlPanel_PSoC6.h
 *  		- Always include "uint16  CycleCount" since it's used in check_BCP()
 *  			- If unwanted, remove the arbitrary transaction counter in check_BCP()
 *  	- To use, read or write RegisterMap contents in the main application, include
 *  	  a declaration to it in main.c - i.e. extern bcp_stc_RegisterMap_t RegisterMap;
 *
 */

#include "cy_pdl.h"
#include "cyhal.h"
#include "cycfg.h"
#include "bridgeControlPanel_PSoC6.h"

bcp_stc_RegisterMap_t RegisterMap;
cyhal_ezi2c_slave_cfg_t sEzI2C_sub_cfg_2;

extern cyhal_ezi2c_t sEzI2C;
extern cyhal_ezi2c_cfg_t sEzI2C_cfg;
extern cyhal_ezi2c_slave_cfg_t sEzI2C_sub_cfg;

/*******************************************************************************
* Function Name: initialize_bcp
********************************************************************************
* Summary:
* - Re-initialize EZI2C peripheral to enable secondary slave address and point
*   the secondary buffer to the RegisterMap structure defined in
*   bridgeControlPanel_PSoC6.h. This enables communication with the
* 	Bridge Control Panel tool.
*
*******************************************************************************/
void initialize_BCP(void)
{
	cy_rslt_t result;

	cyhal_ezi2c_free(&sEzI2C); /* de-initialize EZI2C block for reconfiguration */

	/* Configure Capsense Tuner as primary EzI2C Slave */
	sEzI2C_sub_cfg.buf = (uint8 *)&cy_capsense_tuner;
	sEzI2C_sub_cfg.buf_rw_boundary = sizeof(cy_capsense_tuner);
	sEzI2C_sub_cfg.buf_size = sizeof(cy_capsense_tuner);
	sEzI2C_sub_cfg.slave_address = 8U;

	/* Configure BCP RegisterMap as secondary EzI2C Slave */
	sEzI2C_sub_cfg_2.buf = (uint8 *)&RegisterMap;
	sEzI2C_sub_cfg_2.buf_rw_boundary = sizeof(RegisterMap);
	sEzI2C_sub_cfg_2.buf_size = sizeof(RegisterMap);
	sEzI2C_sub_cfg_2.slave_address = 9U;

	/* Configure EzI2C Slave with two slave addresses */
	sEzI2C_cfg.data_rate = CYHAL_EZI2C_DATA_RATE_400KHZ;
	sEzI2C_cfg.enable_wake_from_sleep = false;
	sEzI2C_cfg.slave1_cfg = sEzI2C_sub_cfg;
	sEzI2C_cfg.slave2_cfg = sEzI2C_sub_cfg_2;
	sEzI2C_cfg.sub_address_size = CYHAL_EZI2C_SUB_ADDR16_BITS;
	sEzI2C_cfg.two_addresses = true;

	/* re-initialize EZI2C slave block */
	result = cyhal_ezi2c_init(&sEzI2C, CYBSP_I2C_SDA, CYBSP_I2C_SCL, NULL, &sEzI2C_cfg);
	if (result != CY_RSLT_SUCCESS)
	{
		for(;;); /* replace with fault handler if desired */
	}
}

/*******************************************************************************
* Function Name: check_BCP
********************************************************************************
* Summary:
* - Checks to see if a Master read transaction to the secondary slave address
*   has happened.
*
*******************************************************************************/
uint32_t check_BCP(void)
{
	cyhal_ezi2c_status_t status; /* EZI2C function call status return value */

	/* I2C - BCP interface - read status of secondary address and do something if Master read transaction happened */
	status = cyhal_ezi2c_get_activity_status(&sEzI2C);  /* Get slave status to see if a Master read transaction happened. */

	status &= CYHAL_EZI2C_STATUS_READ2 ; /* mask status with read of secondary slave address */
	if(CYHAL_EZI2C_STATUS_READ2 == status)
	{
		/* Add/replace code here to execute when Master read transaction is recognized */
		RegisterMap.CycleCount += 1; /* arbitrary transaction counter */
	}

	return status;
}

