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
		if(line.empty() || line[0] == '#') continue;
		
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
		
		std::string argsLine;
		std::getline(ls, argsLine);
		
		std::string current;
		bool inQuotes = false;
		int parenDepth = 0;
		int bracketDepth = 0;
		bool inFunction = false;
		
		for (size_t i = 0; i < argsLine.length(); i++) {
			char c = argsLine[i];
			
			if (c == '"' && !inFunction) {
				inQuotes = !inQuotes;
				current += c;
			}
			else if (c == '=' && !inQuotes && !inFunction && 
					 (i + 1 < argsLine.length() && isalpha(argsLine[i+1]))) {
				inFunction = true;
				current += c;
			}
			else if (c == '(' && (inFunction || inQuotes)) {
				parenDepth++;
				current += c;
			}
			else if (c == ')' && (inFunction || inQuotes)) {
				parenDepth--;
				current += c;
				if (parenDepth == 0 && inFunction) {
					inFunction = false;
				}
			}
			else if (c == '[' && !inQuotes) {
				bracketDepth++;
				current += c;
			}
			else if (c == ']' && !inQuotes) {
				bracketDepth--;
				current += c;
			}
			else if (c == ',' && !inQuotes && !inFunction && parenDepth == 0 && bracketDepth == 0) {
				size_t start = current.find_first_not_of(" \t");
				size_t end = current.find_last_not_of(" \t");
				if (start != std::string::npos) {
					ins.args.push_back(current.substr(start, end - start + 1));
				} else if (!current.empty()) {
					ins.args.push_back(current);
				}
				current.clear();
			}
			else {
				current += c;
			}
		}
		
		if (!current.empty()) {
			size_t start = current.find_first_not_of(" \t");
			size_t end = current.find_last_not_of(" \t");
			if (start != std::string::npos) {
				ins.args.push_back(current.substr(start, end - start + 1));
			} else if (!current.empty()) {
				ins.args.push_back(current);
			}
		}
		
		int minArgs = Commands::howManyArgs(ins.name);
		if (static_cast<size_t>(minArgs) > ins.args.size()) {
			throw NotMinimunArgs(lineNumber, 
				"Needed " + std::to_string(minArgs) + 
				" arguments but found " + std::to_string(ins.args.size()));
		}
		
		program.push_back(ins);
		lineNumber++;
	}
	
	return program;
}
