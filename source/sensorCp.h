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

/* CSD Vref can be found in CapSense Configurator Parameter View on CSD Settings tab */
//#define VREF_CSD				(1219u)
/* voltage on Cmod measured with DMM */
#define VREF_CSD				(1367u)

#define SENSE_CLOCK_SOURCE_AUTO_MASK	(0x7f)

uint16_t measureSensorCp(uint8_t widgetID, uint8_t snsNum);
void measureAllSensorCp(void);
uint32_t calculateCp(uint8_t widgetID, uint8_t snsNum);

#endif /* SOURCE_SENSORCP_H_ */
