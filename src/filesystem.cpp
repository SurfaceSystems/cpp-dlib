#include "filesystem.hpp"
#include <filesystem>
#include <fstream>
#include <iostream>
#include <sys/stat.h>
#include <system_error>

namespace fs {

bool createFolder(const std::string& path) {
	try {
		return std::filesystem::create_directories(path);
	} catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Error creating folder: " << e.what() << std::endl;
		return false;
	}
}

bool remove(const std::string& path) {
	try {
		return std::filesystem::remove_all(path) > 0;
	} catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Error deleting folder: " << e.what() << std::endl;
		return false;
	}
}

bool move(const std::string& path, const std::string& destination) {
	try {
		std::filesystem::rename(path, destination);
		return true;
	} catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Error moving file/folder: " << e.what() << std::endl;
		return false;
	}
}

bool copy(const std::string& path, const std::string& to) {
	try {
		std::filesystem::copy(path, to, 
			std::filesystem::copy_options::recursive |
			std::filesystem::copy_options::overwrite_existing);
		return true;
	} catch (const std::filesystem::filesystem_error& e) {
		std::cerr << "Error copying file/folder: " << e.what() << std::endl;
		return false;
	}
}

bool write(const std::string& path, const std::string& text) {
	try {
		std::ofstream file(path);
		if (!file.is_open()) {
			std::cerr << "Error opening file for writing: " << path << std::endl;
			return false;
		}
		file << text;
		file.close();
		return true;
	} catch (const std::exception& e) {
		std::cerr << "Error writing to file: " << e.what() << std::endl;
		return false;
	}
}

bool append(const std::string& path, const std::string& text) {
	try {
		std::ofstream file(path, std::ios::app);
		if (!file.is_open()) {
			std::cerr << "Error opening file for appending: " << path << std::endl;
			return false;
		}
		file << text;
		file.close();
		return true;
	} catch (const std::exception& e) {
		std::cerr << "Error appending to file: " << e.what() << std::endl;
		return false;
	}
}

std::string read(const std::string& path) {
   try {
		std::ifstream file(path);
		if (!file.is_open()) {
			std::cerr << "Error opening file for reading: " << path << std::endl;
			return "";
		}
		
		std::string content;
		std::string line;
		while (std::getline(file, line)) {
			if (!content.empty()) content += "\n";
			content += line;
		}
		file.close();
		
		return content;
	} catch (const std::exception& e) {
		std::cerr << "Error reading file: " << e.what() << std::endl;
		return "";
	}
}

std::filesystem::path getHomeDir() {
    const char* home = std::getenv("HOME");
    if (home) return std::filesystem::path(home);
#ifdef _WIN32
    home = std::getenv("USERPROFILE");
    if (home) return fs::path(home);
    const char* drive = std::getenv("HOMEDRIVE");
    const char* path = std::getenv("HOMEPATH");
    if (drive && path) return fs::path(drive) / path;
#endif
    throw std::runtime_error("Couldn't find home directory.");
}

std::filesystem::path expandTilde(std::string path) {
    if (!path.empty() && path[0] == '~') {
        std::string rest = path.substr(1);
        return getHomeDir() / rest;
    }
    return std::filesystem::path(path);
}

} // namespace fs

