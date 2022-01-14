#include "compiler.hpp"
#include "utilities/tokenfile.hpp"
#include <boost/filesystem.hpp>
#include "utilities/filesystem.hpp"

#include <iostream>

void SapphireCompiler::Compile(const Options& options, std::u8string_view src)
{
	auto tokens = tokenizer.TokenizeAll(src);

	if (tokens.size() < 1)
		throw std::runtime_error("Input file empty");

	for (int i = 0; i < tokens.size(); i++) {
		switch (tokens[i].second) {
		case Tokenizer::TokenType::TOKEN_WHITESPACE: {
			tokens.erase(tokens.begin() + i);
			i--;
			break;
		}
		case Tokenizer::TokenType::TOKEN_NEWLINE: {
			tokens.erase(tokens.begin() + i);
			i--;
			break;
		}
		}
	}

	Tokenizer::Tokenizer::Consumer consumer{ tokens };

	if (options.TokenFile()) {
		std::u8string tokenFile = TokenFile::Produce(consumer, src);
		boost::filesystem::path p{ options.GetInputFile() };
		std::string tokenPath = boost::filesystem::absolute(p).parent_path().string() + "\\tokenfile.txt";
		WriteFileU8(tokenPath, tokenFile);
		std::cout << "tokenfile written at : " << tokenPath << std::endl;
	}

}
