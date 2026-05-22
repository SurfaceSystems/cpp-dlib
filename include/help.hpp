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
			"--validate <script>", "Check if a local script or in the web can be runned without problems."
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
