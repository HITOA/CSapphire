#include "parser.hpp"
#include "parserexcept.hpp"

void Parser::Parser::SetConsumer(Tokenizer::Tokenizer::Consumer& consumer)
{
	this->consumer = consumer;
}

std::shared_ptr<AST::ASTProgram> Parser::Parser::ParseProgram()
{
	return std::shared_ptr<AST::ASTProgram>{new AST::ASTProgram{ParseStatementBlock()}};
}

std::shared_ptr<AST::ASTStatement> Parser::Parser::ParseStatement()
{
	if (IsTerminator(0)) {
		consumer.Consume();
		return nullptr;
	}

	auto token = consumer.Peek();

	switch (token.second._value) {
	case Tokenizer::TokenType::TOKEN_WORD:
	{
		if (int r = TryParseVarDecl()) {
			if (r == 1)
				return ParseVarDecl();
			return ParseVarDeclAsgn();
		}
		if (int r = TryParseVarAsgn()) {
			return ParseVarAsgn();
		}
		break;
	}
	case Tokenizer::TokenType::TOKEN_PRIMITIVE_TYPE:
	{
		if (int r = TryParseVarDecl()) {
			if (r == 1)
				return ParseVarDecl();
			return ParseVarDeclAsgn();
		}
		break;
	}
	case Tokenizer::TokenType::TOKEN_KEYWORD:
	{
		break;
	}
	}

	throw ParserException{ ParserExceptionType::BAD_TOKEN, token };
}

std::shared_ptr<AST::ASTStatementBlock> Parser::Parser::ParseStatementBlock()
{
	std::shared_ptr<AST::ASTStatementBlock> block{ new AST::ASTStatementBlock{} };

	while (consumer.GetSize() > 0 && 
		consumer.Peek().second._value != Tokenizer::TokenType::TOKEN_BLOCKEND) 
	{
		if (auto v = ParseStatement())
			block.get()->PushStatement(v);
	}

	return block;
}

std::shared_ptr<AST::ASTExpression> Parser::Parser::ParseExpression()
{
	auto lhs = ParsePrimaryExpr();
	return ParseBinaryOp(100, lhs);
}

std::shared_ptr<AST::ASTType> Parser::Parser::ParseType()
{
	auto type = consumer.Peek();
	if (type.second._value == Tokenizer::TokenType::TOKEN_PRIMITIVE_TYPE)
		return ParsePrimitiveType();

	throw ParserException{ ParserExceptionType::BAD_TYPE, type };
}

std::shared_ptr<AST::ASTExpression> Parser::Parser::ParseParenExpr()
{
	consumer.Consume(); //Consume (
	std::shared_ptr<AST::ASTExpression> expr = ParseExpression();
	if (consumer.Peek().second._value != Tokenizer::TokenType::TOKEN_RPAR)
		throw ParserException{ ParserExceptionType::BAD_TOKEN, consumer.Peek(), "RPar missing."};
	consumer.Consume(); //Consume )
	return expr;
}

std::shared_ptr<AST::ASTExpression> Parser::Parser::ParsePrimaryExpr()
{
	auto token = consumer.Peek();

	if (token.first == u8"(")
		return ParseParenExpr();

	switch (token.second._value) {
	case Tokenizer::TokenType::TOKEN_NUMBER:
		return ParseLiteral();
	case Tokenizer::TokenType::TOKEN_STRING:
		return ParseLiteral();
	default:
		throw ParserException{ ParserExceptionType::BAD_TOKEN, token, "Unrocognized Primary Expression."};
	}
}

std::shared_ptr<AST::ASTPrimitive> Parser::Parser::ParsePrimitiveType()
{
	auto primitive = consumer.Consume();

	if (primitive.first == u8"i8")
		return std::shared_ptr<AST::ASTPrimitive>{ new AST::ASTPrimitive{ Sapphire::PrimitiveType::I8 } };
	if (primitive.first == u8"i16")
		return std::shared_ptr<AST::ASTPrimitive>{ new AST::ASTPrimitive{ Sapphire::PrimitiveType::I16 } };
	if (primitive.first == u8"i32")
		return std::shared_ptr<AST::ASTPrimitive>{ new AST::ASTPrimitive{ Sapphire::PrimitiveType::I32 } };
	if (primitive.first == u8"i64")
		return std::shared_ptr<AST::ASTPrimitive>{ new AST::ASTPrimitive{ Sapphire::PrimitiveType::I64 } };

	if (primitive.first == u8"byte")
		return std::shared_ptr<AST::ASTPrimitive>{ new AST::ASTPrimitive{ Sapphire::PrimitiveType::BYTE } };
	if (primitive.first == u8"u16")
		return std::shared_ptr<AST::ASTPrimitive>{ new AST::ASTPrimitive{ Sapphire::PrimitiveType::U16 } };
	if (primitive.first == u8"u32")
		return std::shared_ptr<AST::ASTPrimitive>{ new AST::ASTPrimitive{ Sapphire::PrimitiveType::U32 } };
	if (primitive.first == u8"u64")
		return std::shared_ptr<AST::ASTPrimitive>{ new AST::ASTPrimitive{ Sapphire::PrimitiveType::U64 } };

	if (primitive.first == u8"f32")
		return std::shared_ptr<AST::ASTPrimitive>{ new AST::ASTPrimitive{ Sapphire::PrimitiveType::F32 } };
	if (primitive.first == u8"f64")
		return std::shared_ptr<AST::ASTPrimitive>{ new AST::ASTPrimitive{ Sapphire::PrimitiveType::F64 } };

	if (primitive.first == u8"bool")
		return std::shared_ptr<AST::ASTPrimitive>{ new AST::ASTPrimitive{ Sapphire::PrimitiveType::BOOL } };

	throw ParserException{ ParserExceptionType::BAD_TYPE, primitive };
}

