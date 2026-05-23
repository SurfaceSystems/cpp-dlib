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
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

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

			std::string url = validateUrl(argv[3]); // Get the URL

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
			Log::info("Remember to run \"dlib --repository update\" to update the scripts.");
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

			std::string url = validateUrl(argv[3]); // Get the url

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
			
			std::string url = validateUrl(argv[3]); // Get the URL

			// Check if the url is an actual dlib repository
			if(!Net::exists(url + "index")) {
				Log::error("Not a valid dlib repository.");
				return -1;
			}

			Log::success("Valid dlib repository.");
			return 0;
		} else if(subcommand == "update") {
			// Update data from all the repositories added
			std::filesystem::path dlibconfigpath = fs::getHomeDir() / ".dlib";
			std::filesystem::path repolistfile = dlibconfigpath / "repo-list";
			if(!File::exists(repolistfile.string())) {
				Log::error("There's no repository list.");
				Log::error("If you want to add a repository, use \"add\" subcommand.");
				return -1;
			}

			std::fstream list(repolistfile.string());
			std::string url;

			json allScripts = json::array();

			while(getline(list, url)) {
				if(!Net::exists(url + "index")) {
					Log::warning("Repository " + url + " doesn't contain an index file, ignoring.");
					continue;
				}

				std::string indexContent = Net::read(url + "index");
				std::vector<Instruction> indexList = simpleParse(indexContent);

				for(const Instruction& ins : indexList) {
					if(ins.name == "script") {
						std::string scriptName = ins.args[0];
						std::string scriptPath = ins.args[1];
						std::string scriptVersion = ins.args[2];

						json scriptEntry;
						scriptEntry["name"] = scriptName;
						scriptEntry["repo"] = url;
						scriptEntry["path"] = scriptPath;
						scriptEntry["version"] = scriptVersion;

						allScripts.push_back(scriptEntry);
					}
				}
			}
			list.close();

			std::filesystem::path scriptsFile = dlibconfigpath / "scripts.json";
			std::ofstream out(scriptsFile.string());
			if(out.is_open()) {
				out << allScripts.dump(4);
				out.close();
				Log::success("Updated scripts database (" + std::to_string(allScripts.size()) + " entries).");
			} else {
				Log::error("Could not write to " + scriptsFile.string());
				return -1;
			}
			return 0;
		} else if(subcommand == "help") {
			std::cout << ASCII_BOLD << "dlib --repository add <URL>: " << ASCII_RESET << "Add a repository." << std::endl <<
					 ASCII_BOLD << "dlib --repository list: " << ASCII_RESET << "List all repositories added." << std::endl <<
					 ASCII_BOLD << "dlib --repository delete <URL>: " << ASCII_RESET << "Delete a repository from the repository list." << std::endl <<
					 ASCII_BOLD << "dlib --repository test <URL>: " << ASCII_RESET << "Check if an URL is a valid repository." << std::endl <<
					 ASCII_BOLD << "dlib --repository update: " << ASCII_RESET << "Update remote scripts." << std::endl;
			return 0;
		}

		// No valid subcommand was introduced
		Log::error("Invalid subcommand.");
		Log::info("Type \"dlib --repository help\" to get a list of subcommands.");
		return -1;
	
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
	} else if(command == "install") {
		if(argc < 3) {
			Log::error("Usage: dlib install <specifier>");
			return -1;
		}
		std::string spec = argv[2];  // ex: "mank@example.com/repo:1.0"

		std::string name, repo, version;
		size_t at_pos = spec.find('@');
		size_t colon_pos = spec.rfind(':');

		if(at_pos != std::string::npos) {
			name = spec.substr(0, at_pos);
			std::string rest = spec.substr(at_pos + 1);
			if(colon_pos != std::string::npos && colon_pos > at_pos) {
				size_t colon_in_rest = colon_pos - at_pos - 1;
				repo = rest.substr(0, colon_in_rest);
				version = rest.substr(colon_in_rest + 1);
			} else {
				repo = rest;
			}
		} else {
			if(colon_pos != std::string::npos) {
				name = spec.substr(0, colon_pos);
				version = spec.substr(colon_pos + 1);
			} else {
				name = spec;
			}
		}

		std::filesystem::path dlibconfigpath = fs::getHomeDir() / ".dlib";
		std::filesystem::path scriptsFile = dlibconfigpath / "scripts.json";
		if(!File::exists(scriptsFile.string())) {
			Log::error("No scripts database found. Run 'dlib update' first.");
			return -1;
		}

		std::ifstream in(scriptsFile.string());
		json allScripts;
		try {
			in >> allScripts;
		} catch(json::parse_error& e) {
			Log::error("Failed to parse scripts.json: " + std::string(e.what()));
			return -1;
		}
		in.close();

		std::vector<json> candidates;
		for(auto& entry : allScripts) {
			if(entry["name"] != name) continue;
			if(!repo.empty() && entry["repo"] != repo) continue;
			if(!version.empty() && entry["version"] != version) continue;
			candidates.push_back(entry);
		}

		if(candidates.empty()) {
			Log::error("No script found matching: " + spec);
			return -1;
		}

		json selected;
		if(candidates.size() == 1) {
			selected = candidates[0];
		} else {
			Log::info("Multiple scripts match '" + spec + "':");
			for(size_t i = 0; i < candidates.size(); ++i) {
				std::cout << "  " << i+1 << ". " << candidates[i]["name"]
						<< " @ " << candidates[i]["repo"]
						<< " (version " << candidates[i]["version"] << ")\n";
			}
			std::cout << "Choose one (1-" << candidates.size() << "): ";
			int choice;
			std::cin >> choice;
			if(choice < 1 || choice > (int)candidates.size()) {
				Log::error("Invalid choice.");
				return -1;
			}
			selected = candidates[choice-1];
		}

		std::string repoBase = selected["repo"];
		std::string pathRel = selected["path"];
		std::string scriptName = selected["name"];
		std::string versionn = selected["version"];
		std::string url = repoBase + pathRel + versionn + ".dls";

		std::string script = Net::read(url);

		return run(script);
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

