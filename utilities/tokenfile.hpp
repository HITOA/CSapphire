#pragma once
#include <string>
#include "../tokenizer/tokenizer.hpp"

namespace TokenFile {
	std::u8string Produce(const Tokenizer::Tokenizer::Consumer& consumer, std::u8string_view src);
}