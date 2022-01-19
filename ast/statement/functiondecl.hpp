#pragma once
#include "../statement.hpp"
#include "../expression/prototype.hpp"

#include <string>

namespace AST {
	class ASTFunctionDecl : public ASTStatement {
	public:
		ASTFunctionDecl(std::u8string_view identifier, std::shared_ptr<ASTPrototype> prototype) :
			identifier{ identifier },
			prototype{ prototype } {};
	public:
		void Accept(ASTVisitor* visitor) const {
			visitor->Visit_FunctionDecl(this);
		}
	public:
		std::u8string_view GetIdentifier() const { return identifier; }
		std::shared_ptr<ASTPrototype> GetPrototype() const { return prototype; }
	private:
		std::u8string_view identifier;
		std::shared_ptr<ASTPrototype> prototype;
	};
}