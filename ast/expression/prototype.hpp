#pragma once
#include "../expression.hpp"
#include "../asttype.hpp"
#include "../statement/argdecl.h"
#include <memory>
#include <vector>

namespace AST {
	class ASTPrototype : public ASTExpression {
	public:
		ASTPrototype(std::shared_ptr<ASTStatementBlock> body, std::shared_ptr<ASTType> returnType) :
			body{ body },
			returnType{ returnType } {}
		ASTPrototype(std::vector<std::shared_ptr<ASTArgDecl>> args, std::shared_ptr<ASTStatementBlock> body, std::shared_ptr<ASTType> returnType) :
			args{ args },
			body{ body },
			returnType{ returnType } {}
	public:
		void Accept(ASTVisitor* visitor) const {
			visitor->Visit_Prototype(this);
		}
	public:
		int GetArgNumber() const { return args.size(); }
		std::shared_ptr<ASTArgDecl> GetArg(int idx) const { return args[idx]; }
		std::shared_ptr<ASTStatementBlock> GetBody() const { return body; }
		std::shared_ptr<ASTType> GetType() const { return returnType; }

	private:
		std::vector<std::shared_ptr<ASTArgDecl>> args;
		std::shared_ptr<ASTStatementBlock> body;
		std::shared_ptr<ASTType> returnType;
	};
}