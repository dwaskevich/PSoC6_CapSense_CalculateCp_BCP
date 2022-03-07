/*
 * sensorCp.h
 *
 *  Created on: Mar 6, 2022
 *      Author: Waskevich
 */

#ifndef SOURCE_SENSORCP_H_
#define SOURCE_SENSORCP_H_

#include "stdint.h"

#define MODULATOR_CLK_FREQ_KHZ	(50000u)
#define VREF_CSD				(2021u)

uint16_t measureSensorCp(uint8_t widgetID, uint8_t snsNum);
void measureAllSensorCp(void);
uint32_t calculateCp(uint8_t widgetID, uint8_t snsNum);

#endif /* SOURCE_SENSORCP_H_ */
