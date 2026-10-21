/*
 * Driver.cpp, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 17, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#include "includes/Driver.h"
#include "includes/Globals.h"
#include "includes/settings.h"

namespace TinyTapeout {

Driver::Driver() {
}

void Driver::tick() {
	/*
	 * Grab the command interpreter and execute any pending
	 * commands it has received.
	 */
	CommandInterpreter * cmdInt = Globals::commandInterpreter();
	uint8_t i=0;
	for (i=0; i<cmdInt->numCommandsReceived(); i++) {
		const TinyTapeout::Command & curCommand = cmdInt->commandsQueued()[i];
		curCommand.dump();
		curCommand.execute();
	}
	cmdInt->clearCommandsQueued();
	if (i) {
		Globals::serial()->outputPrompt();
	}
}

bool Driver::init() {
	Platform::IC* chip = Globals::chip();
	if (! chip->init() )
	{
		return false;
	}

	initExtDriveCheck();
	chip->muxControl(!SystemSettings.mux_driven_externally);

	bool factoryTestLoaded = initFactoryTestCheck();

	if (factoryTestLoaded) {
		// we are done here
		return true;
	}
	chip->clockAndResetControl(!SystemSettings.clock_rst_driven_externally);

	if (SystemSettings.default_design) {
		Globals::chip()->clock().stop();
		if (SystemSettings.mux_driven_externally) {

			USBSerial::outputInfo("Def design specified but MUX driven ext");
		} else {
			USBSerial::outputInfo("Loading default project");
			Globals::projectMUX()->enable(SystemSettings.default_design);
		}

		if (SystemSettings.default_clock_rate) {
			// clock before loading project, so if it's set to
			// auto - reset projects
			if (SystemSettings.clock_rst_driven_externally) {

				USBSerial::outputInfo("Def clock rate set, but clock drv ext");
			} else {
				USBSerial::outputInfo("Clocking default project");
				Globals::chip()->clock().start(SystemSettings.default_clock_rate);
			}
		}
	}

	return true;


}


void Driver::initExtDriveCheck() {

	if (! SystemSettings.check_ext_drive_on_boot) {
		return;
	}

	Platform::IC* chip = Globals::chip();
	chip->modeInputPull(TTDIP_PIN_CLOCK_PIN, true);
	chip->modeInputPull(TTDIP_PIN_MUX_INC, true);
	Delay_Ms(TTDIP_POWERON_SETTLE_MS);

	USBSerial::outputDebug("Ext drive check");
	if (chip->digitalRead(TTDIP_PIN_CLOCK_PIN)) {
		// not driven LOW, no external driver
		USBSerial::outputDebug("No clk/rst driver");
		SystemSettings.clock_rst_driven_externally = 0;
	} else {
		USBSerial::outputDebug("EXT clk/rst driver");
		SystemSettings.clock_rst_driven_externally = 1;
	}


	if (chip->digitalRead(TTDIP_PIN_MUX_INC)) {
		// not driven LOW, no external driver
		USBSerial::outputDebug("No MUX driver");
		SystemSettings.mux_driven_externally = 0;
	} else {
		USBSerial::outputDebug("EXT MUX driver");
		SystemSettings.mux_driven_externally = 1;
	}

}

#ifdef DESKTOP_DEBUG
#define FORCE_BOOT_FACT_RESET	false /* set to true to test */
#else
#define FORCE_BOOT_FACT_RESET	false
#endif

bool Driver::initFactoryTestCheck() {
	if (SystemSettings.mux_driven_externally || SystemSettings.clock_rst_driven_externally) {
		USBSerial::outputDebug("Can't load fact test: MUX/CLK ext");
		return false;
	}
	Platform::IC* chip = Globals::chip();
	chip->modeInputPull(TTDIP_PIN_PROJECT_RESET, true);
	Delay_Ms(TTDIP_POWERON_SETTLE_MS);

	if (chip->digitalRead(TTDIP_PIN_PROJECT_RESET) && ! FORCE_BOOT_FACT_RESET) {
		// not pulled low, no factory test
		USBSerial::outputDebug("RST not pulled down, no fact test");
		return false;
	}

	USBSerial::outputInfo("RST pulled: auto-run factory test");
	chip->loadFactoryTest();

	return true;



}

} /* namespace TinyTapeout */
