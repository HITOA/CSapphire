#pragma once
#include "../tokenizer/tokenizer.hpp"
#include <stdexcept>
#include <enum.h>

namespace Parser {
	BETTER_ENUM(ParserExceptionType, uint8_t, 
		BAD_TOKEN,
		BAD_TYPE)

	class ParserException : public std::exception {
	public:
		ParserException(ParserExceptionType type, Tokenizer::Tokenizer::Consumer::TokenType token) : type{ type }, token{ token } {}
	public:
		const char* what() const noexcept {
			return type._to_string();
		}
	public:
		ParserExceptionType type;
		Tokenizer::Tokenizer::Consumer::TokenType token;
	};
}