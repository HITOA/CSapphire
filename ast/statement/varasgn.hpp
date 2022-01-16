#pragma once
#include "../statement.hpp"
#include "../expression.hpp"
#include "../../sapphire/operator.hpp"

#include <memory>
#include <string>

namespace AST {
	class ASTVarAsgn : public ASTStatement {
	public:
		ASTVarAsgn(std::u8string_view identifier,
			std::shared_ptr<ASTExpression> value,
			Sapphire::OperatorType op) :
			identifier{ identifier },
			value{ value },
			op{ op } {};
	public:
		void Accept(ASTVisitor* visitor) const {
			visitor->Visit_VarAsgn(this);
		}
	public:
		std::u8string_view GetIdentifier() const { return identifier; }
		std::shared_ptr<ASTExpression> GetValue() const { return value; }
		Sapphire::OperatorType GetOperator() const { return op; }
	private:
		std::u8string_view identifier;
		std::shared_ptr<ASTExpression> value;
		Sapphire::OperatorType op;
	};
}