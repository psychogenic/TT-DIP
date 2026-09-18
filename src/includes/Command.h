/*
 * Command.h, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 15, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 *
 *
 *  We *could* use a bunch of polymorphism, but that would prob mean
 *  using pointers/dynamic allocations and a bunch of classes, instead
 *  this Command class acts kind of like a factory but just knows
 *  everything about the "subclasses" and handles things in one cpp file.
 *
 *
 */

#ifndef SRC_COMMAND_H_
#define SRC_COMMAND_H_
#include "TTDIPincludes.h"
namespace TinyTapeout {

class Command {

public:
	typedef enum cmdtype {
		NA=0,
		HELP,
		READ_ROM,
		DUMP_STATE,
		SELECT_DESIGN,
		SET_CLOCK_HZ,
		RUN_FACTORY_TEST,
		RESET_PROJECT,
		MANUAL_CLOCK,

		SET_SYS_MUX_EXT,
		SET_SYS_CLK_EXT,
		ABOUT,

		RESTART
	} Type;
	Command(Type tp=NA);

	Type type() const {return cmd_type;}
	void extractParameters(char * bytes, uint8_t len);
	void dump() const;

	bool execute() const;



private:
	Type cmd_type;
	int32_t parameters[COMMAND_MAX_PARAMS];
	uint8_t num_params;
	uint8_t extractIntegers(char * bytes, uint8_t len);
};


typedef struct commandstrmap {
	Command::Type type;
	const char * match_str;
	commandstrmap(Command::Type t, const char * cmpstr) : type(t), match_str(cmpstr) {

	}

} CommandStringMapping;


} /* namespace TinyTapeout */

#endif /* SRC_COMMAND_H_ */
