#include "parser.hpp"
#include <stdexcept>
#include <map>
#include <iostream>

#include "except.hpp"

void ThrowErr(const std::string& str)
{
	throw std::runtime_error(str);
}

int GetTokenPrecedence(const TokenConsumer& consumer, int pos = 0)
{
	std::map<std::string, int> precedenceTable
	{
		{"&", 5},
		{"|", 5},

		{"+", 6},
		{"-", 6},

		{"*", 7},
		{"/", 7},
		{"%", 7},

		{"!", 8},
		{"~", 8},

		{".", 9},
		{"(", 9},
		{"[", 9},
	};
	Token token = consumer.Peek(pos);
	if (!precedenceTable.count(consumer.GetStr(token.offset, token.size)))
		return -1;
	return precedenceTable[consumer.GetStr(token.offset, token.size)];
}

//EXPRESSION

std::vector<std::unique_ptr<ASTExpression>> ParseArgsAsgn(TokenConsumer& consumer)
{
	std::vector<std::unique_ptr<ASTExpression>> args;
	while (1) {
		consumer.IgnoreTrmn();
		if (std::unique_ptr<ASTExpression> arg = ParseExpression(consumer)) {
			args.push_back(std::move(arg));
		}
		else {
			throw CSapphireException{ CSapphireExceptionType::EXPRINV, consumer.Peek().offset, consumer.src };
		}
		if (consumer.Peek().type != TokenType::COMMA)
			break;
		consumer.Consume();
	}
	return std::move(args);
}

std::vector<std::unique_ptr<ASTVariableDecl>> ParseArgsDecl(TokenConsumer& consumer)
{
	std::vector<std::unique_ptr<ASTVariableDecl>> args;
	while (1) {
		consumer.IgnoreTrmn();
		if (std::unique_ptr<ASTVariableDecl> arg = ParseVariableDecl(consumer)) {
			args.push_back(std::move(arg));
		}
		else {
			throw CSapphireException{ CSapphireExceptionType::SYNTAXERR, consumer.Peek().offset, consumer.src };
		}
		if (consumer.Peek().type != TokenType::COMMA)
			break;
		consumer.Consume();
	}
	return std::move(args);
}

std::unique_ptr<ASTFuncDecl> ParseFuncDecl(TokenConsumer& consumer)
{
	if (consumer.GetStr(consumer.Peek().offset, consumer.Peek().size) != "func")
		return nullptr;
	consumer.Consume();

	if (consumer.Consume().type != TokenType::LPAR)
		return nullptr;

	std::vector<std::unique_ptr<ASTVariableDecl>> args;

	if (consumer.Peek().type != TokenType::RPAR)
		args = ParseArgsDecl(consumer);

	consumer.Consume();

	if (consumer.Consume().type != TokenType::BLOCKSTART)
		throw CSapphireException{ CSapphireExceptionType::SYNTAXERR, consumer.Peek().offset, consumer.src };

	std::vector<std::unique_ptr<ASTStatement>> body;

	while (1) {
		Token next = consumer.Peek();
		if (next.type == TokenType::BLOCKEND)
			break;

		if (std::unique_ptr<ASTStatement> stmt = ParseStatement(consumer)) {
			body.push_back(std::move(stmt));
		}
		else {
			throw CSapphireException{ CSapphireExceptionType::STMTINV, next.offset, consumer.src };
		}
	}
	consumer.Consume();
	return std::make_unique<ASTFuncDecl>(std::move(body), std::move(args));
}

std::unique_ptr<ASTConstantExpr> ParseConstantExpr(TokenConsumer& consumer)
{
	Token constant = consumer.Peek();
	if (constant.type != TokenType::NUMBER)
		return nullptr;
	consumer.Consume();
	return std::make_unique<ASTConstantExpr>(consumer.GetStr(constant.offset, constant.size));
}

std::unique_ptr<ASTVariableExpr> ParseVarExpr(TokenConsumer& consumer)
{
	Token identifier = consumer.Consume();
	return std::make_unique<ASTVariableExpr>(consumer.GetStr(identifier.offset, identifier.size));
}

