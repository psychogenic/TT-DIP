/*
 * ProjectMux.h, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 16, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#ifndef SRC_INCLUDES_PROJECTMUX_H_
#define SRC_INCLUDES_PROJECTMUX_H_

#include "includes/platform/IC.h"
#include "settings.h"
namespace TinyTapeout {

class ProjectMux {
public:
	ProjectMux(Platform::IC * chip);
	void reset();
	void enable(DesignID id);
	void disable();

	DesignID selected() { return selected_design;}

private:
	void reset_and_clock_mux(DesignID id);

	Platform::IC chip;
	DesignID	selected_design;
};

} /* namespace TinyTapeout */

#endif /* SRC_INCLUDES_PROJECTMUX_H_ */
