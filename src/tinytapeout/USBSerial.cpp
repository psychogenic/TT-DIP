/*
 * USBSerial.cpp, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 15, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#include "includes/USBSerial.h"

namespace TinyTapeout {

USBSerial::USBSerial(CommandInterpreter * cmdintr) : cmd_interpreter(cmdintr) {

}

void USBSerial::outputString(const char * s) {
	printf(s);
}
bool USBSerial::handleUSBInput(int numbytes, uint8_t *data) {
	return cmd_interpreter->appendInputData(data, numbytes);
}
void USBSerial::outputPrompt() {

#ifdef TTDIP_USB_SERIAL_PROMPT
		outputString(TTDIP_USB_SERIAL_PROMPT);
#endif
}

void USBSerial::outputDebug(const char * s) {
	outputDebugMessage(TTDIP_DEBUGLVL_DEBUG, s);
}

void USBSerial::outputInfo(const char * s){
	outputDebugMessage(TTDIP_DEBUGLVL_INFO, s);
}
void USBSerial::outputWarn(const char * s) {
	outputDebugMessage(TTDIP_DEBUGLVL_WARN, s);
}
void USBSerial::outputError(const char * s) {
	outputDebugMessage(TTDIP_DEBUGLVL_ERROR, s);
}
void USBSerial::outputCritical(const char * s) {
	outputDebugMessage(TTDIP_DEBUGLVL_CRITICAL, s);
}


void USBSerial::outputDebugMessage(uint8_t level, const char * s) {
	static const char * lvls[] = {
			"CRITICAL",
			"ERROR",
			"WARN",
			"INFO",
			"DEBUG"
	};
	if (level > USBSERIAL_DEBUG_LEVEL) {
		return;
	}
	printf("%s: %s\r\n", lvls[level], s);



}

} /* namespace TinyTapeout */
