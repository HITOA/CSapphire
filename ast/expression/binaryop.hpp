#pragma once
#include "../expression.hpp"
#include "../../sapphire/operator.hpp"
#include <memory>

namespace AST {
	class ASTBinaryOp : public ASTExpression {
	public:
		ASTBinaryOp(std::shared_ptr<AST::ASTExpression> lhs, std::shared_ptr<AST::ASTExpression> rhs, Sapphire::Operator& op)
			: lhs{ lhs }, rhs{ rhs }, op{ op } {}
	public:
		void Accept(ASTVisitor* visitor) const {
			visitor->Visit_BinaryOp(this);
		}
	public:
		std::shared_ptr<AST::ASTExpression> GetLhs() const { return lhs; }
		std::shared_ptr<AST::ASTExpression> GetRhs() const { return rhs; }
		Sapphire::Operator GetOperator() const { return op; }
	private:
		std::shared_ptr<AST::ASTExpression> lhs;
		std::shared_ptr<AST::ASTExpression> rhs;
		Sapphire::Operator& op;
	};
}