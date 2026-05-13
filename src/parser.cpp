#include "parser.hpp"

#include <iomanip>

std::vector<Instruction> parse(const std::string& text) {
	std::vector<Instruction> program;

	std::istringstream stream(text);
	std::string line;

	while(std::getline(stream, line)) {
		if(line.empty()) continue;
		if(line.c_str()[0] == '#') continue;

		std::istringstream ls(line);

		Instruction ins;
		ls >> ins.name;

		std::string arg;
		while(ls >> std::quoted(arg)) {
			ins.args.push_back(arg);
		}

		program.push_back(ins);
	}

	return program;
}

