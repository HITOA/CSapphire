#include "compiler.hpp"
#include "utilities/tokenfile.hpp"
#include <boost/filesystem.hpp>
#include "utilities/filesystem.hpp"
#include "utilities/astfile.hpp"
#include "ast/program.hpp"

#include <iostream>

void SapphireCompiler::Compile(const Options& options, std::u8string_view src)
{
	Tokenizer::Tokenizer::Consumer consumer = tokenizer.TokenizeAndGetConsumer(src);

	if (consumer.GetSize() < 1)
		throw std::runtime_error("Input file empty");

	for (int i = 0; i < consumer.GetSize(); i++) {
		switch (consumer.Peek(i).second) {
		case Tokenizer::TokenType::TOKEN_WHITESPACE: {
			consumer.Consume(i);
			i--;
			break;
		}
		case Tokenizer::TokenType::TOKEN_COMMENT: {
			consumer.Consume(i);
			i--;
			break;
		}
		}
	}

	if (options.TokenFile()) {
		std::u8string tokenFile = TokenFile::Produce(consumer, src);
		boost::filesystem::path p{ options.GetInputFile() };
		std::string tokenPath = boost::filesystem::absolute(p).parent_path().string() + "\\tokenfile.txt";
		WriteFileU8(tokenPath, tokenFile);
		std::cout << "tokenfile written at : " << tokenPath << std::endl;
	}

	parser.SetConsumer(consumer);
	
	std::shared_ptr<AST::ASTProgram> program = parser.ParseProgram();

	if (options.AstFile()) {
		std::u8string astFile = AstFile::Produce(program);
		boost::filesystem::path p{ options.GetInputFile() };
		std::string astPath = boost::filesystem::absolute(p).parent_path().string() + "\\astfile.txt";
		WriteFileU8(astPath, astFile);
		std::cout << "astfile written at : " << astPath << std::endl;
	}
}
