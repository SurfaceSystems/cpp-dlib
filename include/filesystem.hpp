#pragma once

#include <iostream>
#include <filesystem>

namespace fs {
	bool createFolder(const std::string& path);
	
	bool remove(const std::string& path);

	bool move(const std::string& path, const std::string& destination);
	bool copy(const std::string& path, const std::string& to);

	bool write(const std::string& path, const std::string& text);
	bool append(const std::string& path, const std::string& text);
	std::string read(const std::string& path);

	std::filesystem::path expandTilde(std::string path);
	std::filesystem::path getHomeDir();
}
