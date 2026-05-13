#pragma once

#include <string>
#include <unordered_map>
#include <functional>
#include "instruction.hpp"

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

