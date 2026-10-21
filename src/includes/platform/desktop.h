/*
 * desktop.h, part of the TinyTapeoutDIP project
 *
 *  Created on: Oct 20, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#ifndef SRC_INCLUDES_PLATFORM_DESKTOP_H_
#define SRC_INCLUDES_PLATFORM_DESKTOP_H_

#ifdef DESKTOP_DEBUG
/* Dummy ch32fun function definition */
#define funDigitalRead(...)		1
#define SystemInit()
#define funGpioInitAll()

#define funPinMode(...)
#define funDigitalWrite(...)

#define Delay_Ms(millis) \
    struct timespec ts = { .tv_sec  = millis / 1000, .tv_nsec = (long)(millis % 1000) * 1000000L }; \
	while (nanosleep(&ts, &ts) == -1) \
	   ; /* retry if interrupted by a signal */


typedef enum {
	GPIO_Speed_In = 0,
	GPIO_Speed_10MHz,
	GPIO_Speed_2MHz,
	GPIO_Speed_50MHz
} GPIOSpeed_TypeDef;

#endif /* DESKTOP_DEBUG */

#endif /* SRC_INCLUDES_PLATFORM_DESKTOP_H_ */
