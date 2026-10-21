/*
 * Command.cpp, part of the TinyTapeoutDIP project
 *
 *  Created on: Sep 15, 2026
 *      Author: Pat Deegan
 *  Copyright (C) 2026 Pat Deegan, https://psychogenic.com
 */

#include "includes/Command.h"
#include "includes/USBSerial.h"
#include "includes/Globals.h"
#include "includes/driver_api.h"
#include "includes/settings.h"
namespace TinyTapeout {


/*
 * no stdlib here, basic implementation of strtol
 */
static int _digit(char c, int base)
{
    int v;
    if (c >= '0' && c <= '9')      v = c - '0';
    else if (c >= 'a' && c <= 'z') v = c - 'a' + 10;
    else if (c >= 'A' && c <= 'Z') v = c - 'A' + 10;
    else return -1;
    return (v < base) ? v : -1;
}

int32_t strtol(const char *nptr, char **endptr, int base)
{
    const char *s = nptr;
    int32_t acc = 0;
    int neg = 0, d;

    while (*s == ' ' || *s == '\t' || *s == '\r' || *s == '\n')
        s++;

    if (*s == '+' || *s == '-') {
        neg = (*s == '-');
        s++;
    }

    if (base == 0) {
        if (s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
            base = 16;
        else if (s[0] == '0')
            base = 8;
        else
            base = 10;
    }

    if (base == 16 && s[0] == '0' && (s[1] == 'x' || s[1] == 'X'))
        s += 2;

    const char *start_digits = s;
    while ((d = _digit(*s, base)) >= 0) {
        acc = acc * (long)base + d;
        s++;
    }

    if (endptr)
        *endptr = (char *)((s == start_digits) ? nptr : s);

    return neg ? -acc : acc;
}











Command::Command(Type tp) : cmd_type(tp), num_params(0) {
	for (uint8_t i=0; i<COMMAND_MAX_PARAMS; i++) {
		parameters[i] = 0;
	}

}


#define TOSWAPSTR(...) snprintf(swapstr, 63, __VA_ARGS__)
#define SENDSWAPSTR(...) TOSWAPSTR(__VA_ARGS__); USBSerial::outputString(swapstr)

bool Command::execute() const {
	char swapstr[64];

	switch (cmd_type) {
		case NA:
			USBSerial::outputError("Invalid command");
			return false;
		case HELP:
			USBSerial::outputString("\r\n\r\n*****************  TinyTapeout DIP Control  ****************\r\n");
			USBSerial::outputString(        "* read_rom()                     -- Current chip           *\r\n");
			USBSerial::outputString(        "* select_design(DESID [, CLKHZ]) -- load design [clocking] *\r\n");
			USBSerial::outputString(        "* set_clock_hz(HZ)               -- start clocking         *\r\n");
			USBSerial::outputString(        "* reset_project()                -- toggle reset           *\r\n");
			USBSerial::outputString(        "* manual_clock()                 -- clock once             *\r\n");
			USBSerial::outputString(        "* dump_state()                   -- take a guess           *\r\n");
			USBSerial::outputString(        "* dump_settings()                -- current settings       *\r\n");
			USBSerial::outputString(        "* run_factory_test()             -- load + clock fact test *\r\n");
			USBSerial::outputString(        "************************************************************\r\n");
			break;
		case READ_ROM:
			USBSerial::outputDebug("Returning 'ROM' contents");
			USBSerial::outputString(TT_DUMMYROM_CONTENTS);
			break;


		case DUMP_STATE:
			USBSerial::outputInfo("Dumping state");
			SENDSWAPSTR("\r\ntt.design=%d\r\ntt.subtile=-1\r\n",Globals::projectMUX()->selected() );
			SENDSWAPSTR("tt.clk_freq=%ld\r\n", (long int)Globals::chip()->clock().rate());
			break;

		case DUMP_SETTINGS:

			USBSerial::outputInfo("Dumping settings");
			SENDSWAPSTR("set.check_ext=%u\r\n", SystemSettings.check_ext_drive_on_boot);
			SENDSWAPSTR("set.def_design=%ld\r\n", (long int)SystemSettings.default_design);
			SENDSWAPSTR("set.def_clk=%ld\r\n", (long int)SystemSettings.default_clock_rate);
			SENDSWAPSTR("set.mux_ext=%u\r\n", SystemSettings.mux_driven_externally);
			SENDSWAPSTR("set.clk_ext=%u\r\n", SystemSettings.clock_rst_driven_externally);
			SENDSWAPSTR("set.auto_reset_proj=%u\r\n", SystemSettings.reset_design_on_load);
			break;

		case RUN_FACTORY_TEST:
			return Globals::chip()->loadFactoryTest();
			break;


		case RESET_PROJECT:
			USBSerial::outputInfo("Project Reset");
			Globals::chip()->writeLow(TTDIP_PIN_PROJECT_RESET);
			Globals::chip()->delayMs(10);
			Globals::chip()->writeHigh(TTDIP_PIN_PROJECT_RESET);
			break;


		case MANUAL_CLOCK:
			USBSerial::outputInfo("Manual clocking");
			Globals::chip()->clock().stop();
			Globals::chip()->writeLow(TTDIP_PIN_CLOCK_PIN);
			Globals::chip()->delayMs(10);
			Globals::chip()->writeHigh(TTDIP_PIN_CLOCK_PIN);
			Globals::chip()->delayMs(10);
			Globals::chip()->writeLow(TTDIP_PIN_CLOCK_PIN);
			break;


		case SET_CLOCK_HZ:
			if (num_params < 1) {
				USBSerial::outputError("Need param clock rate");
				return false;
			}
			Globals::chip()->clock().stop();

			if (parameters[0]) {
				OUTPUT_INFO("Starting clock @ %ld Hz\r\n", (long int)parameters[0]);
				return Globals::chip()->clock().start(parameters[0]);
			}
			USBSerial::outputInfo("Stopped clocking");
			return true;


		case SELECT_DESIGN:
			OUTPUT_INFO("Select design %ld \r\n", (long int)parameters[0]);
			if (num_params < 1) {
				USBSerial::outputError("Need param clock rate");
				return false;

			}
			Globals::chip()->clock().stop();
			Globals::projectMUX()->enable(parameters[0]);
			if (num_params < 2 || ! parameters[1]) {
				return true;
			}
			OUTPUT_INFO("Starting clock @  %ld Hz\r\n", (long int)parameters[1]);
			return Globals::chip()->clock().start(parameters[1]);


		case RESTART:
			USBSerial::outputInfo("Restarting...");
			driver_init();
			break;
		case ABOUT:
			USBSerial::outputString("\r\nTT DIP firmware v" TTDIP_FIRMWARE_VERSION "\r\n");
			USBSerial::outputString("(C) 2026 Pat Deegan, tinytapeout.com\r\n");

			break;
		case SET_SYS_CLK_EXT:
			OUTPUT_INFO("Set external clocking %s \r\n", (parameters[0] ? "true" : "false"));

			SystemSettings.clock_rst_driven_externally = parameters[0] ? 1 : 0;
			Globals::chip()->clockAndResetControl(SystemSettings.clock_rst_driven_externally);
			break;


		case SET_SYS_MUX_EXT:
			OUTPUT_INFO("Set external MUX drive %s \r\n", (parameters[0] ? "true" : "false"));

			SystemSettings.mux_driven_externally = parameters[0] ? 1 : 0;
			Globals::chip()->clockAndResetControl(SystemSettings.mux_driven_externally);
			break;
		default:
			USBSerial::outputError("Unknown command type");
			return false;
		}


	return true;

}

void Command::extractParameters(char * bytes, uint8_t len) {
	uint8_t num_required_params = 0;
	switch (cmd_type) {
	case SET_CLOCK_HZ:
		num_required_params = 1;
		break;
	case SELECT_DESIGN:
		num_required_params = 1;
		break;
	default:
		break;
	}
	if (num_required_params) {
		USBSerial::outputDebug("Need parms");
		extractIntegers((char*)bytes, len);
	} else {
		USBSerial::outputDebug("No params required");
	}

}

uint8_t Command::extractIntegers(char * bytes, uint8_t len) {
	char* endptr = NULL;
	uint8_t i;
	for (i=0; i<len; i++) {
		if (bytes[i] == '(') {
			i++;
			break;
		}
	}
	if (i >= len) {
		return 0;
	}

	char* str = (char*)(&bytes[i]);
	while (true) {
		if (str[0] == '\0') {
			break;
		}
		int32_t v = strtol(str, &endptr, 10);
		if (v == 0 && endptr == str) {
			break;
		}
		parameters[num_params++] = v;
		str = endptr;
		while (*str != '\0' && (*str < '0' || *str > '9') ) {
			str++;
		}
	}

	return num_params;


}

void Command::dump() const {
	char buf[32];

	static CommandStringMapping nameMap[] = {
		CommandStringMapping(Command::HELP, "Help"),
		CommandStringMapping(Command::DUMP_STATE, "Dump State"),
		CommandStringMapping(Command::SELECT_DESIGN, "Design Selection"),
		CommandStringMapping(Command::SET_CLOCK_HZ, "Set Clock Rate"),
		CommandStringMapping(Command::RUN_FACTORY_TEST, "Factory Test"),
		CommandStringMapping(Command::RESET_PROJECT, "Project Reset"),
		CommandStringMapping(Command::MANUAL_CLOCK, "Manual Clocking"),
		CommandStringMapping(Command::READ_ROM, "Read ROM"),
		CommandStringMapping(Command::DUMP_SETTINGS, "Dump Settings"),
		CommandStringMapping(Command::ABOUT, "About"),
		CommandStringMapping(Command::RESTART, "Restart/init"),
		CommandStringMapping(Command::NA, NULL),
	};

	uint8_t i=0;
	while (nameMap[i].match_str != NULL) {

		if (nameMap[i].type == cmd_type) {
			USBSerial::outputDebug(nameMap[i].match_str);
		}
		i++;
	}
	if (num_params) {
		USBSerial::outputDebug(" Parameters:");
		for (i=0; i<num_params; i++) {
			snprintf(buf, 32, "\t %u: %ld", i, (long int)parameters[i]);
			USBSerial::outputDebug(buf);
		}
	}


}

} /* namespace TinyTapeout */
