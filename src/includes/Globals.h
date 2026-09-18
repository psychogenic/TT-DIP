/*
 * Globals.h, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 16, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#ifndef SRC_INCLUDES_GLOBALS_H_
#define SRC_INCLUDES_GLOBALS_H_

#include "includes/CommandInterpreter.h"
#include "includes/ProjectMux.h"
#include "includes/USBSerial.h"

namespace TinyTapeout {

class Globals {
public:
	static TinyTapeout::CommandInterpreter * commandInterpreter();
	static TinyTapeout::ProjectMux * projectMUX();
	static Platform::IC * chip();
	static TinyTapeout::USBSerial * serial();

private:
	Globals();
};

} /* namespace TinyTapeout */

#endif /* SRC_INCLUDES_GLOBALS_H_ */
