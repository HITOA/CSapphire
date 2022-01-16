#pragma once
#include "../statement.hpp"
#include "../asttype.hpp"
#include "../expression.hpp"

#include <string>
#include <memory>

namespace AST {
	class ASTVarDeclAsgn : public ASTStatement {
	public:
		ASTVarDeclAsgn(std::u8string_view identifier, 
			std::shared_ptr<ASTType> type,
			std::shared_ptr<ASTExpression> value) :
			identifier{ identifier },
			type{ type },
			value{ value }{};
	public:
		void Accept(ASTVisitor* visitor) const {
			visitor->Visit_VarDeclAsgn(this);
		}
	public:
		std::u8string_view GetIdentifier() const { return identifier; }
		std::shared_ptr<ASTType> GetType() const { return type; }
		std::shared_ptr<ASTExpression> GetValue() const { return value; }
	private:
		std::u8string_view identifier;
		std::shared_ptr<ASTType> type;
		std::shared_ptr<ASTExpression> value;
	};
}