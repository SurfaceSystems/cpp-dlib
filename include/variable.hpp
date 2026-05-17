#pragma once

#include <string>

class Variable {
public:
	Variable(const std::string& n, const std::string& c = "") 
		: name(n), value(c) {}

	std::string getName() const { return name; }
	std::string get() const { return value; }
	
	void set(const std::string& content) { value = content; }
	void setName(const std::string& n) { name = n; }

private:
	std::string name;
	std::string value;
};

