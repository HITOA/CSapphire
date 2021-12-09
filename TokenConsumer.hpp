#pragma once
#include <vector>
#include "lexer.hpp"

class TokenConsumer {
public:
	TokenConsumer(std::vector<Token> tokens, const std::string& src) : tokens{ tokens }, src{ src } {};
public:
	const Token& Peek() const;
	const Token& Peek(int idx) const;
	Token Consume();
	Token Consume(int idx);
	std::string GetStr(unsigned int offset, unsigned int size) const;
	int IgnoreTrmn();
	int Size();
public:
	std::vector<Token> tokens;
	const std::string& src;
};