/*
 * platformConfig.h, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 16, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#ifndef SRC_INCLUDES_PLATFORM_PLATFORMCONFIG_H_
#define SRC_INCLUDES_PLATFORM_PLATFORMCONFIG_H_

/* TTDIP_NUM_CLOCKS_ON_PROJ_RESET
 * num clock toggles to do while in reset
 */
#define TTDIP_NUM_CLOCKS_ON_PROJ_RESET	5

#ifndef DESKTOP_DEBUG
/* actual MCU pins and ports */

#define TTDIP_PIN_CLOCK_PORT		GPIOA

/*
 * TTDIP_PIN_CLOCK_PIN -- original proto had
 * clock on PA5, ie the SCK line, but this blasted
 * thing doesn't support PWM.
 *
 * Moved to PA3, which does... for dev, have some
 * tweaks here to use PA5 and use a timer/irq to manually
 * clock, ugh.
 *
 * TODO: FIXME -- set to PA3 for new boards
 */
#define TTDIP_PIN_CLOCK_PIN			PA5
// #define TTDIP_PIN_CLOCK_PIN		PA3



// Define TTDIP_CLOCK_PULLUP_ENABLE to have the
// clock line pulled up by default, comment out
// to float
#define TTDIP_CLOCK_PULLUP_ENABLE


#define TTDIP_PIN_MUX_EN			PC0
#define TTDIP_PIN_MUX_RESET			PC3
#define TTDIP_PIN_MUX_INC			PA0

#define TTDIP_PIN_PROJECT_RESET		PA4

#define TTDIP_PIN_UIIN0				PB3


#else
/* desktop sim ... values matter little */
#define TTDIP_PIN_CLOCK_PORT		0
#define TTDIP_PIN_CLOCK_PIN			22

#define TTDIP_PIN_PROJECT_RESET		4

#define TTDIP_PIN_MUX_EN			1
#define TTDIP_PIN_MUX_RESET			2
#define TTDIP_PIN_MUX_INC			3

#define TTDIP_PIN_UIIN0				6i

#endif /* DESKTOP_DEBUG */


#if defined(PA3) && TTDIP_PIN_CLOCK_PIN == PA3
#define TTDIP_CLOCK_USE_PA3
#define TTDIP_PIN_CLOCK_PORTPIN		3
#error HO
#else
#define TTDIP_CLOCK_USE_TIM3
#define TTDIP_PIN_CLOCK_PORTPIN		5
#endif

#endif /* SRC_INCLUDES_PLATFORM_PLATFORMCONFIG_H_ */
