/*
 * sensorCp.c
 *
 *  Created on: Mar 6, 2022
 *      Author: Waskevich
 *
 *  Description:
 *  	- measureSensorCp uses BIST library APIs to measure sensor Cp
 *  	- calculateSensorCp uses scan parameters to calculate sensor Cp
 *  	- define Vref in sensorCp.h
 *
 *  Usage:
 *  	- Include header in main.c (#include "sensorCp.h")
 *  	- calculateCp returns a uint32_t value ... recast to uint16_t if needed
 *  	- calculateAllSensorCp - not implemented yet
 *
 * measureSensorCp: uses BIST library APIs to measure sensor Cp
 *
 * calculateCp: Calculates parasitic capacitance (Cp) for dual-iDAC
 *              CSD sensors using scan parameters and values retrieved
 *              from the dsRAM array. This should replicate results
 *              obtained from the BIST Cy_CapSense_MeasureCapacitanceSensor()
 *              API without having to disrupt normal scanning to run
 *              the BIST algorithm (which can take up to 1 ms per sensor).
 *
 * Cp formula = [idac_gain * mod_idac * raw] / [vref * scan_freq * MAX_resolution] +
 *              [idac_gain * comp_idac] / [vref * scan_freq]
 *
 * Returns sensor Cp in femtofarads. Returns 0x00 if requested widget or
 *              sensor is out of range.
 *
 */

#include "cycfg_capsense.h"
#include "sensorCp.h"

uint16_t measureSensorCp(uint8_t widgetID, uint8_t snsNum)
{
	uint32_t sensorCp;

	Cy_CapSense_MeasureCapacitanceSensor(widgetID, snsNum, &sensorCp, &cy_capsense_context);

	return (uint16_t) sensorCp;
}

void calculateAllSensorCp(void)
{
	uint32_t sensorCp, loopCounter = 0;

	for(uint8_t widgetID = 0; widgetID < CY_CAPSENSE_NUM_WD_VALUE; widgetID++)
	{

	}
}

uint32_t calculateCp(uint8_t widgetID, uint8_t snsNum)
{
	/* declare pointers to required parameters in CapSense data structure */
	const cy_stc_capsense_common_config_t * ptrCommonConfig;
	const cy_stc_capsense_widget_config_t * ptrWdConfig; /* pointer to widget configuration parameters in Flash */
	cy_stc_capsense_widget_context_t * ptrWdContext; /* pointer to widget configuration parameters in SRAM */
	cy_stc_capsense_sensor_context_t * ptrSensors; /* pointer to sensor data in SRAM */

    /* set CapSense data structure pointers to first element (widget or sensor) of each array */
    ptrWdConfig = cy_capsense_context.ptrWdConfig; /* set widget pointer to beginning of widget array (widget 0) */
    ptrSensors = cy_capsense_tuner.sensorContext; /* required for sensor scan data (raw, diff, status, etc) */
    ptrCommonConfig = cy_capsense_context.ptrCommonConfig; /* common config required for clock frequency, idac table */

	/* variables to hold scan parameters retrieved from CapSense dsRAM structure and used to calculate sensor Cp */
	uint32 iDAC_gain, mod_iDAC_value, comp_iDAC_value, sensor_raw_count, snsClkDiv, snsClkSource, snsClkFreq,
	    vref, scan_resolution, idacGainTableIndex, scan_resolution_MAX_value, sensor_Cp_calculated;

    ptrWdConfig += widgetID; /* adjust pointer to requested widget ID */
    ptrWdContext = ptrWdConfig->ptrWdContext; /* required for widget parameters (FT, NT, HYST, etc) */
    if(widgetID < CY_CAPSENSE_NUM_WD_VALUE && snsNum < ptrWdConfig->numSns)
    {
    	ptrSensors = ptrWdConfig->ptrSnsContext; /* set sensor pointer to first sensor in widget */
        ptrSensors += snsNum; /* adjust pointer to requested sensor element */
        mod_iDAC_value = ptrWdContext->idacMod[0]; /* modulator iDAC is associated with the entire widget */
        snsClkDiv = ptrWdContext->snsClk;
        snsClkSource = ptrWdContext->snsClkSource & SENSE_CLOCK_SOURCE_AUTO_MASK;
        if(CY_CAPSENSE_CLK_SOURCE_DIRECT != snsClkSource)  /* dithered clocks have 1/2 effective frequency */
            snsClkFreq = (MODULATOR_CLK_FREQ_KHZ / snsClkDiv) / 2;
        else snsClkFreq = MODULATOR_CLK_FREQ_KHZ / snsClkDiv;
        vref = VREF_CSD;
        idacGainTableIndex = ptrWdContext->idacGainIndex;
        scan_resolution = ptrWdContext->resolution;
//        scan_resolution_MAX_value = (1u << scan_resolution) - 1;
        scan_resolution_MAX_value = ptrWdContext->maxRawCount;
        iDAC_gain = ptrCommonConfig->idacGainTable[idacGainTableIndex].gainValue / 1000;
        comp_iDAC_value = ptrSensors->idacComp; /* compensation iDAC is associated with each sensor within the widget */
        sensor_raw_count = ptrSensors->raw; /* raw values are associated with each sensor within the widget */
        sensor_Cp_calculated = ((iDAC_gain * mod_iDAC_value * sensor_raw_count) / (( ((vref * snsClkFreq) / 100) * scan_resolution_MAX_value) / 1000) +
            (iDAC_gain * comp_iDAC_value) / (((vref * snsClkFreq) / 1000) / 100)) * 10;

        return sensor_Cp_calculated;
    }
    else
    {
        return 0x00;
    }
}

