/*
 * TTDIPconfig.h, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 15, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#ifndef SRC_INCLUDES_TTDIPCONFIG_H_
#define SRC_INCLUDES_TTDIPCONFIG_H_

#define TT_DUMMYROM_REPO	"TinyTapeout/tinytapeout-gf-0p2"
#define TT_DUMMYROM_SHUTTLE	"ttgf0p2"
#define TT_DUMMYROM_COMMIT	"deadbeef01"

#define TTDIP_FIRMWARE_VERSION		"0.9.2"

#define TTDIP_FACTORY_TEST_DESIGNID			1
#define TTDIP_FACTORY_TEST_CLOCKRATE		10

#define TTDIP_USB_SERIAL_ECHO				1
#define TTDIP_USB_SERIAL_PROMPT				">>> "

#define TTDIP_POWERON_SETTLE_MS				10


#define COMMAND_INTERPRETER_INBUFLEN		64
#define COMMAND_INTERPRETER_CMDQUEUELEN		3

#define COMMAND_MAX_PARAMS					3


#define TTDIP_DEBUGLVL_CRITICAL	0
#define TTDIP_DEBUGLVL_ERROR	1
#define TTDIP_DEBUGLVL_WARN		2
#define TTDIP_DEBUGLVL_INFO		3
#define TTDIP_DEBUGLVL_DEBUG	4

#ifndef USBSERIAL_DEBUG_LEVEL
#define USBSERIAL_DEBUG_LEVEL		TTDIP_DEBUGLVL_INFO
#endif


#define TT_DUMMYROM_CONTENTS	"commit=" TT_DUMMYROM_COMMIT "\r\n" \
								"repo=" TT_DUMMYROM_REPO "\r\n" \
								"shuttle=" TT_DUMMYROM_SHUTTLE "\r\n" \
								"dipfw=" TTDIP_FIRMWARE_VERSION "\r\n"

#endif /* SRC_INCLUDES_TTDIPCONFIG_H_ */
