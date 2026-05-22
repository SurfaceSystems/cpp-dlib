#include "executor.hpp"

#include <iostream>
#include "instruction.hpp"
#include <unordered_map>
#include <functional>
#include <cstdlib>
#include "progress.hpp"
#include "log.hpp"
#include "filesystem.hpp"
#include "evaluator.hpp"
#include <algorithm>

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

	handlers["success"] = [](const Instruction& ins) {
		Log::success(ins.args[0]);
	};

	handlers["set"] = [this](const Instruction& ins) {
		Variable* existingVar = findVariable(ins.args[0]);
		if (existingVar) {
			existingVar->set(ins.args[1]);
		} else {
			variables.emplace_back(ins.args[0], ins.args[1]);
		}
	};	
}

void Executor::execute(std::vector<Instruction> program) {
	size_t ip = 0;
	std::vector<size_t> whileStack;
	
	Evaluator evaluator(variables);
	
	while (ip < program.size()) {
		const Instruction& ins = program[ip];
		Instruction finalInstruction;
		finalInstruction.name = ins.name;
		finalInstruction.type = ins.type;
		
		for (const auto& arg : ins.args) {
			std::string evaluated = evaluator.eval(arg);
			finalInstruction.args.push_back(evaluated);
		}
		
		switch (ins.type) {
			case InstructionType::NORMAL: {
				auto it = handlers.find(ins.name);
				if (it != handlers.end()) {
					it->second(finalInstruction);
				} else {
					std::cerr << "Unknown command: " << ins.name << std::endl;
				}
				ip++;
				break;
			}
			
			case InstructionType::IF: {
				bool condition = evaluateCondition(finalInstruction.args[0]);
				
				if (!condition) {
					ip = ins.jumpTarget;
				} else {
					ip++;
				}
				break;
			}
			
			case InstructionType::ELSE: {
				ip = ins.jumpTarget;
				break;
			}
			
			case InstructionType::ENDIF: {
				ip++;
				break;
			}
			
			case InstructionType::WHILE: {
				bool condition = evaluateCondition(finalInstruction.args[0]);
				
				if (!condition) {
					ip = ins.jumpTarget;
				} else {
					whileStack.push_back(ip);
					ip++;
				}
				break;
			}
			
			case InstructionType::ENDWHILE: {
				ip = ins.jumpTarget;
				break;
			}
			
			case InstructionType::BREAK: {
				if (whileStack.empty()) {
					throw std::runtime_error("BREAK outside loop");
				}
				size_t whileIp = whileStack.back();
				ip = program[whileIp].jumpTarget;
				whileStack.pop_back();
				break;
			}
			
			case InstructionType::CONTINUE: {
				if (whileStack.empty()) {
					throw std::runtime_error("CONTINUE outside loop");
				}
				ip = whileStack.back();
				break;
			}
		}
	}
}

Variable* Executor::findVariable(const std::string& name) {
	auto it = std::find_if(variables.begin(), variables.end(),
		[&name](const Variable& var) {
			return var.getName() == name;
		});

	if (it != variables.end()) {
		return &(*it);
	}
	return nullptr;
}

bool Executor::evaluateCondition(const std::string& condition) {
	Evaluator evaluator(variables);
	std::string result = evaluator.eval(condition);

	if (result.empty() || result == "false" || result == "0" || result == "no") {
		return false;
	}
	return true;
}
