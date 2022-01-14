#pragma once
#include <array>

namespace Sapphire {
	struct Type {
	public:
		Type(const char8_t* name, const int sbyte) : name{ name }, sbyte{ sbyte }, sbit{ sbyte * 8 } {}
	public:
		const char8_t* name;
		const int sbyte;
		const int sbit;
	};

	std::array<Type, 11> sappPrimitiveType{
		Type{ u8"i8", 1 },
		Type{ u8"i16", 2 },
		Type{ u8"i32", 4 },
		Type{ u8"i64", 8 },

		Type{ u8"byte", 1 },
		Type{ u8"u16", 2 },
		Type{ u8"u32", 4 },
		Type{ u8"u64", 8 },

		Type{ u8"f32", 4 },
		Type{ u8"f64", 8 },

		Type{ u8"bool", 1 }
	};
}