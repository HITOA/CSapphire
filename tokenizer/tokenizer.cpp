#include "tokenizer.hpp"
#include "../sapphire/definition.hpp"

Tokenizer::Tokenizer::Tokenizer()
{
	std::transform(
		Sapphire::sappOperator.begin(),
		Sapphire::sappOperator.end(),
		std::back_inserter(symbolsOperator),
		[](Sapphire::Operator op) -> std::u8string_view { return op.name; });

	std::transform(
		Sapphire::sappPrimitiveType.begin(),
		Sapphire::sappPrimitiveType.end(),
		std::back_inserter(primitiveType),
		[](Sapphire::Type tp) -> std::u8string_view { return std::u8string_view{ tp.name }; });
	
	std::transform(
		Sapphire::sappKeyword.begin(),
		Sapphire::sappKeyword.end(),
		std::back_inserter(keyword),
		[](Sapphire::Keyword kw) -> std::u8string_view { return std::u8string_view(kw.name); });
}

std::u8string_view Tokenizer::Tokenizer::TokenizeWord(const std::u8string_view& str)
{
	int size = 0;
	while (std::isalnum(str[size]) || str[size] == u8'_')
		size++;
	return str.substr(0, size);
}

std::u8string_view Tokenizer::Tokenizer::TokenizeNumber(const std::u8string_view& str)
{
	int size = 0;
	while (std::isdigit(str[size]) || str[size] == u8'.')
		size++;
	if (str[0] == u8'.' || str[size] == u8'.')
		return str.substr(0, 0);

	return str.substr(0, size);
}

std::u8string_view Tokenizer::Tokenizer::TokenizeComment(const StringType& str)
{
	assert(("length must be greater than 1", str.length() > 1));
	for (int i = 1; i < str.length(); i++)
		if (str[i] == u8'\n')
			return str.substr(0, (size_t)(i));
	return str.substr(0, str.length());
}

std::pair<std::u8string_view, Tokenizer::TokenType> Tokenizer::Tokenizer::Tokenize(const std::u8string_view& str)
{

	{ //Operator
		std::u8string_view r = TryTokenizeWord(str, symbolsOperator); //Check for operator sign
		if (r.length() == 1)
			return std::make_pair(r, TokenType::TOKEN_OPERATOR);
	}

	{ //Primitive Type
		std::u8string_view r = TryTokenizeWord(str, primitiveType);
		if (r.length() > 0)
			return std::make_pair(r, TokenType::TOKEN_PRIMITIVE_TYPE);
	}

	{ //Keyword
		std::u8string_view r = TryTokenizeWord(str, keyword);
		if (r.length() > 0)
			return std::make_pair(r, TokenType::TOKEN_KEYWORD);
	}

	{ //Number
		std::u8string_view r = TokenizeNumber(str);
		if (r.length() > 0)
			return std::make_pair(r, TokenType::TOKEN_NUMBER);
	}

	{ //Word
		std::u8string_view r = TokenizeWord(str);
		if (r.length() > 0)
			return std::make_pair(r, TokenType::TOKEN_WORD);
	}

	switch (str[0]) {
	case u8'"':
	{
		std::u8string_view r = TokenizeEnclosure(str, u8'"');
		if (r.length() > 0)
			return std::make_pair(r, TokenType::TOKEN_STRING);
		break;
	}
	case u8'#':
	{
		std::u8string_view r = TokenizeComment(str);
		if (r.length() > 0)
			return std::make_pair(r, TokenType::TOKEN_COMMENT);
		break;
	}
	case u8'\n':
	{
		std::u8string_view r = TokenizeSymbol(str);
		return std::make_pair(r, TokenType::TOKEN_NEWLINE);
	}
	case u8' ':
	{
		std::u8string_view r = TokenizeSymbol(str);
		return std::make_pair(r, TokenType::TOKEN_WHITESPACE);
	}
	case u8'\t':
	{
		std::u8string_view r = TokenizeSymbol(str);
		return std::make_pair(r, TokenType::TOKEN_WHITESPACE);
	}
	case BLOCKSTART: 
	{
		std::u8string_view r = TokenizeSymbol(str);
		return std::make_pair(r, TokenType::TOKEN_BLOCKSTART);
	}
	case BLOCKEND: 
	{
		std::u8string_view r = TokenizeSymbol(str);
		return std::make_pair(r, TokenType::TOKEN_BLOCKEND);
	}
	case u8'(': 
	{
		std::u8string_view r = TokenizeSymbol(str);
		return std::make_pair(r, TokenType::TOKEN_LPAR);
	}
	case u8')':
	{
		std::u8string_view r = TokenizeSymbol(str);
		return std::make_pair(r, TokenType::TOKEN_RPAR);
	}
	case u8',':
	{
		std::u8string_view r = TokenizeSymbol(str);
		return std::make_pair(r, TokenType::TOKEN_COMMA);
	}
	}

	return std::make_pair(str.substr(0, 1), TokenType::TOKEN_BAD);
}
