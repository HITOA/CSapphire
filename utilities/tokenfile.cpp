#include "tokenfile.hpp"

std::pair<int, int> Index2Pos(std::u8string_view src, int index) {
	if (index == 0)
		return std::make_pair(0, 0);

	assert(("Index2Pos : index out of bound", index < src.length()));

	int line = 0;
	int column = 0;

	for (int c = 0; c != index; c++) {
		if (src[c] == u8'\n')
		{
			line++;
			column = 0;
		}
		else {
			column++;
		}
	}

	return std::make_pair(line, column);
}

std::u8string IntToU8String(int v) {
	std::u8string tb = u8"0123456789";
	std::u8string result;

	do {
		int r = v % 10;
		v /= 10;
		result = tb[r] + result;
	} while (v > 0);
	return result;
}

void AddWhiteSpaceUntil(std::u8string& src, int c) {
	c -= src.length();

	for (int i = 0; i < c; i++)
		src += u8" ";
}

std::u8string TokenFile::Produce(const Tokenizer::Tokenizer::Consumer& consumer, std::u8string_view src)
{
	std::u8string output{};

	for (int i = 0; i < consumer.GetSize() - 1; i++) 
	{
		std::u8string line{};
		std::u8string_view tokenContent = consumer.Peek(i).first;
		const char8_t* typeName = (const char8_t*)consumer.Peek(i).second._to_string();
		std::pair<int, int> pos = Index2Pos(src, consumer.Peek(i).first.data() - src.data());
		
		line += std::u8string{ u8"<" } + 
			IntToU8String(pos.first) + 
			u8", " + 
			IntToU8String(pos.second) + 
			u8">";

		AddWhiteSpaceUntil(line, 15);
		line += typeName;
		AddWhiteSpaceUntil(line, 40);
		line += tokenContent;

		output += line + u8"\n";
	}

	return output;
}
