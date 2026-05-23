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
#include "filesystem.hpp"
#include <filesystem>

// Recieving the content of a script, parse it and run it.
int run(const std::string& program) {
	std::vector<Instruction> parsedProgram; // Initialize a vector of Instructions for the program

	Parser parser; // Initialize the parser

	try {
		parsedProgram = parser.parse(program); // Parse the program
	} catch (InvalidCommand& e) { // An unknown command was used
		Log::error(e.what());
		return -1;
	} catch (NotMinimunArgs& e) { // The minimun requirement of arguments wasn't satisfied
		Log::error(e.what());
		return -1;
	} catch (std::exception& e) { // Any other problem has occured
		Log::error(e.what());
		return -1;
	}	

	if(parsedProgram.empty()) { // Check if the program is empty
		Log::error("That program is empty.");
		return -1;
	}

	Executor executor; // Initialize the executor

	executor.execute(parsedProgram); // Execute the program

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
		// Display current version
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
	
		if(argc < 3) { // Two arguments are needed at least
			Log::error("Needed at least 2 arguments");
			Log::info("Type \"dlib --repository help\" to get a list of commands");
			return -1;
		}

		std::string subcommand = argv[2]; // Get the subcommand

		if(subcommand == "add") {
			// Add a repository for dlib

			std::filesystem::path repolistfile = fs::getHomeDir() / ".dlib" / "repo-list"; 
			std::filesystem::path dlibFolder = fs::getHomeDir() / ".dlib";

			if(!File::exists(repolistfile.string())) { // Check if the repository list already exists
				Log::warning("dlib config folder wasn't created, creating it now.");
				fs::createFolder(dlibFolder.string()); // Create dlib folder if it doesn't exist
			}

			// Check if the user introduced an URL for a repository
			if(argc < 4) {
				Log::error("Needed an URL for the repository.");
				Log::info("Usage: dlib --repository add <URL>");
				return -1;
			}

			std::string url = argv[3]; // Get the URL

			// Check if the url is an actual dlib repository
			if(!Net::exists(url + "index")) {
				Log::error("That url isn't a valid dlib repository.");
				Log::info("Check for spelling errors.");
				return -1;
			}

			// Check if the repository was already added
			if(fs::contains(repolistfile.string(), url)) {
				Log::warning("That repository is already added, not adding it again.");
				return -1;
			}

			// Add the repository to the repository list
			fs::append(repolistfile.string(), url + "\n");
			Log::success("Repository succesfully added!");
			return 0;
		} else if(subcommand == "list") {
			std::filesystem::path repolistfile = fs::getHomeDir() / ".dlib" / "repo-list"; 

			if(!File::exists(repolistfile.string())) { // Check if the repository list exists
				Log::error("There's no repository list.");
				Log::error("If you want to add a repository, use \"add\" subcommand.");
				return -1;
			}

			std::string list = File::read(repolistfile.string());

			if(list.empty()) {
				Log::warning("Empty repository list.");
				return -1;
			}
			
			std::cout << list << std::endl;

			return 0;
		} else if(subcommand == "delete") {
			// Check if the user introduced an url to delete
			if(argc < 4) {
				Log::error("You need to introduce an URL to delete.");
				Log::info("Usage: dlib --repository delete <URL>");
				return -1;
			}

			std::string url = argv[3]; // Get the url

			std::filesystem::path repolistfile = fs::getHomeDir() / ".dlib" / "repo-list"; 

			if(!File::exists(repolistfile.string())) { // Check if the repository list exists
				Log::error("There's no repository list.");
				Log::error("If you want to add a repository, use \"add\" subcommand.");
				return -1;
			}
			
			fs::replace(repolistfile.string(), url, "");
			return 0;
		} else if(subcommand == "test") {
			// Check if the user introduced an url to test 
			if(argc < 4) {
				Log::error("You need to introduce an URL to test.");
				Log::info("Usage: dlib --repository test <URL>");
				return -1;
			}
			
			std::string url = argv[3]; // Get the URL

			// Check if the url is an actual dlib repository
			if(!Net::exists(url + "index")) {
				Log::error("Not a valid dlib repository.");
				return -1;
			}

			Log::success("Valid dlib repository.");
			return 0;
		} else if(subcommand == "help") {}

		// No valid subcommand was introduced
		Log::error("Invalid subcommand.");
		Log::info("Type \"dlib --repository help\" to get a list of subcommands.");
		return -1;
	
	} else if(command == "--debug") {
	} else if(command == "--validate") {
		// Check if a script can be runned without any errors

		if(argc < 3) { // Check if the user introduced a script as an argument
			Log::error("Needed a dls script.");
			Log::info("Usage: dlib --validate script.dls");
			return -1;
		}

		std::string script = argv[2]; // Get the script

		Parser parser; // Initilize parser

		try {
			if(File::exists(script)) { // Check if the script exists locally
				parser.parse(File::read(script));
			} else if(Net::exists(script)) { // Check if the script exists in the web
				parser.parse(Net::read(script));
			} else { // The script doesn't exist anywhere
				Log::error("Script not found.");
				return -1;
			}
		} catch(std::exception& e) { // An error was thrown, the script is invalid
			Log::error(e.what());
			Log::error("Invalid script");
			return -1;
		}

		// If this place is reached, no errors have occured
		// The script is valid.
		Log::info("Valid script");
		return 0;
	}	

	// As no valid program argument has been introduced, check if the file exists locally or in the web
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

