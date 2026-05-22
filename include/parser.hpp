#pragma once

#include "instruction.hpp"
#include <string>
#include <vector>

class Parser {
public:
	std::vector<Instruction> parse(const std::string& script);
	
private:
	Instruction parseLine(const std::string& line, int lineNumber);
	
	bool isControlKeyword(const std::string& keyword);
	
	std::string trim(const std::string& str);

	std::vector<std::string> parseArgumentsByComma(const std::string& line);
};

