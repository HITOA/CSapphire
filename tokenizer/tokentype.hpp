#pragma once
#include <enum.h>

namespace Tokenizer {

	BETTER_ENUM(TokenType, uint8_t, 
		TOKEN_WORD,
		TOKEN_STRING,
		TOKEN_NUMBER,
		TOKEN_COMMENT,
		TOKEN_KEYWORD,
		TOKEN_BLOCKSTART,
		TOKEN_BLOCKEND,
		TOKEN_OPERATOR,
		TOKEN_PRIMITIVE_TYPE,
		TOKEN_NEWLINE,
		TOKEN_WHITESPACE,
		TOKEN_RPAR,
		TOKEN_LPAR,
		TOKEN_COMMA,
		TOKEN_BAD)
}