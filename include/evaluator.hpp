#pragma once

#include <string>
#include <vector>
#include <functional>
#include <unordered_map>

class Evaluator {
public:
    Evaluator();
    ~Evaluator();
    
    std::string eval(const std::string& expression);
    
private:
    std::unordered_map<std::string, std::function<std::string(const std::vector<std::string>&)>> functions;
    
    std::string evalFunction(const std::string& funcExpr);
    std::vector<std::string> parseArguments(const std::string& argsStr);
    size_t findOperatorOutsideQuotes(const std::string& str, char op);
    
    bool isNumber(const std::string& str);
    std::string trim(const std::string& str);
    bool isQuotedString(const std::string& str);
    std::string unescape(const std::string& str);
    
    std::string funcRead(const std::vector<std::string>& args);
    std::string funcOS(const std::vector<std::string>& args);
    std::string funcSum(const std::vector<std::string>& args);
    std::string funcMultiply(const std::vector<std::string>& args);
    std::string funcConcat(const std::vector<std::string>& args);
    std::string funcIf(const std::vector<std::string>& args);
    std::string funcLength(const std::vector<std::string>& args);
    std::string funcToUpper(const std::vector<std::string>& args);
    std::string funcToLower(const std::vector<std::string>& args);
    std::string funcTrim(const std::vector<std::string>& args);
    std::string funcSubstr(const std::vector<std::string>& args);
    std::string funcDate(const std::vector<std::string>& args);
    std::string funcTime(const std::vector<std::string>& args);
    std::string funcExists(const std::vector<std::string>& args);
};

