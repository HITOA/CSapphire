#include "compiler.hpp"
#include "utilities/tokenfile.hpp"
#include <boost/filesystem.hpp>
#include "utilities/filesystem.hpp"

#include <iostream>

void SapphireCompiler::Compile(const Options& options, std::u8string_view src)
{
	Tokenizer::Tokenizer::Consumer consumer = tokenizer.TokenizeAndGetConsumer(src);

	if (consumer.GetSize() < 1)
		throw std::runtime_error("Input file empty");

	if (options.TokenFile()) {
		std::u8string tokenFile = TokenFile::Produce(consumer, src);
		boost::filesystem::path p{ options.GetInputFile() };
		std::string tokenPath = boost::filesystem::absolute(p).parent_path().string() + "\\tokenfile.txt";
		WriteFileU8(tokenPath, tokenFile);
		std::cout << "tokenfile written at : " << tokenPath << std::endl;
	}

}
