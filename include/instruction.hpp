#pragma once

#include <string>
#include <vector>

enum class InstructionType {
    NORMAL,
    IF,
    ELSE,
    ENDIF,
    WHILE,
    ENDWHILE,
    BREAK,
    CONTINUE
};

struct Instruction {
    std::string name;
    std::vector<std::string> args;
    std::vector<std::string> evaluatedArgs;
    InstructionType type = InstructionType::NORMAL;
    
    int jumpTarget = -1;
    
    Instruction() = default;
    Instruction(const std::string& n) : name(n) {}
};

