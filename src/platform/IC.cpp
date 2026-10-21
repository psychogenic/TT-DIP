/*
 * IC.cpp, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 16, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#include "includes/platform/IC.h"
#include "includes/USBSerial.h"
#include "includes/Globals.h"
namespace Platform {

IC::IC() {
}

bool IC::init() {

	SystemInit();

	funGpioInitAll();

	return true;
}
void IC::clockAndResetControl(bool enable) {

	if (enable) {
		modeOutput(TTDIP_PIN_PROJECT_RESET, GPIO_Speed_10MHz);
		modeOutput(TTDIP_PIN_CLOCK_PIN, GPIO_Speed_50MHz);
	} else {

		modeInputPull(TTDIP_PIN_PROJECT_RESET, true);
		modeInputPull(TTDIP_PIN_CLOCK_PIN, true);
	}

}
void IC::muxControl(bool enable) {

	if (enable) {
		modeOutput(TTDIP_PIN_MUX_EN, GPIO_Speed_10MHz);
		modeOutput(TTDIP_PIN_MUX_RESET, GPIO_Speed_10MHz);
		modeOutput(TTDIP_PIN_MUX_INC, GPIO_Speed_50MHz);
	} else {
		modeInputPull(TTDIP_PIN_MUX_EN, false);
		modeInputPull(TTDIP_PIN_MUX_RESET, false);
		modeInputPull(TTDIP_PIN_MUX_INC, true);

	}

}


void IC::delayMs(uint16_t millis) {
	Delay_Ms(millis);

}
ClockRateHz IC::loadFactoryTest() {

	TinyTapeout::USBSerial::outputInfo("Run Factory Test");
	TinyTapeout::Globals::chip()->clock().stop();
	TinyTapeout::Globals::projectMUX()->enable(TTDIP_FACTORY_TEST_DESIGNID);
	return TinyTapeout::Globals::chip()->clock().start(TTDIP_FACTORY_TEST_CLOCKRATE);
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
uint8_t IC::digitalRead(uint8_t pin) {
	return funDigitalRead(pin);
}
void IC::projectReset(bool inreset) {
	if (SystemSettings.clock_rst_driven_externally) {
		TinyTapeout::USBSerial::outputError("Can't reset -- driven externally");
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
