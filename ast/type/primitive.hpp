#pragma once
#include "../asttype.hpp"
#include "../../sapphire/primitivetype.hpp"

namespace AST {
	class ASTPrimitive : public ASTType {
	public:
		ASTPrimitive(Sapphire::PrimitiveType type) : type{ type } {}
	public:
		void Accept(ASTVisitor* visitor) const {
			visitor->Visit_Primitive(this);
		}
	public:
		Sapphire::PrimitiveType GetType() const { return type; }
	private:
		Sapphire::PrimitiveType type;
	};
}