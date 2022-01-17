#pragma once
#include "../tokenizer/tokenizer.hpp"
#include <stdexcept>
#include <enum.h>

namespace Parser {
	BETTER_ENUM(ParserExceptionType, uint8_t, 
		BAD_TOKEN,
		BAD_TYPE,
		TERMINATOR_MISSING)

	class ParserException : public std::exception {
	public:
		ParserException(ParserExceptionType type, Tokenizer::Tokenizer::Consumer::TokenType token) : type{ type }, token{ token } 
		{
			fullmsg += "Error: ";
			fullmsg += type._to_string();
		}
		ParserException(ParserExceptionType type, Tokenizer::Tokenizer::Consumer::TokenType token, std::string msg) : type{ type }, token{ token }
		{
			fullmsg += "Error: ";
			fullmsg += type._to_string();
			fullmsg += ", ";
			fullmsg += msg;
		}
	public:
		const char* what() const noexcept {
			return fullmsg.c_str();
		}
	public:
		ParserExceptionType type;
		Tokenizer::Tokenizer::Consumer::TokenType token;
		std::string fullmsg;
	};
}