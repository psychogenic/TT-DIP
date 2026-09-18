/*
 * Globals.cpp, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 16, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#include "includes/Globals.h"

namespace TinyTapeout {

TinyTapeout::CommandInterpreter* Globals::commandInterpreter() {
	static TinyTapeout::CommandInterpreter _cmdInt;
	return &_cmdInt;
}

TinyTapeout::ProjectMux* Globals::projectMUX() {
	static TinyTapeout::ProjectMux _pmux(chip());
	return &_pmux;
}

Platform::IC* Globals::chip() {
	static Platform::IC _ic;
	return &_ic;
}

TinyTapeout::USBSerial * Globals::serial() {
	static TinyTapeout::USBSerial _ser(commandInterpreter());
	return &_ser;
}
Globals::Globals() {

}

} /* namespace TinyTapeout */
