/*
 * IC.h, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 16, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#ifndef SRC_INCLUDES_PLATFORM_IC_H_
#define SRC_INCLUDES_PLATFORM_IC_H_

#include "Clock.h"
namespace Platform {

class IC {
public:
	IC();
	bool init();
	void clockAndResetControl(bool enable);
	void muxControl(bool enable);

	Clock & clock() { return _clk;}

	void projectReset(bool inreset);

	void manualClock(uint16_t times=1, uint16_t delay_millis=1);

	void delayMs(uint16_t millis);


	void writeHigh(uint8_t pin);
	void writeLow(uint8_t pin);
private:
	Clock _clk;
};

} /* namespace Platform */

#endif /* SRC_INCLUDES_PLATFORM_IC_H_ */
