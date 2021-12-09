#include "TokenConsumer.hpp"

const Token& TokenConsumer::Peek() const
{
	if (tokens.size() < 1)
		return Token{ TokenType::TRMN, 0, 0 };
	return tokens[0];
}

const Token& TokenConsumer::Peek(int idx) const
{
	if (tokens.size() <= idx)
		return Token{ TokenType::TRMN, 0, 0 };
	return tokens[idx];
}

Token TokenConsumer::Consume()
{
	if (tokens.size() < 1)
		return Token{ TokenType::TRMN, 0, 0 };
	Token token{ tokens[0] };
	tokens.erase(tokens.begin());
	return token;
}

Token TokenConsumer::Consume(int idx)
{
	if (tokens.size() <= idx)
		return Token{ TokenType::TRMN, 0, 0 };
	Token token{ tokens[idx] };
	tokens.erase(tokens.begin() + idx);
	return token;
}

std::string TokenConsumer::GetStr(unsigned int offset, unsigned int size) const
{
	return std::move(src.substr(offset, size));
}

int TokenConsumer::IgnoreTrmn()
{
	int length = 0;
	while (tokens.size() > 0) {
		Token token = Peek();
		if (GetStr(token.offset, token.size) != "\n")
			break;
		length++;
		tokens.erase(tokens.begin());
	}
	return length;
}

int TokenConsumer::Size()
{
	return tokens.size();
}
