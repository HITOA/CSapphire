#include "lexer.hpp"
#include <functional>
#include <iostream>

unsigned int SkipWhitespace(const std::string& src, unsigned int pos)
{
	std::string ignoredChar{ " \t\r" };
	unsigned int length = 0;

	while (ignoredChar.find(src[pos + length]) != std::string::npos)
		length++;

	return length;
}

TokenData TokenizeKeyword(const std::string& src, unsigned int pos)
{
	std::vector<std::string> keywords
	{
		"class",
		"func",
		"return",
		"if",
		"elif",
		"else",
		"for",
		"while"
	};

	if (!std::isalpha(src[pos]))
		return TokenData::BadToken();

	unsigned int length = 0;

	while (std::isalpha(src[pos + length]))
		length++;

	if (length == 0)
		return TokenData::BadToken();

	std::string keyword = src.substr(pos, length);
	
	if (std::find(keywords.begin(), keywords.end(), keyword) == keywords.end()) {
		return TokenData::BadToken();
	}

	return TokenData{ Token{ TokenType::KEYWORD, pos, length }, length, true };
}

TokenData TokenizeName(const std::string& src, unsigned int pos)
{
	if (!std::isalpha(src[pos]) && src[pos] != '_')
		return TokenData::BadToken();

	unsigned int length = 0;

	while (std::isalnum(src[pos + length]) || src[pos + length] == '_')
		length++;

	if (length == 0)
		return TokenData::BadToken();

	if (src.substr(pos, length) == "do" || src.substr(pos, length) == "end")
		return TokenData::BadToken();

	return TokenData{ Token{ TokenType::NAME, pos, length }, length, true };
}

TokenData TokenizeNumber(const std::string& src, unsigned int pos)
{
	if (!std::isdigit(src[pos]))
		return TokenData::BadToken();

	unsigned int length = 0;

	while (std::isdigit(src[pos + length]) || src[pos + length] == '.')
		length++;

	if (src[pos + length - 1] == '.')
		length--;

	if (length == 0)
		return TokenData::BadToken();

	return TokenData{ Token{ TokenType::NUMBER, pos, length }, length, true };
}

TokenData TokenizeString(const std::string& src, unsigned int pos)
{
	if (src[pos] != '\"' && src[pos] != '\'')
		return TokenData::BadToken();

	unsigned int length = 1;

	while (src[pos + length] != src[pos]) {
		length++;

		if ((pos + length) >= src.size())
			return TokenData::BadToken();
	}

	return TokenData{ Token{ TokenType::STRING, pos + 1, length - 1 }, length + 1, true };
}

TokenData TokenizeOpArth(const std::string& src, unsigned int pos)
{
	std::vector<std::string> opArth{
		"+",
		"-",
		"*",
		"/",
		"%"
	};

	for (auto op : opArth) {
		if (src.size() - pos >= op.size())
			if (src.substr(pos, op.size()) == op)
				return TokenData{ Token{ TokenType::OPARTH, pos, (unsigned int)op.size()}, (unsigned int)op.size(), true };
	}

	return TokenData::BadToken();
}

TokenData TokenizeOpAsgn(const std::string& src, unsigned int pos)
{
	std::vector<std::string> opAsgn{
		"=",
		"+=",
		"-=",
		"*=",
		"/=",
		"%="
	};

	for (auto op : opAsgn) {
		if (src.size() - pos >= op.size())
			if (src.substr(pos, op.size()) == op)
				return TokenData{ Token{ TokenType::OPASGN, pos, (unsigned int)op.size()}, (unsigned int)op.size(), true };
	}

	return TokenData::BadToken();
}

TokenData TokenizeOpRela(const std::string& src, unsigned int pos)
{
	std::vector<std::string> opRela{
		"==",
		"!=",
		">",
		"<",
		">=",
		"<="
	};

	for (auto op : opRela) {
		if (src.size() - pos >= op.size())
			if (src.substr(pos, op.size()) == op)
				return TokenData{ Token{ TokenType::OPRELA, pos, (unsigned int)op.size()}, (unsigned int)op.size(), true };
	}

	return TokenData::BadToken();
}

