/*
 * settings.c, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 17, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#include "includes/settings.h"
#include "includes/TTDIPconfig.h"
#include "projectselect.h"

#ifndef TT_DIP_AUTO_EXT_DRIVE_CHECK
#define TT_DIP_AUTO_EXT_DRIVE_CHECK		1
#endif

#ifndef TT_DIP_RST_FACTORY_TEST_CHECK
#define TT_DIP_RST_FACTORY_TEST_CHECK 	1
#endif

#ifndef TT_DIP_DEFAULT_DESIGN_ID
#define TT_DIP_DEFAULT_DESIGN_ID	0
#endif

#ifndef TT_DIP_DEFAULT_CLOCK_HZ
#define TT_DIP_DEFAULT_CLOCK_HZ		0
#endif

#ifndef TT_DIP_MUX_EXTERNALLY_DRIVEN
#define TT_DIP_MUX_EXTERNALLY_DRIVEN 	0
#endif

#ifndef TT_DIP_CLOCK_AND_RESET_EXTERNALLY_DRIVEN
#define TT_DIP_CLOCK_AND_RESET_EXTERNALLY_DRIVEN	0
#endif

#ifndef TT_DIP_RESET_DESIGN_ON_LOAD
#define TT_DIP_RESET_DESIGN_ON_LOAD 1
#endif


Settings_t SystemSettings = {
		.check_ext_drive_on_boot = TT_DIP_AUTO_EXT_DRIVE_CHECK,
		.check_reset_factory_test_on_boot = TT_DIP_RST_FACTORY_TEST_CHECK,
		.default_design = TT_DIP_DEFAULT_DESIGN_ID,
		.default_clock_rate = TT_DIP_DEFAULT_CLOCK_HZ,
		.echo_on = TTDIP_USB_SERIAL_ECHO,
		.mux_driven_externally = TT_DIP_MUX_EXTERNALLY_DRIVEN,
		.clock_rst_driven_externally = TT_DIP_CLOCK_AND_RESET_EXTERNALLY_DRIVEN,
		.reset_design_on_load = TT_DIP_RESET_DESIGN_ON_LOAD
};
