#include <iostream>
#include <fstream>
#include "options.hpp"
#include "source/utf8/cpp17.h"
#include "compiler.hpp"
#include "tokenizer/tokenizer.hpp"

#define _SILENCE_ALL_CXX20_DEPRECATION_WARNINGS

int HandleError(const std::exception& err) {
	std::cerr << err.what() << std::endl;
	return EXIT_FAILURE;
}

int main(int argc, const char* argv[]) {
	try {
		Options options{};

		if (!options.Parse(argc, argv))
			return EXIT_SUCCESS;

		std::u8string inputbuffer;
		std::ifstream ifs{ options.GetInputFile() };
		std::istreambuf_iterator<char> it{ ifs.rdbuf() };
		std::istreambuf_iterator<char> eos{};

		switch (options.GetEncoding()) {
		case Options::Encoding::utf8:
			inputbuffer = std::u8string{ it, eos };
			break;
		case Options::Encoding::utf16:
			utf8::utf16to8(it, eos, std::back_inserter(inputbuffer));
			break;
		case Options::Encoding::utf32:
			utf8::utf32to8(it, eos, std::back_inserter(inputbuffer));
			break;
		default:
			return EXIT_FAILURE;
		}

		SapphireCompiler compiler{};

		compiler.Compile(options, inputbuffer);
		
		return EXIT_SUCCESS;
	}
	catch (const std::exception& err) {
		return HandleError(err);
	}
	return EXIT_FAILURE;
}