std::unique_ptr<ASTCallOperation> ParseCallOperation(TokenConsumer& consumer, std::unique_ptr<ASTExpression> lhs)
{
	consumer.Consume(); //consume LPAR

	if (consumer.Peek().type == TokenType::RPAR) {
		consumer.Consume();
		return std::make_unique<ASTCallOperation>(std::move(lhs));
	}

	std::vector<std::unique_ptr<ASTExpression>> args = ParseArgsAsgn(consumer);
	consumer.Consume();

	return std::make_unique<ASTCallOperation>(std::move(lhs), std::move(args));
}

std::unique_ptr<ASTMemberAccessOperation> ParseMemberAccessOperation(TokenConsumer& consumer, std::unique_ptr<ASTExpression> lhs)
{
	consumer.Consume(); //consume .
	Token attribute = consumer.Consume();
	if (attribute.type != TokenType::NAME)
		throw CSapphireException{ CSapphireExceptionType::SYNTAXERR, attribute.offset, consumer.src };
	return std::make_unique<ASTMemberAccessOperation>(std::move(lhs), consumer.GetStr(attribute.offset, attribute.size));
}

std::unique_ptr<ASTExpression> ParseParenExpr(TokenConsumer& consumer)
{
	if (consumer.Peek().type != TokenType::LPAR)
		return nullptr;
	consumer.Consume();
	std::unique_ptr<ASTExpression> expr = ParseExpression(consumer);
	if (consumer.Peek().type != TokenType::RPAR)
		throw CSapphireException{ CSapphireExceptionType::RPARMISSING, consumer.Peek().offset, consumer.src };
	consumer.Consume();
	return std::move(expr);
}

std::unique_ptr<ASTExpression> ParseIdentifierExpr(TokenConsumer& consumer)
{
	return ParseVarExpr(consumer);
}

std::unique_ptr<ASTExpression> ParseUnaryOp(TokenConsumer& consumer)
{
	return nullptr;
}

std::unique_ptr<ASTExpression> ParseBinOpRhs(TokenConsumer& consumer, int ExprPrec, std::unique_ptr<ASTExpression> lhs)
{
	while (1) {
		int tokPrec = GetTokenPrecedence(consumer);

		if (tokPrec < ExprPrec)
			return std::move(lhs);

		Token op = consumer.Peek(); //Check

		switch (op.type) {
		case TokenType::LPAR: {
			lhs = ParseCallOperation(consumer, std::move(lhs));
			continue;
		}
		case TokenType::DOT: {
			lhs = ParseMemberAccessOperation(consumer, std::move(lhs));
			continue;
		}
		}

		consumer.Consume();
		std::unique_ptr<ASTExpression> rhs = ParsePrimaryExpr(consumer);
		if (!rhs)
			return nullptr;

		int nextPrec = GetTokenPrecedence(consumer);

		if (tokPrec < nextPrec) {
			rhs = ParseBinOpRhs(consumer, tokPrec + 1, std::move(rhs));
			if (!rhs)
				return nullptr;
		}

		lhs = std::make_unique<ASTBinOperation>(consumer.GetStr(op.offset, op.size), std::move(lhs), std::move(rhs));
	}
}

std::unique_ptr<ASTExpression> ParsePrimaryExpr(TokenConsumer& consumer)
{
	switch (consumer.Peek().type) {
	case TokenType::NAME: {
		return ParseIdentifierExpr(consumer);
	}
	case TokenType::NUMBER: {
		return ParseConstantExpr(consumer);
	}
	case TokenType::LPAR: {
		return ParseParenExpr(consumer);
	}
	case TokenType::KEYWORD: {
		return ParseFuncDecl(consumer);
	}
	default: {
		return nullptr;
	}
	}
}

std::unique_ptr<ASTExpression> ParseExpression(TokenConsumer& consumer)
{
	std::unique_ptr<ASTExpression> lhs = ParsePrimaryExpr(consumer);
	if (!lhs)
		return nullptr;
	return ParseBinOpRhs(consumer, 0, std::move(lhs));
}

//STATEMENT

