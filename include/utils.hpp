#include <iostream>
#include <vector>

#pragma once

bool fileExistsOnInternet(const std::string& url);
#ifndef _WIN32
size_t write_callback(void* contents,
	size_t size,
	size_t nmemb,
	void* userp);
#endif
std::string readFileFromInternet(const std::string& url);
std::vector<std::string> splitString(const std::string& input, char delimiter);
bool fileExists(std::string filename);

