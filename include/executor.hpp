#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include "instruction.hpp"
#include <unordered_set>

namespace Commands {
	inline const std::unordered_set<std::string> validCommands = 
	{
		"msg"
	};
    
	inline bool isValid(const std::string& cmd) {
		return validCommands.find(cmd) != validCommands.end();
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

