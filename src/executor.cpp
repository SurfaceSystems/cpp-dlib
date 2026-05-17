#include "executor.hpp"

#include <iostream>
#include "instruction.hpp"
#include <unordered_map>
#include <functional>
#include <cstdlib>
#include "progress.hpp"
#include "log.hpp"
#include "filesystem.hpp"

Executor::Executor() {

	handlers["msg"] = [](const Instruction& ins) {
		std::cout << ins.args[0] << std::endl;
	};

	handlers["download"] = [](const Instruction& ins) {
		downloadFileWithProgress(ins.args[0], ins.args[1]);
	};

	handlers["run"] = [](const Instruction& ins) {
		system(ins.args[0].c_str());
	};

	handlers["info"] = [](const Instruction& ins) {
		Log::info(ins.args[0]);
	};

	handlers["warning"] = [](const Instruction& ins) {
		Log::warning(ins.args[0]);
	};

	handlers["error"] = [](const Instruction& ins) {
		Log::error(ins.args[0]);
	};

	handlers["log"] = [](const Instruction& ins) {
		LogFile::log(ins.args[0]);
	};

	handlers["mkdir"] = [](const Instruction& ins) {
		!fs::createFolder(ins.args[0]) ? Log::error("Uneable to create folder: " + ins.args[0]) : Log::info("Created folder: " + ins.args[0]);
	};

	handlers["delete"] = [](const Instruction& ins) {
		!fs::remove(ins.args[0]) ? Log::error("Uneable to delete path: " + ins.args[0]) : Log::info("Deleted: " + ins.args[0]);
	};

	handlers["move"] = [](const Instruction& ins) {
		!fs::move(ins.args[0], ins.args[1]) ? Log::error("Uneable to move file: " + ins.args[0]) : Log::info("Moved: " + ins.args[0] + " to: " + ins.args[1]);
	};

	handlers["copy"] = [](const Instruction& ins) {
		!fs::copy(ins.args[0], ins.args[1]) ? Log::error("Uneable to copy file/path: " + ins.args[0]) : Log::info("Copied: " + ins.args[0] + " to: " + ins.args[1]);
	};

	handlers["write"] = [](const Instruction& ins) {
		!fs::write(ins.args[0], ins.args[1]) ? Log::error("Uneable to write text to: " + ins.args[0]) : Log::info("Succesfully writed text to: " + ins.args[0]);
	};

	handlers["append"] = [](const Instruction& ins) {
		!fs::append(ins.args[0], ins.args[1]) ? Log::error("Uneable to append text to: " + ins.args[0]) : Log::info("Succesfully appended text to: " + ins.args[0]);	
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

