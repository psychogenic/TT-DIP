/*
 * CommandInterpreter.h, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 15, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#ifndef SRC_COMMANDINTERPRETER_H_
#define SRC_COMMANDINTERPRETER_H_

#include "TTDIPincludes.h"
#include "Command.h"

namespace TinyTapeout {

class CommandInterpreter {
	/*
	 * Send all received data to the interpreter using
	 * 	appendInputData()
	 * then check numCommandsReceived() to see if any are queued.
	 * If so:
	 *
	 * 	* get the pointer to the queue
	 * 	  Command * cmds = commandsQueued()
	 * 	* handle each command as appropriate
	 * 	  for (uint8_t i=0; i< numCommandsReceived(); i++) {
	 * 	  	// deal with cmds[i]
	 * 	  }
	 *
	 * 	* clear that queue
	 * 	  clearCommandsQueued()
	 */
public:
	CommandInterpreter();

	bool appendInputData(uint8_t * bytes, uint8_t len);

	uint8_t numCommandsReceived() { return cmd_queue_len;}
	const Command * commandsQueued();
	void clearCommandsQueued();


private:
	bool parseCommand(uint8_t * bytes, uint8_t len);





	uint8_t in_buffer[COMMAND_INTERPRETER_INBUFLEN + 1];
	uint8_t buf_len;
	Command cmd_queue[COMMAND_INTERPRETER_CMDQUEUELEN];
	uint8_t cmd_queue_len;

};

} /* namespace TinyTapeout */

#endif /* SRC_COMMANDINTERPRETER_H_ */
