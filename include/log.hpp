#pragma once

#include <iostream>

namespace Log {
	void message(const std::string& msg);
	void info(const std::string& msg);
	void warning(const std::string& msg);
	void error(const std::string& msg);
	void success(const std::string& msg);
}

namespace LogFile {
	void log(const std::string& msg);
}
