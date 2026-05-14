#include "executor.hpp"

#include <iostream>
#include "instruction.hpp"
#include <unordered_map>
#include <functional>
#include <cstdlib>
#include "progress.hpp"

Executor::Executor() {

	handlers["msg"] = [](const Instruction& ins) {
		std::cout << ins.args[0] << std::endl;
	};

	handlers["mkdir"] = [](const Instruction& ins) {
		system(("mkdir " + ins.args[0]).c_str());
	};

	handlers["download"] = [](const Instruction& ins) {
		downloadFileWithProgress(ins.args[0], ins.args[1]);
	};

	handlers["run"] = [](const Instruction& ins) {
		system(ins.args[0].c_str());
	};

}

void Executor::execute(const Instruction& ins) {
	auto it = handlers.find(ins.name);

	if (it != handlers.end()) {
		it->second(ins);
	} else {
		std::cout << "Unknown command: " << ins.name << std::endl;
	}
}

