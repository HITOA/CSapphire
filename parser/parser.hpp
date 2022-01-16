#pragma once
#include "../tokenizer/tokenizer.hpp"
#include "../ast/ast.hpp"
#include <memory>

namespace Parser {
	class Parser {
	public:
		Parser() {};
		~Parser() {}
	public:
		void SetConsumer(Tokenizer::Tokenizer::Consumer& consumer);
	public:
		std::shared_ptr<AST::ASTProgram> ParseProgram();
	private:
		std::shared_ptr<AST::ASTStatement> ParseStatement();
		std::shared_ptr<AST::ASTStatementBlock> ParseStatementBlock();
		std::shared_ptr<AST::ASTType> ParseType();
	private:
		std::shared_ptr<AST::ASTPrimitive> ParsePrimitiveType();

		std::shared_ptr<AST::ASTVarDecl> ParseVarDecl();			// Variable Declaration
		//std::shared_ptr<AST::ASTVarAsgn> ParseVarAsgn();			// Variable Assignation
		//std::shared_ptr<AST::ASTVarDeclAsgn> ParseVarDeclAsgn();	// Variable Declaration & Assignation
	private:
		bool IsTerminator(int i);
		bool CanBeType(int i);

		int TryParseVarDecl();
	private:
		Tokenizer::Tokenizer::Consumer consumer;
	};
}