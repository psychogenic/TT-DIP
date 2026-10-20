/*
 * platformIncludes.h, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 16, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#ifndef SRC_INCLUDES_PLATFORM_PLATFORMINCLUDES_H_
#define SRC_INCLUDES_PLATFORM_PLATFORMINCLUDES_H_

#include "../TTDIPincludes.h"
#ifdef DESKTOP_DEBUG
#include "desktop.h"
#else
#include "ch32fun.h"
#endif

#include "platformConfig.h"

#endif /* SRC_INCLUDES_PLATFORM_PLATFORMINCLUDES_H_ */
