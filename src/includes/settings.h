/*
 * settings.h, part of the TinyTapeoutDIP project
 *
 * ** SET THESE VALUES IN TOP-LEVEL projectselect.h **
 *
 *
 * We have no flash, no where for non-volatile storage
 * other than within the program.
 *
 * Still, in case we ever do, and just to be able to
 * tweak things on the fly within the executable,
 * settings are all stashed in a single struct: SystemSettings
 *
 *
 *  Created on: Sep 17, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#ifndef SRC_INCLUDES_SETTINGS_H_
#define SRC_INCLUDES_SETTINGS_H_

#include "TTDIPincludes.h"

#ifdef __cplusplus
extern "C" {
#endif


typedef uint16_t 	DesignID;
typedef uint32_t	ClockRateHz;

typedef struct settingsstruct {
	DesignID	default_design; /* loaded at startup */
	ClockRateHz	default_clock_rate; /* used with default design, if non-0 */
	uint8_t		echo_on; /* echo back on terminal */
	/* don't load/use MUX, drive using physical pins externally */
	uint8_t 	mux_driven_externally;
	/* don't load/use project clock and reset, drive using physical pins externally */
	uint8_t		clock_rst_driven_externally;


} Settings_t;

extern Settings_t SystemSettings;


#ifdef __cplusplus
}
#endif


#endif /* SRC_INCLUDES_SETTINGS_H_ */