std::shared_ptr<AST::ASTExpression> Parser::Parser::ParseBinaryOp(int ExprPrec, std::shared_ptr<AST::ASTExpression> lhs)
{
	while (1) {
		int tokenPrec = GetTokenPrecedence(0, true);
		if (tokenPrec == -1)
			return lhs;
		if (tokenPrec > ExprPrec)
			return lhs;

		auto op = consumer.Consume(); //operator
		auto rhs = ParsePrimaryExpr();

		int nextPrec = GetTokenPrecedence(0, true);
		if (tokenPrec > nextPrec)
			rhs = ParseBinaryOp(tokenPrec - 1, rhs);

		lhs = std::shared_ptr<AST::ASTExpression>{ new AST::ASTBinaryOp{lhs, rhs, GetOperatorFromStr(op.first, true)} };
	}
}

std::shared_ptr<AST::ASTLiteral> Parser::Parser::ParseLiteral() //Incomplet
{
	auto token = consumer.Consume();
	std::shared_ptr<AST::ASTType> type{ new AST::ASTPrimitive{Sapphire::PrimitiveType::I32} };

	return std::shared_ptr<AST::ASTLiteral>{ new AST::ASTLiteral{ token.first, type } };
}

std::shared_ptr<AST::ASTVarDecl> Parser::Parser::ParseVarDecl()
{
	std::shared_ptr<AST::ASTType> type = ParseType();
	std::u8string_view identifier = consumer.Consume().first;

	consumer.Consume(); //Consume EOI

	return std::shared_ptr<AST::ASTVarDecl>{ new AST::ASTVarDecl{ identifier, type } };
}

std::shared_ptr<AST::ASTVarAsgn> Parser::Parser::ParseVarAsgn()
{
	std::u8string_view identifier = consumer.Consume().first;
	consumer.Consume(); //Consumer Asgn Operator
	std::shared_ptr<AST::ASTExpression> value = ParseExpression();

	if (!IsTerminator(0))
		throw ParserException{ ParserExceptionType::TERMINATOR_MISSING, consumer.Peek() };
	consumer.Consume();
	return std::shared_ptr<AST::ASTVarAsgn>{ new AST::ASTVarAsgn{ identifier, value } };
}

std::shared_ptr<AST::ASTVarDeclAsgn> Parser::Parser::ParseVarDeclAsgn()
{
	std::shared_ptr<AST::ASTType> type = ParseType();
	std::u8string_view identifier = consumer.Consume().first;
	consumer.Consume();
	std::shared_ptr<AST::ASTExpression> value = ParseExpression();
	return std::shared_ptr<AST::ASTVarDeclAsgn>{new AST::ASTVarDeclAsgn{ identifier, type, value }};
}

bool Parser::Parser::IsTerminator(int i)
{
	auto token = consumer.Peek(i);
	return token.second._value == Tokenizer::TokenType::TOKEN_NEWLINE
		|| token.second._value == Tokenizer::TokenType::TOKEN_SEMICOLON
		|| token.second._value == Tokenizer::TokenType::TOKEN_EOF;
}

bool Parser::Parser::CanBeType(int i)
{
	auto token = consumer.Peek(i);
	return token.second._value == Tokenizer::TokenType::TOKEN_PRIMITIVE_TYPE
		|| token.second._value == Tokenizer::TokenType::TOKEN_WORD;
}

int Parser::Parser::GetTokenPrecedence(int i, bool isBinary)
{
	auto token = consumer.Peek(i);

	for (Sapphire::Operator op : Sapphire::sappOperator) {
		if (op.isBinary != isBinary)
			continue;
		if (token.first == op.name)
			return op.precedence;
	}
	return -1;
}

Sapphire::Operator& Parser::Parser::GetOperatorFromStr(std::u8string_view str, bool isBinary)
{
	for (Sapphire::Operator& op : Sapphire::sappOperator) {
		if (op.isBinary != isBinary)
			continue;
		if (str == op.name)
			return op;
	}
	throw std::logic_error("this is not an operator.");
}

int Parser::Parser::TryParseVarDecl()
{
	if (!CanBeType(0))
		return 0;

	auto type = consumer.Peek();
	auto identifier = consumer.Peek(1);
	
	if (identifier.second._value != Tokenizer::TokenType::TOKEN_WORD)
		return 0;

	if (IsTerminator(2))
		return 1;

	if (consumer.Peek(2).first.length() > 0 && consumer.Peek(2).first[0] == u8'=')
		return 2;

	return 0;
}

int Parser::Parser::TryParseVarAsgn()
{
	if (consumer.Peek().second._value != Tokenizer::TokenType::TOKEN_WORD)
		return 0;

	auto op1 = consumer.Peek(1);

	if (op1.second._value != Tokenizer::TokenType::TOKEN_OPERATOR)
		return 0;

	if (consumer.Peek(2).second._value == Tokenizer::TokenType::TOKEN_OPERATOR)
		return 2;
	return 1;
}
