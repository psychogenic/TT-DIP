/*
 * platformConfig.h, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 16, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#ifndef SRC_INCLUDES_PLATFORM_PLATFORMCONFIG_H_
#define SRC_INCLUDES_PLATFORM_PLATFORMCONFIG_H_


#ifndef DESKTOP_DEBUG

#define TTDIP_PIN_CLOCK_PORT		GPIOA
#define TTDIP_PIN_CLOCK_PORTPIN		5
#define TTDIP_PIN_CLOCK_PIN			PA5
//define TTDIP_PIN_CLOCK_PIN		PA3

#if TTDIP_PIN_CLOCK_PIN == PA3
#define TTDIP_CLOCK_USE_PA3
#else
#define TTDIP_CLOCK_USE_TIM3
#endif



#define TTDIP_PIN_MUX_EN			PC0
#define TTDIP_PIN_MUX_RESET			PC3
#define TTDIP_PIN_MUX_INC			PA0

#define TTDIP_PIN_PROJECT_RESET		PA4

#else

#define TTDIP_PIN_CLOCK_PORT		0
#define TTDIP_PIN_CLOCK_PORTPIN		5
#define TTDIP_PIN_CLOCK_PIN			TTDIP_PIN_CLOCK_PORTPIN

#define TTDIP_PIN_PROJECT_RESET		4

#define TTDIP_PIN_MUX_EN			1
#define TTDIP_PIN_MUX_RESET			2
#define TTDIP_PIN_MUX_INC			3




#endif


#endif /* SRC_INCLUDES_PLATFORM_PLATFORMCONFIG_H_ */
