#pragma once
#include <string>
#include <vector>

enum class TokenType {
	UNKNOWN,
	NAME, //
	NUMBER, //
	STRING, //
	COMMENT,
	LPAR,
	RPAR, //
	LBRCK,
	RBRCK, //
	BLOCKSTART,
	BLOCKEND, //
	TRMN,
	DOT,
	COMMA,
	COLON,
	OPARTH,
	OPASGN,
	OPRELA,
	OPLOGI,
	OPBITW,
	OPINC, //
	OPDEC, //
	KEYWORD
};

struct Token {
public:
	explicit Token(TokenType type, unsigned int offset, unsigned int size) : type{ type }, offset{ offset }, size{ size } {};

public:
	TokenType type;
	unsigned int offset;
	unsigned int size;
};

struct TokenData {
public:
	TokenData(Token token, unsigned int length, bool isValid) : token{ token }, length{ length }, isValid{ isValid } {};

public:
	static TokenData BadToken();
	static TokenData UnknownToken(unsigned int pos);

public:
	Token token;
	unsigned int length;
	bool isValid;
};

unsigned int SkipWhitespace(const std::string& src, unsigned int pos);

TokenData TokenizeKeyword(const std::string& src, unsigned int pos);
TokenData TokenizeName(const std::string& src, unsigned int pos);
TokenData TokenizeNumber(const std::string& src, unsigned int pos);
TokenData TokenizeString(const std::string& src, unsigned int pos);
TokenData TokenizeOpArth(const std::string& src, unsigned int pos);
TokenData TokenizeOpAsgn(const std::string& src, unsigned int pos);
TokenData TokenizeOpRela(const std::string& src, unsigned int pos);
TokenData TokenizeOpLogi(const std::string& src, unsigned int pos);
TokenData TokenizeOpBitw(const std::string& src, unsigned int pos);
TokenData TokenizeSpecial(const std::string& src, unsigned int pos);
TokenData TokenizeComment(const std::string& src, unsigned int pos);
TokenData TokenizeUnknown(const std::string& src, unsigned int pos);

std::vector<Token> TokenizeAll(const std::string& src);
