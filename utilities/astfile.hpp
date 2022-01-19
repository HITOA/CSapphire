#pragma once
#include "../ast/ast.hpp"
#include <stack>

namespace AstFile {
	class ASTViewer : public AST::ASTVisitor {
	public:
		std::u8string GetResult();
	public:
		void Visit_Program(const AST::ASTProgram* node);

		void Visit_StatementBlock(const AST::ASTStatementBlock* node);

		void Visit_BinaryOp(const AST::ASTBinaryOp* node);
		void Visit_Literal(const AST::ASTLiteral* node);
		void Visit_Prototype(const AST::ASTPrototype* node);

		void Visit_ArgDecl(const AST::ASTArgDecl* node);
		void Visit_FunctionDecl(const AST::ASTFunctionDecl* node);
		void Visit_VarDecl(const AST::ASTVarDecl* node);
		void Visit_VarAsgn(const AST::ASTVarAsgn* node);
		void Visit_VarDeclAsgn(const AST::ASTVarDeclAsgn* node);

		void Visit_Primitive(const AST::ASTPrimitive* node);
	private:
		std::stack<std::u8string> rstack;
	};

	std::u8string Produce(std::shared_ptr<AST::ASTProgram> program);
}