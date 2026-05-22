/*
 * @name dlib
 * @author Blas Fernández
 * @version v0.3.0 
 * @copyright GNU GPL v3
 */

// 13-05-2026
// Oh shit, there we go again

// Include necessary functions
#include <iostream>
#include "parser.hpp"
#include "executor.hpp"
#include "log.hpp"
#include "version.hpp"
#include "colors.hpp"
#include "exceptions.hpp"
#include "utils.hpp"
#include "help.hpp"

int run(const std::string& program) {
	std::vector<Instruction> parsedProgram;

	Parser parser;

	try {
		parsedProgram = parser.parse(program);
	} catch (InvalidCommand& e) {
		Log::error(e.what());
		return -1;
	} catch (NotMinimunArgs& e) {
		Log::error(e.what());
		return -1;
	} catch (std::exception& e) {
		Log::error(e.what());
		return -1;
	}	

	if(parsedProgram.empty()) {
		Log::error("That program is empty.");
		return -1;
	}

	Executor executor;

	executor.execute(parsedProgram);

	return 0;
}

// Main method
int main(int argc, char** argv) {
	// Check if no arguments were introduced by running dlib
	if(argc < 2) {
		Log::error("No argument was introduced. At least one is required.");
		Log::info("You can use \"dlib --help\" to get a list of commands.");
		return -1;
	}

	std::string command = argv[1];

	if(command == "--help") {
		// Show a list of commands for the user to know how to use dlib
		std::cout << "dlib " << ASCII_MAGENTA << VERSION << ASCII_RESET << " (c) " << YEAR << " Blas Fernández" << std::endl;

		Help::display();

		return 0;
	} else if(command == "--version") {
		std::cout << "┌─────┐" << std::endl
			  << "│  ▼  │ dlib" << std::endl
			  << "│  ▼  │ library and program downloader" << std::endl
			  << "│  ▼  │ " << ASCII_MAGENTA << VERSION << ASCII_RESET << std::endl
			  << "│  █  │" << std::endl
			  << "└─────┘" << std::endl;
		
		std::cout << "dlib " << ASCII_MAGENTA << VERSION << ASCII_RESET << " Copyright (C) " << YEAR << "  Blas Fernández" << std::endl
			  << "This program comes with ABSOLUTELY NO WARRANTY." << std::endl
			  << "This is free software, and you are welcome to redistribute it" << std::endl
			  << "under certain conditions. See LICENSE file for details." << std::endl;

		return 0;
	} else if(command == "--repository") {
	} else if(command == "--debug") {
	}	

	if(File::exists(command)) {
		// Run a file
		std::string program = File::read(command);
	
		return run(program);
	} else if(Net::exists(command)) {
		// Run a script from the internet
		std::string program = Net::read(command);

		return run(program);
	}
 
	// An invalid command was introduced
	Log::error("Unknown command: " + command);
	Log::info("Use \"dlib --help\" to get a list of commands.");
	return -1;
}

