#pragma once
#include <stdexcept>
#include <string>

enum class CSapphireExceptionType {
	TRMNMISSING,
	STMTINV,
	EXPRINV,
	SYNTAXERR,
	RPARMISSING,
	HINTEXPECTED
};

struct CSapphireException : public std::exception {
public:
	CSapphireException(CSapphireExceptionType type, unsigned int pos, const std::string& src) : type{ type }, pos{ pos }, src{ src } {};
	CSapphireException(CSapphireExceptionType type, unsigned int pos, const std::string& src, std::string info) : type{ type }, pos{ pos }, src{ src }, info{ info } {};
public:
	const char* what() const noexcept {
		return std::to_string((int)type).c_str();
	}
	std::string GetFullPos() const noexcept {
		std::pair<int, int> fullpos = GetSymbolFullPosition(pos, src);
		return "<line:" + std::to_string(fullpos.first) + ", char:" + std::to_string(fullpos.second) + ">";
	}
	std::string GetInfo() const noexcept {
		return info;
	}
	std::string GetError() const noexcept {
		switch (type) {
		case CSapphireExceptionType::TRMNMISSING:
			return "'\\n' or ';' missing.";
		case CSapphireExceptionType::STMTINV:
			return "invalide statements.";
		case CSapphireExceptionType::EXPRINV:
			return "invalide expression.";
		case CSapphireExceptionType::SYNTAXERR:
			return "syntax error.";
		case CSapphireExceptionType::RPARMISSING:
			return "')' missing.";
		case CSapphireExceptionType::HINTEXPECTED:
			return "hint expected.";
		default:
			return "unknown error.";
		}
	}
private:
	std::pair<int, int> GetSymbolFullPosition(int pos, const std::string& src) const noexcept {
		if (pos >= src.size())
			return std::make_pair(-1, pos);
		int line = 1;
		int subpos = 1;
		for (int i = 0; i <= pos; i++) {
			if (src[i] == '\n') {
				line++;
				subpos = 1;
			}
			subpos++;
		}
		return std::make_pair(line, subpos);
	}
private:
	CSapphireExceptionType type;
	unsigned int pos;
	const std::string& src;
	std::string info;
};