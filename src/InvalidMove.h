//
// Created by Sagiv Marzini on 06/08/2026.
//

#ifndef BIT_CHECKERS_INVALIDMOVE_H
#define BIT_CHECKERS_INVALIDMOVE_H

#include <algorithm>
#include <exception>
#include <string>

class InvalidMove : public std::exception {
public:
	explicit InvalidMove(std::string msg) : message(std::move(msg)) {
	}

	[[nodiscard]] const char *what() const noexcept override {
		return message.c_str();
	}

private:
	std::string message;
};

#endif //BIT_CHECKERS_INVALIDMOVE_H
