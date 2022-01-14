#pragma once
#include "tokenizer/tokenizer.hpp"
#include "options.hpp"

class SapphireCompiler {
public:
	SapphireCompiler() = default;
	~SapphireCompiler() = default;
public:
	void Compile(const Options& options, std::u8string_view src);
private:
	Tokenizer::Tokenizer tokenizer;
};