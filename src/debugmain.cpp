/*
 * debugmain.cpp, part of the TinyTapeoutDIP project
 *
 * Used to test things on desktop.
 * Just type in commands, like
 *   set_clock_hz(12345)
 * and it'll respond.
 *
 *
 * Needs ncurses.
 *
 *  Created on: Sep 15, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */
#ifdef DESKTOP_DEBUG
#include "includes/driver_api.h"
#include "includes/Globals.h"

#include <iostream>
#include <ncurses.h>

int kbhit(void)
{
    int ch = getch();

    if (ch != ERR) {
        ungetch(ch);
        return 1;
    } else {
        return 0;
    }
}
int main(void) {
	char c[1];

	if (! driver_init()) {
		std::cerr << "Could not init chip??" << std::endl;
		return 1;
	}
	TinyTapeout::Globals::chip()->delayMs(1500);
	std::cout << std::endl;



	initscr();
	cbreak();
	// noecho();
	nodelay(stdscr, TRUE);
	scrollok(stdscr, TRUE);
	for (;;) {

		if (kbhit()) {

			c[0] = getch();
			refresh();
			driver_handle_input(1, (uint8_t*)c);
			driver_tick();
			/*
			for (uint8_t i=0; i<cmdInt.numCommandsReceived(); i++) {
				const TinyTapeout::Command & curCommand = cmdInt.commandsQueued()[i];
				serial.outputInfo("Have command");
				curCommand.dump();
				curCommand.execute();
			}
			cmdInt.clearCommandsQueued();
			*/
		}



	}
	return 0;
}

#endif /* DESKTOP_DEBUG */
