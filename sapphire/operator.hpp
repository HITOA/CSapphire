#pragma once
#include <array>
#include <enum.h>


namespace Sapphire {

	struct Operator {
	public:
		Operator(const char8_t* name, const int precedence) : name{ name }, precedence{ precedence } {}
	public:
		const char8_t* name;
		const int precedence;
	};

	static std::array<Operator, 23> sappOperator{
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

	BETTER_ENUM(OperatorType, uint8_t,
		NOT,
		COMPLEMENT,
		UNARY_PLUS,
		UNARY_MINUS,
		MULTIPLY,
		DIVIDE,
		MODULO,
		BINARY_PLUS,
		BINARY_MINUS,
		BTW_RIGHT_SHIFT,
		BTW_LEFT_SHIFT,
		BTW_AND,
		BTW_XOR,
		BTW_OR,
		LESS_EQUAL,
		LESS,
		GREATER,
		GREATER_EQUAL,
		EQUAL,
		NOT_EQUAL,
		AND,
		OR,
		ASSIGNEMENT
	)
}