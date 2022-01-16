#pragma once
#include "statementblock.hpp"

namespace AST {
	class ASTProgram : public ASTNode {
	public:
		ASTProgram(std::shared_ptr<ASTStatementBlock> block) : block{ block } {}
	public:
		void Accept(ASTVisitor* visitor) const {
			visitor->Visit_Program(this);
		}
	public:
		std::shared_ptr<ASTStatementBlock> GetBlock() const { return block; }
	private:
		std::shared_ptr<ASTStatementBlock> block;
	};
}