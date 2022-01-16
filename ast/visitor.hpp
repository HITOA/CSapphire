#pragma once

namespace AST {
#pragma region astdeclaration
	class ASTProgram;

	class ASTStatementBlock;

	class ASTLiteral;

	class ASTVarDecl;
	class ASTVarAsgn;
	class ASTVarDeclAsgn;

	class ASTPrimitive;
#pragma endregion

	class ASTVisitor {
	public:
		virtual void Visit_Program(const ASTProgram* node) = 0;

		virtual void Visit_StatementBlock(const ASTStatementBlock* node) = 0;

		virtual void Visit_Literal(const ASTLiteral* node) = 0;

		virtual void Visit_VarDecl(const ASTVarDecl* node) = 0;
		virtual void Visit_VarAsgn(const ASTVarAsgn* node) = 0;
		virtual void Visit_VarDeclAsgn(const ASTVarDeclAsgn* node) = 0;

		virtual void Visit_Primitive(const ASTPrimitive* node) = 0;
	};
}