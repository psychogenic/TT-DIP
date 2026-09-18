/*
 * projectselect.h, part of the TinyTapeoutDIP project
 *
 * We have no flash.  To enable a project to be loaded on powerup,
 * set the design ID (and optional clock rate) here, and re-build.
 *
 * It will then load the project at start, but still allow projects to
 * be selected at will.
 *
 *  Created on: Sep 17, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#ifndef SRC_PROJECTSELECT_H_
#define SRC_PROJECTSELECT_H_

/*
 * Default project loaded
 *
 * If TT_DIP_DEFAULT_DESIGN_ID is non-zero,
 * this project will be loaded, and reset, on startup.
 *
 * If TT_DIP_DEFAULT_CLOCK_HZ is also non-zero, it will
 * be clocked as well (if TT_DIP_RESET_DESIGN_ON_LOAD)
 *
 * Note: only works according to whether you've set or not
 * the external drive for MUX/CLOCK (see below
 *
 */
#define TT_DIP_DEFAULT_DESIGN_ID	0
#define TT_DIP_DEFAULT_CLOCK_HZ		10

#define TT_DIP_RESET_DESIGN_ON_LOAD	1


/*
 * MUX drive externally
 *
 * set TT_DIP_MUX_EXTERNALLY_DRIVEN non-zero
 * to have this system avoid driving the MUX lines
 */
#define TT_DIP_MUX_EXTERNALLY_DRIVEN				0

/*
 * CLOCK/RESET drive externally
 *
 *
 * set TT_DIP_CLOCK_AND_RESET_EXTERNALLY_DRIVEN non-zero
 * to have this system avoid touching the project
 * CLK and ~RESET lines
 */
#define TT_DIP_CLOCK_AND_RESET_EXTERNALLY_DRIVEN	0




#endif /* SRC_PROJECTSELECT_H_ */
