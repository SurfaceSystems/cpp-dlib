#include "parser.hpp"

#include <iomanip>
#include "exceptions.hpp"
#include "executor.hpp"

std::vector<Instruction> parse(const std::string& text) {
	std::vector<Instruction> program;

	std::istringstream stream(text);
	std::string line;

	int lineNumber = 1;

	while(std::getline(stream, line)) {
		if(line.empty()) continue;
		if(line[0] == '#') continue;

		if(line.substr(0,2) != "- ") {
			throw InvalidCommand(lineNumber, "Invalid command: " + line);
		}

		line = line.substr(2);

		std::istringstream ls(line);

		Instruction ins;
		ls >> ins.name;

		if(!Commands::isValid(ins.name)) {
			throw InvalidCommand(lineNumber, "Unknown command: " + ins.name);
		}

		std::string arg;
		while(ls >> std::quoted(arg)) {
			ins.args.push_back(arg);
		}

		if (static_cast<size_t>(Commands::howManyArgs(ins.name)) > ins.args.size()) {
			throw NotMinimunArgs(lineNumber, "Needed " + std::to_string(Commands::howManyArgs(ins.name)) + " arguments but found " + std::to_string(ins.args.size()));
		}
		
		program.push_back(ins);

		lineNumber++;
	}

	return program;
}

