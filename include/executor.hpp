#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include "instruction.hpp"

namespace Commands {
	inline const std::unordered_map<std::string, int> commandArgs = {
		{"msg", 1},
		{"mkdir", 1},
		{"download", 2},
		{"run", 1},
		{"info", 1},
		{"warning", 1},
		{"error", 1},
		{"log", 1}
	};

	inline bool isValid(const std::string& cmd) {
		return commandArgs.find(cmd) != commandArgs.end();
	}

	inline int howManyArgs(const std::string& cmd) {
		auto it = commandArgs.find(cmd);
		if (it != commandArgs.end()) {
			return it->second;
		}
		return -1;
	}	
}

class Executor {
public:
	Executor();

	void execute(const Instruction& ins);

private:
	std::unordered_map<
		std::string,
		std::function<void(const Instruction&)>
	> handlers;
};