std::unique_ptr<ASTVariableDecl> ParseVariableDecl(TokenConsumer& consumer)
{
	if (consumer.Peek().type != TokenType::NAME)
		return nullptr;

	Token identifier = consumer.Consume();

	if (consumer.Peek().type != TokenType::COLON)
		return std::make_unique<ASTVariableDecl>(consumer.GetStr(identifier.offset, identifier.size));

	consumer.Consume(); //consume colon

	Token hint = consumer.Consume();

	if (hint.type != TokenType::NAME)
		throw CSapphireException{ CSapphireExceptionType::HINTEXPECTED, hint.offset, consumer.src };

	return std::make_unique<ASTVariableDecl>(consumer.GetStr(identifier.offset, identifier.size),
		consumer.GetStr(hint.offset, hint.size));
}

std::unique_ptr<ASTVariableAsgn> ParseVariableAsgn(TokenConsumer& consumer)
{
	Token identifier = consumer.Consume();
	Token op = consumer.Consume();
	std::unique_ptr<ASTExpression> expression = ParseExpression(consumer);

	if (expression == nullptr)
		throw CSapphireException{ CSapphireExceptionType::EXPRINV, op.offset, consumer.src };

	return std::make_unique<ASTVariableAsgn>
		(
			consumer.GetStr(identifier.offset, identifier.size),
			consumer.GetStr(op.offset, op.size),
			std::move(expression)
		);
}

std::unique_ptr<ASTReturnStatement> ParseReturnStmt(TokenConsumer& consumer)
{
	consumer.Consume(); //consume return keyword
	if (std::unique_ptr<ASTExpression> expr = ParseExpression(consumer))
		return std::make_unique<ASTReturnStatement>(std::move(expr));
	return std::make_unique<ASTReturnStatement>();
}

std::unique_ptr<ASTStatement> ParseIdentifierStmt(TokenConsumer& consumer)
{
	if (consumer.Peek(1).type == TokenType::OPASGN)
		return ParseVariableAsgn(consumer);

	if (GetTokenPrecedence(consumer, 1) == -1)
		return ParseVariableDecl(consumer);

	return ParseExpression(consumer);
}

std::unique_ptr<ASTStatement> ParseKeywordStmt(TokenConsumer& consumer)
{
	Token current = consumer.Peek();
	std::string keyword = consumer.GetStr(current.offset, current.size);

	if (keyword == "return")
		return ParseReturnStmt(consumer);

	return nullptr;
}

std::unique_ptr<ASTStatement> ParseStatement(TokenConsumer& consumer)
{
	Token current = consumer.Peek();

	switch (current.type) {
	case TokenType::TRMN: {
		consumer.Consume();
		return ParseStatement(consumer);
	}
	case TokenType::NAME: {
		std::unique_ptr<ASTStatement> stmt = ParseIdentifierStmt(consumer);
		Token trmn = consumer.Consume();
		if (trmn.type == TokenType::TRMN)
			return std::move(stmt);
		throw CSapphireException{ CSapphireExceptionType::TRMNMISSING, trmn.offset, consumer.src };
	}
	case TokenType::KEYWORD: {
		std::unique_ptr<ASTStatement> stmt = ParseKeywordStmt(consumer);
		Token trmn = consumer.Consume();
		if (trmn.type == TokenType::TRMN)
			return std::move(stmt);
		throw CSapphireException{ CSapphireExceptionType::TRMNMISSING, trmn.offset, consumer.src };
	}
	default: {
		std::unique_ptr<ASTExpression> expr = ParseExpression(consumer);

		if (!expr)
			throw CSapphireException{ CSapphireExceptionType::SYNTAXERR, current.offset, consumer.src };

		Token trmn = consumer.Consume();
		if (trmn.type == TokenType::TRMN)
			return std::move(expr);
		throw CSapphireException{ CSapphireExceptionType::TRMNMISSING, trmn.offset, consumer.src };
	}
	}

	return nullptr;
}

std::unique_ptr<ASTProgram> ParseProgram(TokenConsumer& consumer)
{
	std::vector<std::unique_ptr<ASTStatement>> r;
	while (consumer.Size() > 0) {
		if (std::unique_ptr<ASTStatement> c = ParseStatement(consumer)) {
			r.push_back(std::move(c));
		}
		else {
			throw CSapphireException{ CSapphireExceptionType::STMTINV, consumer.Peek().offset, consumer.src };
		}
	}
	return std::make_unique<ASTProgram>(std::move(r));
}