/*
 * bridgeControlPanel_PSoC6.h
 *
 *  Created on: Mar 6, 2022
 *      Author: Waskevich
 */

#ifndef BRIDGECONTROLPANEL_H_
#define BRIDGECONTROLPANEL_H_

#include "cycfg_capsense.h"

/* Define a register map that will be exposed to I2C host at secondary address (for Bridge Control Panel) */
typedef struct {
	uint16_t  CycleCount; /* holds arbitrary I2C Master-read cycle count */
	uint16_t  numWidgets;
	uint16_t  Cmod; /* BIST Cmod measurement */
	uint16_t  sensorCp[CY_CAPSENSE_SENSOR_COUNT]; /* BIST Cp measurements */
    uint16_t  sensor_raw_count[CY_CAPSENSE_SENSOR_COUNT];
    uint16_t  bsln[CY_CAPSENSE_SENSOR_COUNT];
    uint16_t  diff[CY_CAPSENSE_SENSOR_COUNT];
} bcp_stc_RegisterMap_t;

void initialize_BCP(void);
uint32_t check_BCP(void);

#endif /* BRIDGECONTROLPANEL_H_ */
