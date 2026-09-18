/*
 * Clock.h, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 16, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#ifndef SRC_INCLUDES_PLATFORM_CLOCK_H_
#define SRC_INCLUDES_PLATFORM_CLOCK_H_

#include "platformIncludes.h"
#include "../settings.h"

namespace Platform {

class Clock {
public:
	Clock();
	ClockRateHz start(ClockRateHz hz);
	void stop(void);
	ClockRateHz rate() { return clock_rate_hz;}
private:
	ClockRateHz clock_rate_hz;
};

} /* namespace Platform */

#endif /* SRC_INCLUDES_PLATFORM_CLOCK_H_ */
