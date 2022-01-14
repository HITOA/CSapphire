#pragma once
#include <array>

namespace Sapphire {

	struct Operator {
	public:
		Operator(const char8_t* name, const int precedence) : name{ name }, precedence{ precedence } {}
	public:
		const char8_t* name;
		const int precedence;
	};

	std::array<Operator, 23> sappOperator{
	Operator{ u8"!", 0 },
	Operator{ u8"~", 0 },
	Operator{ u8"+", 0 }, //Unary
	Operator{ u8"-", 0 }, //Unary

	Operator{ u8"*", 1 },
	Operator{ u8"/", 1 },
	Operator{ u8"%", 1 },

	Operator{ u8"+", 2 }, //Binary
	Operator{ u8"-", 2 }, //Binary

	Operator{ u8">>", 3 },
	Operator{ u8"<<", 3 },

	Operator{ u8"&", 4 },

	Operator{ u8"^", 5 },
	Operator{ u8"|", 5 },

	Operator{ u8"<=", 6 },
	Operator{ u8"<", 6 },
	Operator{ u8">", 6 },
	Operator{ u8">=", 6 },
	Operator{ u8"==", 6 },
	Operator{ u8"!=", 6 },

	Operator{ u8"&&", 7 },
	Operator{ u8"||", 7 },

	Operator{ u8"=", 8 }
	};
}