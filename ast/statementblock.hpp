#pragma once
#include "statement.hpp"
#include <vector>
#include <memory>

namespace AST {
	class ASTStatementBlock : public ASTNode {
	public:
		ASTStatementBlock() = default;
		ASTStatementBlock(std::vector<std::shared_ptr<ASTStatement>> statements) : statements{ statements } {}
	public:
		void Accept(ASTVisitor* visitor) const {
			visitor->Visit_StatementBlock(this);
		}
	public:
		size_t GetSize() const { return statements.size(); }
		std::shared_ptr<ASTStatement> GetStatement(int idx) const { return statements[idx]; }
		void PushStatement(std::shared_ptr<ASTStatement> statement) { statements.push_back(statement); }
	private:
		std::vector<std::shared_ptr<ASTStatement>> statements;
	};
}