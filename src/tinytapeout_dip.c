/*
 * tinytapeout_dip.c, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 17, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#include "ch32fun.h"
#include <stdio.h>
#include <string.h>
#include "fsusb.h"
#include "includes/driver_api.h"
#include "includes/settings.h"


// this callback is mandatory when FUNCONF_USE_USBPRINTF is defined,
// can be empty though
void HandleUSBInput(int numbytes, uint8_t *data) {
	if (SystemSettings.echo_on) {
		_write(0, (const char*)data, numbytes);
	}
	driver_handle_input(numbytes, data);
}


int main()
{
	SystemInit();
	funGpioInitAll();
	USBFSSetup();
	if (! driver_init() ) {
		for (;;) {
			printf("Could not init driver??");
			Delay_Ms(500);
		}
	}

	while(1) {
		poll_input(); // check if there is input from the tty
		driver_tick();
		Delay_Ms(10);
	}
}


