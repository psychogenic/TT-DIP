/*
 * CommandInterpreter.cpp, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 15, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#include "includes/CommandInterpreter.h"
#include "includes/USBSerial.h"
namespace TinyTapeout {
#define CMDINT_MAX_SWAP_LEN (COMMAND_INTERPRETER_INBUFLEN + 16)
static char out_swap_buf[CMDINT_MAX_SWAP_LEN + 1];

#define OUPUT_SWAP(func, tpl, contentstr) \
		snprintf(out_swap_buf, CMDINT_MAX_SWAP_LEN, tpl, contentstr); \
		USBSerial::func(out_swap_buf)

CommandInterpreter::CommandInterpreter() : buf_len(0), cmd_queue_len(0) {
	memset(in_buffer, 0, COMMAND_INTERPRETER_INBUFLEN + 1);

}


bool CommandInterpreter::appendInputData(uint8_t * bytes, uint8_t len) {
	uint8_t cmds_parsed = 0;
	for (uint8_t i=0; i<len; i++) {
		if (bytes[i] == '\r' || bytes[i] == '\n') {
			in_buffer[buf_len] = '\0';
			if (buf_len) {

				OUPUT_SWAP(outputDebug, "Parsing '%s'", (char*)in_buffer);
				if (parseCommand(in_buffer, buf_len)) {
					cmds_parsed++;
				} else {
					OUPUT_SWAP(outputError, "Unrecognized '%s'", (char*)in_buffer);
					USBSerial::outputPrompt();
				}
				buf_len = 0; // reset buffer regardless of whether command was parsed
			} else {
				USBSerial::outputPrompt();
			}
		} else if (bytes[i] == ' ' || bytes[i] == '\t' || bytes[i] == ';') {

			// USBSerial::outputDebug("skip whitespace");
		} else {
			if (buf_len < COMMAND_INTERPRETER_INBUFLEN) {
				in_buffer[buf_len++] = bytes[i];
			} else {
				USBSerial::outputError("cmd int buf overflow, flush.");
				buf_len = 0;
			}
		}
	}

	if (cmds_parsed) {
		return true;
	}
	return false;
}


static CommandStringMapping CmdStringMatchMap[] = {
			CommandStringMapping(Command::HELP, "?"),
			CommandStringMapping(Command::HELP, "help"),
			CommandStringMapping(Command::DUMP_STATE, "dump_state"),
			CommandStringMapping(Command::SELECT_DESIGN, "select_design"),
			CommandStringMapping(Command::SET_CLOCK_HZ, "set_clock_hz"),
			CommandStringMapping(Command::RUN_FACTORY_TEST, "run_factory_test"),
			CommandStringMapping(Command::RESET_PROJECT, "reset_project"),
			CommandStringMapping(Command::MANUAL_CLOCK, "manual_clock"),
			CommandStringMapping(Command::READ_ROM, "read_rom"),
			CommandStringMapping(Command::ABOUT, "about"),
			CommandStringMapping(Command::RESTART, "restart"),
			CommandStringMapping(Command::SET_SYS_MUX_EXT, "set_mux_ext"),
			CommandStringMapping(Command::SET_SYS_CLK_EXT, "set_clk_ext"),

			CommandStringMapping(Command::NA, NULL),

	};


bool CommandInterpreter::parseCommand(uint8_t * bytes, uint8_t len)
{

	uint8_t i=0;
	while (CmdStringMatchMap[i].match_str != NULL) {
		size_t mxlen = strlen(CmdStringMatchMap[i].match_str);
		if (len < mxlen) {
			mxlen = len;
		}
		if (strncmp(CmdStringMatchMap[i].match_str, (const char*)bytes, mxlen) == 0) {
			// a match!
			OUPUT_SWAP(outputDebug, "Found '%s'", CmdStringMatchMap[i].match_str);

			if (cmd_queue_len >= COMMAND_INTERPRETER_CMDQUEUELEN) {
				USBSerial::outputError("Out of space in cmd Q");
				return true;
			}

			cmd_queue[cmd_queue_len] = Command(CmdStringMatchMap[i].type);
			cmd_queue[cmd_queue_len].extractParameters((char*)bytes, len);

			cmd_queue_len++;
			return true;
		}
		i++;
	}

	return false;


}
const Command * CommandInterpreter::commandsQueued() {
	return cmd_queue;
}
void CommandInterpreter::clearCommandsQueued() {
	Command clrd(Command::NA);
	for (uint8_t i=0; i<cmd_queue_len; i++) {
		cmd_queue[cmd_queue_len] = clrd;
	}
	cmd_queue_len = 0;
}

} /* namespace TinyTapeout */