TokenData TokenizeOpLogi(const std::string& src, unsigned int pos)
{
	std::vector<std::string> opLogi{
		"&&",
		"||",
		"!"
	};

	for (auto op : opLogi) {
		if (src.size() - pos >= op.size())
			if (src.substr(pos, op.size()) == op)
				return TokenData{ Token{ TokenType::OPLOGI, pos, (unsigned int)op.size()}, (unsigned int)op.size(), true };
	}

	return TokenData::BadToken();
}

TokenData TokenizeOpBitw(const std::string& src, unsigned int pos)
{
	std::vector<std::string> opBitw{
		"&",
		"|",
		"^",
		"~",
		"<<",
		">>"
	};

	for (auto op : opBitw) {
		if (src.size() - pos >= op.size())
			if (src.substr(pos, op.size()) == op)
				return TokenData{ Token{ TokenType::OPBITW, pos, (unsigned int)op.size()}, (unsigned int)op.size(), true };
	}

	return TokenData::BadToken();
}

TokenData TokenizeSpecial(const std::string& src, unsigned int pos)
{
	std::vector<std::pair<std::string, TokenType>> specials
	{
		{"{", TokenType::BLOCKSTART},
		{"do", TokenType::BLOCKSTART},
		{"}", TokenType::BLOCKEND},
		{"end", TokenType::BLOCKEND},
		{"(", TokenType::LPAR},
		{")", TokenType::RPAR},
		{"[", TokenType::LBRCK},
		{"]", TokenType::RBRCK},
		{"\n", TokenType::TRMN},
		{";", TokenType::TRMN},
		{".", TokenType::DOT},
		{",", TokenType::COMMA},
		{"++", TokenType::OPINC},
		{"--", TokenType::OPDEC},
		{":", TokenType::COLON}
	};

	for (auto special : specials) {
		if (src.size() - pos >= special.first.size())
			if (src.substr(pos, special.first.size()) == special.first)
				return TokenData{ Token{ special.second, pos, (unsigned int)special.first.size()}, (unsigned int)special.first.size(), true };
	}

	return TokenData::BadToken();
}

TokenData TokenizeComment(const std::string& src, unsigned int pos)
{
	if (src[pos] != '#')
		return TokenData::BadToken();

	unsigned int length = 1;

	while (src[pos + length] != '\n') {
		length++;

		if ((pos + length) >= src.size())
			return TokenData{ Token{ TokenType::COMMENT, pos + 1, length }, length, true };
	}

	return TokenData{ Token{ TokenType::COMMENT, pos + 1, length - 1 }, length + 1, true };
}

TokenData TokenizeUnknown(const std::string& src, unsigned int pos)
{
	return TokenData::UnknownToken(pos);
}

std::vector<Token> TokenizeAll(const std::string& src)
{
	std::vector<std::function<TokenData(const std::string&, unsigned int)>> tokenizers
	{
		TokenizeKeyword,
		TokenizeName,
		TokenizeNumber,
		TokenizeString,
		TokenizeSpecial,
		TokenizeOpRela,
		TokenizeOpAsgn,
		TokenizeOpBitw,
		TokenizeOpLogi,
		TokenizeOpArth,
		TokenizeComment,
		TokenizeUnknown
	};

	std::vector<Token> tokens{};
	unsigned int pos = 0;

	while (pos < src.size()) {
		pos += SkipWhitespace(src, pos);

		for (auto tokenizer : tokenizers) {
			TokenData token = tokenizer(src, pos);

			if (token.isValid) {
				tokens.push_back(token.token);
				pos += token.length;
				break;
			}
		}
	}

	return tokens;
}

TokenData TokenData::BadToken()
{
	return TokenData{ Token{ TokenType::UNKNOWN, 0, 0 }, 0, false };
}

TokenData TokenData::UnknownToken(unsigned int pos)
{
	return TokenData{ Token{ TokenType::UNKNOWN, pos, 1 }, 1, true };
}
