#pragma once

#include <iostream>
#include <vector>

namespace Help {
	inline static std::vector<std::vector<std::string>> help = {
		{
			"--help", "Show a list of dlib commands."
		},
		{
			"--version", "Display current dlib version."
		},
		{
			"<.dls file>", "Run a local dlib script."
		},
		{
			"<url>", "Run a script from the internet."
		}
	};

	extern void display();
}
