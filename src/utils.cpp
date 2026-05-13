#include "utils.hpp"

#include <string>
#include <vector>
#include <sstream>  // Make sure this is included for stringstream
#include <fstream>
#ifdef _WIN32
	#include <windows.h>
	#include <wininet.h>
	#pragma comment(lib, "wininet.lib")  // Link WinINet library
	#include <conio.h>
#else
	#include <cstdlib>  // For system() call on Linux
	#include <curl/curl.h>
#endif

using namespace std;

bool fileExistsOnInternet(const std::string& url) {
#ifdef _WIN32
	// Windows-specific code using WinINet API
	HINTERNET hInternet = InternetOpen("HTTP Checker", INTERNET_OPEN_TYPE_PRECONFIG, NULL, NULL, 0);
	if (!hInternet) return false;

	HINTERNET hConnect = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (!hConnect) {
		InternetCloseHandle(hInternet);
		return false;
	}

	DWORD statusCode = 0;
	DWORD length = sizeof(DWORD);

	HttpQueryInfo(hConnect, HTTP_QUERY_STATUS_CODE | HTTP_QUERY_FLAG_NUMBER, &statusCode, &length, NULL);
	InternetCloseHandle(hConnect);
	InternetCloseHandle(hInternet);

	return statusCode == 200;

#else
	// Linux-specific code using system call
	std::string command = "wget --spider -q " + url;
	int result = system(command.c_str());

	return result == 0;  // If wget returns 0, it means the file exists
#endif
}

#ifndef _WIN32
size_t write_callback(void* contents,
	size_t size,
	size_t nmemb,
	void* userp) {

	((std::string*)userp)->append(
		(char*)contents,
		size * nmemb
	);

	return size * nmemb;
}
#endif

std::string readFileFromInternet(const std::string& url) {
#ifdef _WIN32
	// Initialize an internet session
	HINTERNET hInternet = InternetOpen("File Reader", INTERNET_OPEN_TYPE_DIRECT, NULL, NULL, 0);
	if (!hInternet) {
		std::cerr << "Failed to open internet connection." << std::endl;
		return "";
	}

	// Open the URL
	HINTERNET hUrl = InternetOpenUrl(hInternet, url.c_str(), NULL, 0, INTERNET_FLAG_RELOAD, 0);
	if (!hUrl) {
		std::cerr << "Failed to open URL: " << url << std::endl;
		InternetCloseHandle(hInternet);
		return "";
	}

	// Buffer for reading data
	char buffer[4096];
	DWORD bytesRead;
	std::stringstream fileContent;

	// Read data from the URL
	while (InternetReadFile(hUrl, buffer, sizeof(buffer), &bytesRead) && bytesRead > 0) {
		fileContent.write(buffer, bytesRead);
	}

	// Clean up
	InternetCloseHandle(hUrl);
	InternetCloseHandle(hInternet);

	return fileContent.str();  // Return the full content of the file as a string
#else
	CURL* curl = curl_easy_init();
	std::string buffer;

	curl_easy_setopt(curl,
		CURLOPT_URL,
		url.c_str());

	curl_easy_setopt(curl,
		CURLOPT_WRITEFUNCTION,
		write_callback);

	curl_easy_setopt(curl,
		CURLOPT_WRITEDATA,
		&buffer);

	curl_easy_perform(curl);


	curl_easy_cleanup(curl);

	return buffer;
#endif
}

std::vector<std::string> splitString(const std::string& input, char delimiter) {
	std::vector<std::string> result;
	std::istringstream stream(input);
	std::string token;
	
	while (std::getline(stream, token, delimiter)) {
		if (!token.empty()) {
			result.push_back(token);
		}
	}

	return result;
}

bool fileExists(std::string filename) {
	fstream f(filename);
	return f.good();
}

