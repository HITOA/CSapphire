#pragma once
#include "basetokenizer.hpp"
#include "tokentype.hpp"
#include <algorithm>
#include <iterator>
#include <cctype>

namespace Tokenizer 
{
	class Tokenizer : public U8BaseTokenizer<TokenType> {
	public:
		Tokenizer();
	public:
		std::u8string_view TokenizeWord(const std::u8string_view& str);

		std::u8string_view TokenizeNumber(const std::u8string_view& str);

		std::u8string_view TokenizeComment(const StringType& str);

		std::pair<std::u8string_view, TokenType> Tokenize(const std::u8string_view& str);
	private:
		std::vector<std::u8string_view> symbolsOperator;
		std::vector<std::u8string_view> primitiveType;
		std::vector<std::u8string_view> keyword;
	};
}