#include "evaluator.hpp"
#include <fstream>
#include <sstream>
#include <cctype>
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <cmath>
#include <iostream>
#include <regex>

// Constructor
Evaluator::Evaluator() {
    functions = {
        {"READ", [this](const std::vector<std::string>& args) { return funcRead(args); }},
        {"OS", [this](const std::vector<std::string>& args) { return funcOS(args); }},
        {"SUM", [this](const std::vector<std::string>& args) { return funcSum(args); }},
        {"MULTIPLY", [this](const std::vector<std::string>& args) { return funcMultiply(args); }},
        {"CONCAT", [this](const std::vector<std::string>& args) { return funcConcat(args); }},
        {"IF", [this](const std::vector<std::string>& args) { return funcIf(args); }},
        {"LENGTH", [this](const std::vector<std::string>& args) { return funcLength(args); }},
        {"TOUPPER", [this](const std::vector<std::string>& args) { return funcToUpper(args); }},
        {"TOLOWER", [this](const std::vector<std::string>& args) { return funcToLower(args); }},
        {"TRIM", [this](const std::vector<std::string>& args) { return funcTrim(args); }},
        {"SUBSTR", [this](const std::vector<std::string>& args) { return funcSubstr(args); }},
        {"DATE", [this](const std::vector<std::string>& args) { return funcDate(args); }},
        {"TIME", [this](const std::vector<std::string>& args) { return funcTime(args); }},
        {"EXISTS", [this](const std::vector<std::string>& args) { return funcExists(args); }}
    };
}

Evaluator::~Evaluator() {}

std::string Evaluator::eval(const std::string& expression) {
    std::string expr = trim(expression);
    
    if (expr.empty()) return "";
    
    size_t plusPos = findOperatorOutsideQuotes(expr, '+');
    if (plusPos != std::string::npos) {
        std::string left = expr.substr(0, plusPos);
        std::string right = expr.substr(plusPos + 1);
        
        std::string leftVal = eval(left);
        std::string rightVal = eval(right);
        
        return leftVal + rightVal;
    }
    
    if (isQuotedString(expr)) {
        return unescape(expr.substr(1, expr.length() - 2));
    }
    
    if (expr.front() == '=') {
        return evalFunction(expr);
    }
    
    return expr;
}

std::string Evaluator::evalFunction(const std::string& funcExpr) {
    size_t parenOpen = funcExpr.find('(');
    if (parenOpen == std::string::npos) {
        return funcExpr;
    }
    
    std::string funcName = funcExpr.substr(1, parenOpen - 1);
    
    std::string argsStr = funcExpr.substr(parenOpen + 1);
    if (!argsStr.empty() && argsStr.back() == ')') {
        argsStr.pop_back();
    }
    
    std::vector<std::string> args = parseArguments(argsStr);
    
    for (auto& arg : args) {
        arg = eval(arg);
    }
    
    auto it = functions.find(funcName);
    if (it != functions.end()) {
        return it->second(args);
    }
    
    return funcExpr;
}

std::vector<std::string> Evaluator::parseArguments(const std::string& argsStr) {
    std::vector<std::string> args;
    std::string current;
    bool inQuotes = false;
    int parenDepth = 0;
    
    for (size_t i = 0; i < argsStr.length(); i++) {
        char c = argsStr[i];
        
        if (c == '"' && (i == 0 || argsStr[i-1] != '\\')) {
            inQuotes = !inQuotes;
            current += c;
        }
        else if (c == '(' && !inQuotes) {
            parenDepth++;
            current += c;
        }
        else if (c == ')' && !inQuotes) {
            parenDepth--;
            current += c;
        }
        else if (c == ',' && !inQuotes && parenDepth == 0) {
            args.push_back(trim(current));
            current.clear();
        }
        else {
            current += c;
        }
    }
    
    if (!current.empty()) {
        args.push_back(trim(current));
    }
    
    return args;
}

size_t Evaluator::findOperatorOutsideQuotes(const std::string& str, char op) {
    bool inQuotes = false;
    int parenDepth = 0;
    
    for (size_t i = 0; i < str.length(); i++) {
        char c = str[i];
        
        if (c == '"' && (i == 0 || str[i-1] != '\\')) {
            inQuotes = !inQuotes;
        }
        else if (c == '(' && !inQuotes) {
            parenDepth++;
        }
        else if (c == ')' && !inQuotes) {
            parenDepth--;
        }
        else if (c == op && !inQuotes && parenDepth == 0) {
            return i;
        }
    }
    
    return std::string::npos;
}

