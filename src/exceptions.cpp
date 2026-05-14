#include "exceptions.hpp"

#include <iostream>
#include <ostream>
#include <string>
#include <sstream>

InvalidCommand::InvalidCommand(int li, const std::string& msg)
	: line(li), message(msg) {}

int InvalidCommand::getLine() const noexcept {
	return line;
}

const char* InvalidCommand::what() const noexcept {
	// Construir mensaje completo incluyendo la línea
	std::ostringstream oss;
	oss << message << " at line " << line;
	fullMessage = oss.str();
	return fullMessage.c_str();
}

NotMinimunArgs::NotMinimunArgs(int li, const std::string& msg)
	: line(li), message(msg) {}

int NotMinimunArgs::getLine() const noexcept {
	return line;
}

const char* NotMinimunArgs::what() const noexcept {
	// Construir mensaje completo incluyendo la línea
	std::ostringstream oss;
	oss << message << " at line " << line;
	fullMessage = oss.str();
	return fullMessage.c_str();
}

