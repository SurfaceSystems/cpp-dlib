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

// Main method
int main(int argc, char** argv) {

	std::string program = "- down \"hello world\"";

	std::vector<Instruction> parsedProgram;

 	try {
		parsedProgram = parse(program);
	} catch(const InvalidCommand& e) {
		Log::error(e.what());
		return -1;
	}
	Executor executor;

	executor.execute(parsedProgram[0]);


	return 0;



	// Check if no arguments were introduced by running dlib
	if(argc < 2) {
		Log::error("No argument was introduced. At least one is required.");
		Log::info("You can use \"dlib --help\" to get a list of commands.");
		return -1;
	}

	std::string command = argv[1];

	if(command == "--help") {
		// Show a list of commands for the user to know how to use dlib
		std::cout << "dlib " << ASCII_CYAN << VERSION << ASCII_RESET << " (c) " << YEAR << " Blas Fernández" << std::endl;

		return 0;
	} else if(command == "--version") {
		std::cout << "┌─────┐" << std::endl
			  << "│  ▼  │" << std::endl
			  << "│  ▼  │" << std::endl
			  << "│  ▼  │" << std::endl
			  << "│  █  │" << std::endl
			  << "└─────┘" << std::endl;
		
		std::cout << "dlib " << ASCII_CYAN << VERSION << ASCII_RESET << " Copyright (C) " << YEAR << "  Blas Fernández" << std::endl
			  << "This program comes with ABSOLUTELY NO WARRANTY." << std::endl
			  << "This is free software, and you are welcome to redistribute it" << std::endl
			  << "under certain conditions. See LICENSE file for details." << std::endl;

		return 0;
	} else if(command == "--net") {
	} else if(command == "--repository") {
	} else if(command == "--debug") {
	} else if(command == "--package") {} 


	// An invalid command was introduced
	Log::error("Unknown command: " + command);
	Log::info("Use \"dlib --help\" to get a list of commands.");
	return -1;
}

