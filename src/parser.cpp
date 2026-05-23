#include "parser.hpp"

#include <sstream>
#include <iostream>
#include <cctype>
#include <algorithm>
#include <stdexcept>
#include <stack>
#include "executor.hpp"
#include "exceptions.hpp"
#include <iomanip>

std::vector<Instruction> Parser::parse(const std::string& script) {
	std::vector<Instruction> program;
	std::istringstream stream(script);
	std::string line;
	int lineNumber = 1;
	
	std::stack<int> ifStack;
	std::stack<int> whileStack;
	
	while (std::getline(stream, line)) {
		if (line.empty() || line[0] == '#') {
			lineNumber++;
			continue;
		}
		
		try {
			Instruction ins = parseLine(line, lineNumber);
			program.push_back(ins);
			
			size_t currentIndex = program.size() - 1;
			
			if (ins.type == InstructionType::IF) {
				ifStack.push(currentIndex);
			}
			else if (ins.type == InstructionType::WHILE) {
				whileStack.push(currentIndex);
			}
			else if (ins.type == InstructionType::ENDIF) {
				if (!ifStack.empty()) {
					int ifIndex = ifStack.top();
					ifStack.pop();
					program[ifIndex].jumpTarget = currentIndex;
				} else {
					throw std::runtime_error("ENDIF without IF");
				}
			}
			else if (ins.type == InstructionType::ENDWHILE) {
				if (!whileStack.empty()) {
					int whileIndex = whileStack.top();
					whileStack.pop();
					program[currentIndex].jumpTarget = whileIndex;
					program[whileIndex].jumpTarget = currentIndex;
				} else {
					throw std::runtime_error("ENDWHILE without WHILE");
				}
			}
			
		} catch (const std::exception& e) {
			throw std::runtime_error("Line " + std::to_string(lineNumber) + ": " + e.what());
		}
		
		lineNumber++;
	}
	
	if (!ifStack.empty()) {
		throw std::runtime_error("Unclosed IF at end of file");
	}
	if (!whileStack.empty()) {
		throw std::runtime_error("Unclosed WHILE at end of file");
	}
	
	return program;
}

Instruction Parser::parseLine(const std::string& line, int lineNumber) {
	std::string trimmedLine = trim(line);
	
	bool hasPrefix = (trimmedLine.substr(0, 2) == "- ");
	
	std::string content = hasPrefix ? trimmedLine.substr(2) : trimmedLine;
	std::istringstream iss(content);
	
	std::string keyword;
	iss >> keyword;
	
	std::string lowerKeyword = keyword;
	std::transform(lowerKeyword.begin(), lowerKeyword.end(), lowerKeyword.begin(), ::tolower);
	
	Instruction ins;
	
	if (lowerKeyword == "if") {
		ins.type = InstructionType::IF;
		ins.name = "if";
		
		std::string condition;
		std::getline(iss, condition);
		ins.args.push_back(trim(condition));
		
	} else if (lowerKeyword == "else") {
		ins.type = InstructionType::ELSE;
		ins.name = "else";
		
	} else if (lowerKeyword == "endif") {
		ins.type = InstructionType::ENDIF;
		ins.name = "endif";
		
	} else if (lowerKeyword == "while") {
		ins.type = InstructionType::WHILE;
		ins.name = "while";
		
		std::string condition;
		std::getline(iss, condition);
		ins.args.push_back(trim(condition));
		
	} else if (lowerKeyword == "endwhile") {
		ins.type = InstructionType::ENDWHILE;
		ins.name = "endwhile";
		
	} else if (lowerKeyword == "break") {
		ins.type = InstructionType::BREAK;
		ins.name = "break";
		
	} else if (lowerKeyword == "continue") {
		ins.type = InstructionType::CONTINUE;
		ins.name = "continue";
		
	} else {
		if (!hasPrefix && !isControlKeyword(lowerKeyword)) {
			throw std::runtime_error("Invalid command format. Expected '- ' prefix: " + line);
		}
		
		ins.type = InstructionType::NORMAL;
		ins.name = keyword;

		std::string argsLine;
		std::getline(iss, argsLine);
		ins.args = parseArgumentsByComma(argsLine);	

		if(Commands::howManyArgs(ins.name) > ins.args.size()) {
			throw NotMinimunArgs(lineNumber, "Not enough arguments. Needed " + std::to_string(Commands::howManyArgs(ins.name)) + " but found " + std::to_string(ins.args.size()));
		}

	}
	
	return ins;
}

bool Parser::isControlKeyword(const std::string& keyword) {
	std::string lower = keyword;
	std::transform(lower.begin(), lower.end(), lower.begin(), ::tolower);
	
	return (lower == "if" || lower == "else" || lower == "endif" ||
			lower == "while" || lower == "endwhile" || 
			lower == "break" || lower == "continue");
}

std::string Parser::trim(const std::string& str) {
	size_t start = str.find_first_not_of(" \t\n\r\f\v");
	if (start == std::string::npos) return "";
	size_t end = str.find_last_not_of(" \t\n\r\f\v");
	return str.substr(start, end - start + 1);
}

std::vector<std::string> Parser::parseArgumentsByComma(const std::string& line) {
	std::vector<std::string> args;
	std::string current;
	bool inQuotes = false;
	int parenDepth = 0;
	bool inFunction = false;
	
	for (size_t i = 0; i < line.length(); i++) {
		char c = line[i];
		
		if (c == '"' && !inFunction) {
			inQuotes = !inQuotes;
			current += c;
		}
		else if (c == '=' && !inQuotes && i + 1 < line.length() && isalpha(line[i+1])) {
			inFunction = true;
			current += c;
		}
		else if (c == '(' && inFunction) {
			parenDepth++;
			current += c;
		}
		else if (c == ')' && inFunction) {
			parenDepth--;
			current += c;
			if (parenDepth == 0) {
				inFunction = false;
			}
		}
		else if (c == ',' && !inQuotes && !inFunction && parenDepth == 0) {
			args.push_back(trim(current));
			current.clear();
		}
		else {
			current += c;
		}
	}
	
	if (!current.empty()) {
		args.push_back(trim(current));
	}
	
	return args;
}

std::vector<Instruction> simpleParse(const std::string& text) {
	std::vector<Instruction> program;

	std::istringstream stream(text);
	std::string line;

	while(std::getline(stream, line)) {
		if(line.empty()) continue;
		if(line.c_str()[0] == '#') continue;

		if(line.substr(0,2) != "- ") {
			continue;
		}

		line = line.substr(2);
		
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

