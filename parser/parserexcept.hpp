#pragma once
#include "../tokenizer/tokenizer.hpp"
#include "../utilities/u8stringhelper.hpp"
#include <stdexcept>
#include <enum.h>
#include <iostream>

namespace Parser {
	BETTER_ENUM(ParserExceptionType, uint8_t, 
		BAD_TOKEN,
		BAD_TYPE,
		TERMINATOR_MISSING)

	class ParserException : public std::exception {
	public:
		ParserException(ParserExceptionType type, Tokenizer::Tokenizer::Consumer::TokenType token, std::u8string_view src) : type{ type }, token{ token } 
		{
			int offset = token.second._value == Tokenizer::TokenType::TOKEN_EOF ? 1 : 0;
			auto pos = Index2Pos(src, src.size() - (token.first.size() + offset));
			fullmsg += "<" + std::to_string(pos.first);
			fullmsg += ", " + std::to_string(pos.second);
			fullmsg += "> : ";
			fullmsg += type._to_string();
		}
		ParserException(ParserExceptionType type, Tokenizer::Tokenizer::Consumer::TokenType token, std::u8string_view src, std::string msg) : type{ type }, token{ token }
		{
			int offset = token.second._value == Tokenizer::TokenType::TOKEN_EOF ? 1 : 0;
			auto pos = Index2Pos(src, src.size() - (token.first.size() + offset));
			fullmsg += "<" + std::to_string(pos.first);
			fullmsg += ", " + std::to_string(pos.second);
			fullmsg += "> : ";
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