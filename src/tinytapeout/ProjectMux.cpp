/*
 * ProjectMux.cpp, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 16, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#include "includes/ProjectMux.h"
#include "includes/USBSerial.h"
#include "includes/Globals.h"
#include "includes/settings.h"

namespace TinyTapeout {

ProjectMux::ProjectMux(Platform::IC * ic) : chip(*ic), selected_design(0) {
}

void ProjectMux::reset() {

	if (SystemSettings.mux_driven_externally) {
		USBSerial::outputInfo("Can't reset mux -- driven externally");
	} else {
		chip.writeLow(TTDIP_PIN_MUX_INC);
		chip.writeLow(TTDIP_PIN_MUX_EN);
		chip.writeLow(TTDIP_PIN_MUX_RESET);
		chip.delayMs(10);
		chip.writeHigh(TTDIP_PIN_MUX_RESET);
		chip.delayMs(10);
	}
	selected_design = 0;
}

void ProjectMux::enable(DesignID id) {
	OUTPUT_INFO("Enable project %d\r\n", id);
	if (SystemSettings.mux_driven_externally) {
		USBSerial::outputInfo("MUX can't enable -- driven externally");
		return;
	}
	disable();
	reset_and_clock_mux(id);

	if (SystemSettings.reset_design_on_load) {
		USBSerial::outputDebug("Auto project reset on enable");
		Globals::chip()->projectReset(true);
		Globals::chip()->manualClock(TTDIP_NUM_CLOCKS_ON_PROJ_RESET);
	}
	Globals::chip()->projectReset(false);
	selected_design = id;

}

void ProjectMux::disable() {
	if (SystemSettings.mux_driven_externally) {
		USBSerial::outputInfo("MUX can't disable -- driven externally");
		return;
	}
	reset_and_clock_mux(0);
	chip.writeLow(TTDIP_PIN_MUX_EN);
	selected_design = 0;
}

void ProjectMux::reset_and_clock_mux(DesignID id) {

	reset();
	OUTPUT_DEBUG("Clocking MUX %d times\r\n", id);
	for (DesignID i=0; i<id; i++) {
		chip.writeHigh(TTDIP_PIN_MUX_INC);
		chip.delayMs(1);
		chip.writeLow(TTDIP_PIN_MUX_INC);
		chip.delayMs(1);

	}

	chip.writeHigh(TTDIP_PIN_MUX_EN);
}

} /* namespace TinyTapeout */
