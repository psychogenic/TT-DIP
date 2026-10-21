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


#define TT_DIP_DEFAULT_DESIGN_ID		0
#define TT_DIP_DEFAULT_CLOCK_HZ			10

#define TT_DIP_RESET_DESIGN_ON_LOAD		1




/* TT_DIP_AUTO_EXT_DRIVE_CHECK:
 * if non-zero, on boot, for both the project and
 * MUX clocks, will:
 *   * set pin to input with pull up
 *   * check if driven LOW
 *   * if so, leave the pin as an input and avoid driving
 *   related subsystem (clock/reset, or MUX lines)
 *
 */
#define TT_DIP_AUTO_EXT_DRIVE_CHECK		1


/* TT_DIP_RST_FACTORY_TEST_CHECK:
 * if non-zero, on boot, will check whether the
 * project reset line is held low.  If so, the
 * factory test will be loaded and clocked
 * (assuming neither MUX nor CLOCK are driven extenally)
 *
 */
#define TT_DIP_RST_FACTORY_TEST_CHECK	1


/* MUX and CLOCK external drive defaults
 * May be overwritten if TT_DIP_AUTO_EXT_DRIVE_CHECK
 */

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
