/*
 * measureSensorCp.c
 *
 *  Created on: Mar 6, 2022
 *      Author: Waskevich
 */

#include "cycfg_capsense.h"
#include "measureSensorCp.h"

uint16_t measureSensorCp(uint8_t widgetID, uint8_t snsNum)
{
	uint32_t sensorCp;

	Cy_CapSense_MeasureCapacitanceSensor(widgetID, snsNum, &sensorCp, &cy_capsense_context);

	return (uint16_t) sensorCp;
}

void measureAllSensorCp(void)
{
	for(uint8_t widgetID = 0; widgetID < CY_CAPSENSE_NUM_WD_VALUE; widgetID++)
	{
		uint32_t sensorCp, loopCounter = 0;


	}


}
