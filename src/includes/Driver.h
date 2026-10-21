/*
 * Driver.h, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 17, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#ifndef SRC_DRIVER_H_
#define SRC_DRIVER_H_

namespace TinyTapeout {

class Driver {
public:
	Driver();
	bool init();
	void tick();

private:
	void initExtDriveCheck();
	bool initFactoryTestCheck();


};

} /* namespace TinyTapeout */

#endif /* SRC_DRIVER_H_ */
