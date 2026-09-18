/*
 * USBSerial.h, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 15, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#ifndef SRC_INCLUDES_USBSERIAL_H_
#define SRC_INCLUDES_USBSERIAL_H_
#include "TTDIPincludes.h"
#include "CommandInterpreter.h"


#if USBSERIAL_DEBUG_LEVEL >= TTDIP_DEBUGLVL_DEBUG
#define OUTPUT_DEBUG(...)	printf(__VA_ARGS__)
#else
#define OUTPUT_DEBUG(...)
#endif

#if USBSERIAL_DEBUG_LEVEL >= TTDIP_DEBUGLVL_INFO
#define OUTPUT_INFO(...)	printf(__VA_ARGS__)
#else
#define OUTPUT_INFO(...)
#endif

#if USBSERIAL_DEBUG_LEVEL >= TTDIP_DEBUGLVL_WARN
#define OUTPUT_WARN(...)	printf(__VA_ARGS__)
#else
#define OUTPUT_WARN(...)
#endif
#if USBSERIAL_DEBUG_LEVEL >= TTDIP_DEBUGLVL_ERROR
#define OUTPUT_ERROR(...)	printf(__VA_ARGS__)
#else
#define OUTPUT_ERROR(...)
#endif


namespace TinyTapeout {

class USBSerial {

public:
	USBSerial(CommandInterpreter * cmdintr);
	bool handleUSBInput(int numbytes, uint8_t *data);
	bool handleUSBInput(int numbytes, const char *data) { return handleUSBInput(numbytes, (uint8_t*)data);}

	static void outputPrompt();

	static void outputString(const char * s);
	static void outputDebug(const char * s);
	static void outputInfo(const char * s);
	static void outputWarn(const char * s);
	static void outputError(const char * s);
	static void outputCritical(const char * s);


private:
	static void outputDebugMessage(uint8_t level, const char * s);

	CommandInterpreter * cmd_interpreter;
};

} /* namespace TinyTapeout */

#endif /* SRC_INCLUDES_USBSERIAL_H_ */
