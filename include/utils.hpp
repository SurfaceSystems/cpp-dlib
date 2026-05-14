#include <iostream>
#include <vector>

#pragma once

#ifndef _WIN32
size_t write_callback(void* contents,
	size_t size,
	size_t nmemb,
	void* userp);
#endif
std::vector<std::string> splitString(const std::string& input, char delimiter);

namespace File {
	extern bool exists(const std::string& filename);
	extern std::string read(const std::string& filename);
}

namespace Net {
	extern bool exists(const std::string& url);
	extern std::string read(const std::string& url);
}

