#pragma once
#include "../expression.hpp"
#include "../asttype.hpp"

namespace AST {
	class ASTLiteral : public ASTExpression {
	public:
		ASTLiteral(std::u8string_view value, std::shared_ptr<ASTType> type) :
			value{ value },
			type{ type } {}
	public:
		void Accept(ASTVisitor* visitor) const {
			visitor->Visit_Literal(this);
		}
	public:
		std::u8string_view GetValue() const { return value; }
		std::shared_ptr<ASTType> GetType() const { return type; }
	private:
		std::u8string_view value;
		std::shared_ptr<ASTType> type;
	};
}