bool Evaluator::isNumber(const std::string& str) {
    if (str.empty()) return false;
    char* end;
    std::strtod(str.c_str(), &end);
    return end != str.c_str() && *end == '\0';
}

std::string Evaluator::trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}

bool Evaluator::isQuotedString(const std::string& str) {
    return str.length() >= 2 && str.front() == '"' && str.back() == '"';
}

std::string Evaluator::unescape(const std::string& str) {
    std::string result;
    for (size_t i = 0; i < str.length(); i++) {
        if (str[i] == '\\' && i + 1 < str.length()) {
            switch (str[i + 1]) {
                case 'n': result += '\n'; break;
                case 't': result += '\t'; break;
                case '"': result += '"'; break;
                case '\\': result += '\\'; break;
                default: result += str[i + 1]; break;
            }
            i++;
        } else {
            result += str[i];
        }
    }
    return result;
}

std::string Evaluator::funcRead(const std::vector<std::string>& args) {
    if (args.empty()) return "";
    std::string filename = args[0];
    std::ifstream file(filename);
    if (!file.is_open()) return "";
    std::stringstream buffer;
    buffer << file.rdbuf();
    return buffer.str();
}

std::string Evaluator::funcOS(const std::vector<std::string>& args) {
    #ifdef _WIN32
        return "Windows";
    #elif __linux__
        return "Linux";
    #elif __APPLE__
        return "macOS";
    #else
        return "Unknown";
    #endif
}

std::string Evaluator::funcSum(const std::vector<std::string>& args) {
    double total = 0;
    for (const auto& arg : args) {
        try {
            total += std::stod(arg);
        } catch (...) {}
    }
    if (total == std::floor(total)) {
        return std::to_string(static_cast<long long>(total));
    }
    return std::to_string(total);
}

std::string Evaluator::funcMultiply(const std::vector<std::string>& args) {
    if (args.empty()) return "0";
    double result = 1;
    for (const auto& arg : args) {
        try {
            result *= std::stod(arg);
        } catch (...) {}
    }
    if (result == std::floor(result)) {
        return std::to_string(static_cast<long long>(result));
    }
    return std::to_string(result);
}

std::string Evaluator::funcConcat(const std::vector<std::string>& args) {
    std::string result;
    for (const auto& arg : args) {
        result += arg;
    }
    return result;
}

std::string Evaluator::funcIf(const std::vector<std::string>& args) {
    if (args.size() < 3) return "";
    std::string condition = args[0];
    bool isTrue = !condition.empty() && condition != "false" && condition != "0";
    return isTrue ? args[1] : args[2];
}

std::string Evaluator::funcLength(const std::vector<std::string>& args) {
    if (args.empty()) return "0";
    return std::to_string(args[0].length());
}

std::string Evaluator::funcToUpper(const std::vector<std::string>& args) {
    if (args.empty()) return "";
    std::string result = args[0];
    std::transform(result.begin(), result.end(), result.begin(), ::toupper);
    return result;
}

std::string Evaluator::funcToLower(const std::vector<std::string>& args) {
    if (args.empty()) return "";
    std::string result = args[0];
    std::transform(result.begin(), result.end(), result.begin(), ::tolower);
    return result;
}

std::string Evaluator::funcTrim(const std::vector<std::string>& args) {
    if (args.empty()) return "";
    return trim(args[0]);
}

std::string Evaluator::funcSubstr(const std::vector<std::string>& args) {
    if (args.size() < 2) return "";
    std::string str = args[0];
    int start = 0;
    int length = str.length();
    
    try {
        start = std::stoi(args[1]);
        if (args.size() >= 3) {
            length = std::stoi(args[2]);
        }
    } catch (...) {
        return "";
    }
    
    if (start < 0) start = 0;
    if (start >= static_cast<int>(str.length())) return "";
    if (start + length > static_cast<int>(str.length())) {
        length = str.length() - start;
    }
    
    return str.substr(start, length);
}

std::string Evaluator::funcDate(const std::vector<std::string>& args) {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d");
    return ss.str();
}

std::string Evaluator::funcTime(const std::vector<std::string>& args) {
    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);
    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%H:%M:%S");
    return ss.str();
}

std::string Evaluator::funcExists(const std::vector<std::string>& args) {
    if (args.empty()) return "false";
    std::ifstream file(args[0]);
    return file.good() ? "true" : "false";
}

