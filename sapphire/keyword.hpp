#pragma once
#include <array>

namespace Sapphire {

	struct Keyword {
	public:
		Keyword(const char8_t* name) : name{ name } {}
	public:
		const char8_t* name;
	};

	std::array<Keyword, 10> sappKeyword{
		Keyword{u8"true"},
		Keyword{u8"false"},

		Keyword{u8"if"},
		Keyword{u8"else"},
		Keyword{u8"elif"},
		Keyword{u8"for"},
		Keyword{u8"while"},

		Keyword{u8"func"},
		Keyword{u8"class"},
		Keyword{u8"struct"}
	};
}