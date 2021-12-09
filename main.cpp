#include <iostream>
#include <fstream>
#include <sstream>
#include "lexer.hpp"
#include "utils.hpp"
#include "parser.hpp"
#include "except.hpp"

int main(int argc, char* argv) {
	std::ifstream file{ "D:/Sapphire/compiler_test.sp", std::ios::binary };

	std::stringstream buffer;
	buffer << file.rdbuf();
	std::string src = buffer.str();

	std::vector<Token> tokens = TokenizeAll(src);

	//std::cout << PrettyTokens(src, tokens) << std::endl;

	TokenConsumer consumer{ tokens, src };

	try {
		std::unique_ptr<ASTProgram> r = ParseProgram(consumer);
	}
	catch (const CSapphireException & e) {
		std::cout << e.GetFullPos() << " : " << e.GetError() << std::endl;
	}
	catch (const std::exception & e) {
		std::cout << e.what() << std::endl;
	}
	

	return EXIT_SUCCESS;
}