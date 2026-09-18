/*
 * IC.cpp, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 16, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#include "includes/platform/IC.h"
#include "includes/USBSerial.h"
#include "includes/settings.h"
namespace Platform {

IC::IC() {
}

bool IC::init() {

#ifndef DESKTOP_DEBUG
	SystemInit();

	funGpioInitAll();
    // funPinMode(TTDIP_PIN_CLOCK_PIN, GPIO_CFGLR_OUT_50Mhz_PP);
#endif
	clockAndResetControl(!SystemSettings.clock_rst_driven_externally);
	muxControl(!SystemSettings.mux_driven_externally);


	return true;
}
void IC::clockAndResetControl(bool enable) {

#ifndef DESKTOP_DEBUG
	if (enable) {

		funPinMode(TTDIP_PIN_PROJECT_RESET, GPIO_CFGLR_OUT_10Mhz_PP);
	    funPinMode(TTDIP_PIN_CLOCK_PIN, GPIO_CFGLR_OUT_50Mhz_PP);
	} else {

		funPinMode(TTDIP_PIN_PROJECT_RESET, GPIO_CNF_IN_PUPD);
	    funPinMode(TTDIP_PIN_CLOCK_PIN, GPIO_CNF_IN_PUPD);
	}
#endif /* DESKTOP_DEBUG */
}
void IC::muxControl(bool enable) {
#ifndef DESKTOP_DEBUG
	if (enable) {

		funPinMode(TTDIP_PIN_MUX_EN, GPIO_CFGLR_OUT_10Mhz_PP);
		funPinMode(TTDIP_PIN_MUX_RESET, GPIO_CFGLR_OUT_10Mhz_PP);
		funPinMode(TTDIP_PIN_MUX_INC, GPIO_CFGLR_OUT_50Mhz_PP);
	} else {
		funPinMode(TTDIP_PIN_MUX_EN, GPIO_CNF_IN_PUPD);
		funPinMode(TTDIP_PIN_MUX_RESET, GPIO_CNF_IN_PUPD);
		funPinMode(TTDIP_PIN_MUX_INC, GPIO_CNF_IN_PUPD);
	}
#endif /* DESKTOP_DEBUG */
}


void IC::delayMs(uint16_t millis) {
#ifdef DESKTOP_DEBUG
	struct timespec ts = {
	        .tv_sec  = millis / 1000,
	        .tv_nsec = (long)(millis % 1000) * 1000000L
	    };
	while (nanosleep(&ts, &ts) == -1)
	   ; /* retry if interrupted by a signal */
#else
	Delay_Ms(millis);
#endif

}

void IC::manualClock(uint16_t times, uint16_t delay_millis) {
	if (SystemSettings.clock_rst_driven_externally) {
		TinyTapeout::USBSerial::outputError("Can't manual clock -- driven externally");
		return ;
	}
	for (uint16_t i=0; i<times; i++) {
		writeLow(TTDIP_PIN_CLOCK_PIN);
		delayMs(delay_millis);
		writeHigh(TTDIP_PIN_CLOCK_PIN);
		delayMs(delay_millis);
	}
}


void IC::projectReset(bool inreset) {
	if (SystemSettings.clock_rst_driven_externally) {
		TinyTapeout::USBSerial::outputError("Can't reset project -- driven externally");
		return ;
	}
	if (inreset) {
		// active LOW reset
		writeLow(TTDIP_PIN_PROJECT_RESET);
	} else {
		writeHigh(TTDIP_PIN_PROJECT_RESET);
	}
}
void IC::writeHigh(uint8_t pin) {
#ifdef DESKTOP_DEBUG
	OUTPUT_DEBUG("%i HIGH\r\n", pin);
#else
	funDigitalWrite(pin, FUN_HIGH);
#endif
}
void IC::writeLow(uint8_t pin) {
#ifdef DESKTOP_DEBUG
	OUTPUT_DEBUG("%i LOW\r\n", pin);
#else
	funDigitalWrite(pin, FUN_LOW);
#endif
}

} /* namespace Platform */
