#include "help.hpp"

#include <iostream>
#include <string>
#include "colors.hpp"

void Help::display() {
	for(std::vector<std::string> item : Help::help) {
		std::cout << ASCII_BOLD << "dlib " << item[0] << ": " << ASCII_RESET << item[1] << std::endl;
	}
}

