#pragma once
#include "../statement.hpp"
#include "../asttype.hpp"

#include <string>

namespace AST {
	class ASTVarDecl : public ASTStatement {
	public:
		ASTVarDecl(std::u8string_view identifier, std::shared_ptr<ASTType> type) :
			identifier{ identifier }, 
			type{ type } {};
	public:
		void Accept(ASTVisitor* visitor) const {
			visitor->Visit_VarDecl(this);
		}
	public:
		std::u8string_view GetIdentifier() const { return identifier; }
		std::shared_ptr<ASTType> GetType() const { return type; }
	private:
		std::u8string_view identifier;
		std::shared_ptr<ASTType> type;
	};
}