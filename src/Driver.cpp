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
	if (! Globals::chip()->init() )
	{
		return false;
	}
	if (SystemSettings.default_design) {
		Globals::chip()->clock().stop();
		if (SystemSettings.mux_driven_externally) {

			USBSerial::outputInfo("Def design specified but MUX driven externally!");
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

} /* namespace TinyTapeout */
