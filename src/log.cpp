#include "log.hpp"

#include "colors.hpp"
#include <fstream>

void Log::message(const std::string& msg) {
	std::cout << msg << std::endl;
}

void Log::info(const std::string& msg) {
	std::cout << ASCII_BLUE << msg << ASCII_RESET << std::endl;
}

void Log::warning(const std::string& msg) {
	std::cout << ASCII_YELLOW << msg << ASCII_RESET << std::endl;
}

void Log::error(const std::string& msg) {
	std::cout << ASCII_RED << msg << ASCII_RESET << std::endl;
}

void LogFile::log(const std::string& msg) {
	std::fstream file("dlib.log");

	file << msg << std::endl;

	file.close();
}

