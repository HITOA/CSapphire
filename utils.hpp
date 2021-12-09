#pragma once
#include <iostream>
#include "lexer.hpp"
#include "ast.hpp"

std::string PrettyTokens(const std::string& src, std::vector<Token> tokens) {
	std::vector<std::string> enumName{
		"UNKNOWN",
		"NAME",
		"NUMBER",
		"STRING",
		"COMMENT",
		"LPAR",
		"RPAR",
		"LBRCK",
		"RBRCK",
		"BLOCKSTART",
		"BLOCKEND",
		"TRMN",
		"DOT",
		"COMMA",
		"COLON",
		"OPARTH",
		"OPASGN",
		"OPRELA",
		"OPLOGI",
		"OPBITW",
		"OPINC",
		"OPDEC",
		"KEYWORD"
	};
	std::string r{};

	for (auto token : tokens) {
		r += "{\n";
		r += "	type : " + enumName[(int)token.type] + "\n";
		r += "	value : \"" + src.substr(token.offset, token.size) + "\"\n";
		r += "}\n";
	}

	return r;
}
