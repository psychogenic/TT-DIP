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

	SystemInit();

	funGpioInitAll();
    // funPinMode(TTDIP_PIN_CLOCK_PIN, GPIO_CFGLR_OUT_50Mhz_PP);

	clockAndResetControl(!SystemSettings.clock_rst_driven_externally);
	muxControl(!SystemSettings.mux_driven_externally);


	return true;
}
void IC::clockAndResetControl(bool enable) {

	if (enable) {

		funPinMode(TTDIP_PIN_PROJECT_RESET, GPIO_CFGLR_OUT_10Mhz_PP);
	    funPinMode(TTDIP_PIN_CLOCK_PIN, GPIO_CFGLR_OUT_50Mhz_PP);
	} else {

		funPinMode(TTDIP_PIN_PROJECT_RESET, GPIO_CNF_IN_PUPD);
	    funPinMode(TTDIP_PIN_CLOCK_PIN, GPIO_CNF_IN_PUPD);
	}

}
void IC::muxControl(bool enable) {

	if (enable) {

		funPinMode(TTDIP_PIN_MUX_EN, GPIO_CFGLR_OUT_10Mhz_PP);
		funPinMode(TTDIP_PIN_MUX_RESET, GPIO_CFGLR_OUT_10Mhz_PP);
		funPinMode(TTDIP_PIN_MUX_INC, GPIO_CFGLR_OUT_50Mhz_PP);
	} else {
		funPinMode(TTDIP_PIN_MUX_EN, GPIO_CNF_IN_PUPD);
		funPinMode(TTDIP_PIN_MUX_RESET, GPIO_CNF_IN_PUPD);
		funPinMode(TTDIP_PIN_MUX_INC, GPIO_CNF_IN_PUPD);
	}

}


void IC::delayMs(uint16_t millis) {
	Delay_Ms(millis);

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


void IC::modeInputPull(uint8_t pin, bool pull_up) {
	funPinMode(pin, GPIO_CFGLR_IN_PUPD);
	if (pull_up) {
		funDigitalWrite(pin, FUN_HIGH);
	} else {
		funDigitalWrite(pin, FUN_LOW);
	}
}

void IC::modeInput(uint8_t pin) {

	funPinMode(pin, GPIO_CFGLR_IN_FLOAT);
}

void IC::modeOutput(uint8_t pin, GPIOSpeed_TypeDef slew) {
	funPinMode(PC1, slew | GPIO_CNF_OUT_PP);
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
#endif
	funDigitalWrite(pin, FUN_HIGH);
}
void IC::writeLow(uint8_t pin) {
#ifdef DESKTOP_DEBUG
	OUTPUT_DEBUG("%i LOW\r\n", pin);
#endif
	funDigitalWrite(pin, FUN_LOW);

}

} /* namespace Platform */
