/*
 * driver_api.c, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 17, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#ifndef SRC_DRIVER_API_C_
#define SRC_DRIVER_API_C_

#include "includes/driver_api.h"
#include "includes/Driver.h"
#include "includes/Globals.h"

static TinyTapeout::Driver drv;

bool driver_init() {
	return drv.init();
}

void driver_tick() {
	drv.tick();

}

bool driver_handle_input(int numbytes, uint8_t *data) {
	return TinyTapeout::Globals::serial()->handleUSBInput(numbytes, data);
}



#endif /* SRC_DRIVER_API_C_ */
