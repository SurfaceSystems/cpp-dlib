#pragma once

#include <exception>
#include <string>

class InvalidCommand : public std::exception {
private:
	int line;
	std::string message;
	mutable std::string fullMessage;

public:
	explicit InvalidCommand(int li, const std::string& msg);
	~InvalidCommand() noexcept = default;

	int getLine() const noexcept;
	const char* what() const noexcept override;
};

class NotMinimunArgs: public std::exception {
private:
	int line;
	std::string message;
	mutable std::string fullMessage;

public:
	explicit NotMinimunArgs(int li, const std::string& msg);
	~NotMinimunArgs() noexcept = default;

	int getLine() const noexcept;
	const char* what() const noexcept override;
};

