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
		void SetSource(std::u8string_view source);
	public:
		std::shared_ptr<AST::ASTProgram> ParseProgram();
	private:
		std::shared_ptr<AST::ASTStatement> ParseStatement();
		std::shared_ptr<AST::ASTStatementBlock> ParseStatementBlock();
		std::shared_ptr<AST::ASTExpression> ParseExpression();
		std::shared_ptr<AST::ASTType> ParseType();
	private:
		std::shared_ptr<AST::ASTExpression> ParseParenExpr();
		std::shared_ptr<AST::ASTExpression> ParsePrimaryExpr();
		std::vector<std::shared_ptr<AST::ASTArgDecl>> ParseArgDeclList();
	private:
		std::shared_ptr<AST::ASTPrimitive> ParsePrimitiveType();
		
		std::shared_ptr<AST::ASTExpression> ParseBinaryOp(int ExprPrec, std::shared_ptr<AST::ASTExpression> lhs);
		std::shared_ptr<AST::ASTLiteral> ParseLiteral();
		std::shared_ptr<AST::ASTPrototype> ParsePrototype();

		std::shared_ptr<AST::ASTArgDecl> ParseArgDecl();
		std::shared_ptr<AST::ASTFunctionDecl> ParseFunctionDecl();
		std::shared_ptr<AST::ASTVarDecl> ParseVarDecl();			// Variable Declaration
		std::shared_ptr<AST::ASTVarAsgn> ParseVarAsgn();			// Variable Assignation
		std::shared_ptr<AST::ASTVarDeclAsgn> ParseVarDeclAsgn();	// Variable Declaration & Assignation
	private:
		bool IsTerminator(int i);
		bool CanBeType(int i);
		void ConsumeNewLine();
		int GetTokenPrecedence(int i, bool isBinary);
		Sapphire::Operator& GetOperatorFromStr(std::u8string_view str, bool isBinary);

		int TryParseVarDecl();
		int TryParseVarAsgn();
	private:
		Tokenizer::Tokenizer::Consumer consumer;
		std::u8string_view source;
	};
}