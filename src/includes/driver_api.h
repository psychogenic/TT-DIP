/*
 * driver_api.h, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 17, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 *
 *  C-accessible API for all the C++ stuff under the hood.
 */

#ifndef SRC_INCLUDES_DRIVER_API_H_
#define SRC_INCLUDES_DRIVER_API_H_
#include "includes/TTDIPincludes.h"

#ifdef __cplusplus
extern "C" {
#endif

// driver_init -- call once to set things up
bool driver_init();

// driver_tick -- call regularly to handle pending commands
void driver_tick();

// driver_handle_input -- call everytime you get bytes incoming
bool driver_handle_input(int numbytes, uint8_t *data);


#ifdef __cplusplus
}
#endif

#endif /* SRC_INCLUDES_DRIVER_API_H_ */
