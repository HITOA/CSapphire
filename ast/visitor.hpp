#pragma once
#include <stdexcept>

namespace AST {
#pragma region astdeclaration
	class ASTProgram;

	class ASTStatementBlock;

	class ASTLiteral;
	class ASTBinaryOp;
	class ASTPrototype;

	class ASTArgDecl;
	class ASTFunctionDecl;
	class ASTVarDecl;
	class ASTVarAsgn;
	class ASTVarDeclAsgn;

	class ASTPrimitive;
#pragma endregion

	class ASTVisitor {
	public:
		virtual void Visit_Program(const ASTProgram* node) { throw std::logic_error{ "Not implemented." }; };

		virtual void Visit_StatementBlock(const ASTStatementBlock* node) { throw std::logic_error{ "Not implemented." }; };

		virtual void Visit_Literal(const ASTLiteral* node) { throw std::logic_error{ "Not implemented." }; };
		virtual void Visit_BinaryOp(const ASTBinaryOp* node) { throw std::logic_error{ "Not implemented." }; };
		virtual void Visit_Prototype(const ASTPrototype* node) { throw std::logic_error{ "Not implemented." }; };

		virtual void Visit_ArgDecl(const ASTArgDecl* node) { throw std::logic_error{ "Not implemented." }; };
		virtual void Visit_FunctionDecl(const ASTFunctionDecl* node) { throw std::logic_error{ "Not implemented." }; };
		virtual void Visit_VarDecl(const ASTVarDecl* node) { throw std::logic_error{ "Not implemented." }; };
		virtual void Visit_VarAsgn(const ASTVarAsgn* node) { throw std::logic_error{ "Not implemented." }; };
		virtual void Visit_VarDeclAsgn(const ASTVarDeclAsgn* node) { throw std::logic_error{ "Not implemented." }; };

		virtual void Visit_Primitive(const ASTPrimitive* node) { throw std::logic_error{ "Not implemented." }; };
	};
}