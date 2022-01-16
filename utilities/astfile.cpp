#include "astfile.hpp"
#include <iostream>

std::u8string IndentStr(std::u8string_view str) {
	std::u8string r = u8"\t";

	for (int i = 0; i < str.length(); i++) {
		r += str[i];
		if (str[i] == u8'\n')
			r += u8"\t";
	}

	return r;
}

std::u8string AstFile::Produce(std::shared_ptr<AST::ASTProgram> program)
{
	ASTViewer viewer{};
	program.get()->Accept(&viewer);
	return viewer.GetResult();
}

std::u8string AstFile::ASTViewer::GetResult()
{
	return rstack.top();
}

void AstFile::ASTViewer::Visit_Program(const AST::ASTProgram* node)
{
	std::u8string r = u8"Program: \n";

	node->GetBlock().get()->Accept(this);

	std::u8string blockview = rstack.top();
	rstack.pop();

	blockview = IndentStr(blockview);
	r += blockview;
	rstack.push(r);
}

void AstFile::ASTViewer::Visit_StatementBlock(const AST::ASTStatementBlock* node)
{
	std::u8string r = u8"StatementBlock: \n";

	for (int i = node->GetSize() - 1; i > -1; i--) {
		node->GetStatement(i).get()->Accept(this);
	}

	for (int i = 0; i < node->GetSize(); i++) {
		std::u8string stmtr = u8"statement: \n";
		std::u8string stmt = rstack.top();
		rstack.pop();
		stmt = IndentStr(stmt);
		stmtr += stmt;
		r += IndentStr(stmtr);
		r += u8"\n";
	}

	rstack.push(r);
}

void AstFile::ASTViewer::Visit_Literal(const AST::ASTLiteral* node)
{
	std::u8string r = u8"Literal: \n";
	r += u8"\tvalue: ";
	r += node->GetValue();
	r += u8"\n\ttype: ";
	node->GetType().get()->Accept(this);
	r += rstack.top();
	rstack.pop();
	rstack.push(r);
}

void AstFile::ASTViewer::Visit_VarDecl(const AST::ASTVarDecl* node)
{
	std::u8string r = u8"VarDecl: \n";
	r += u8"\tidentifier: ";
	r += node->GetIdentifier();
	r += u8"\n\ttype: ";
	node->GetType().get()->Accept(this);
	r += rstack.top();
	rstack.pop();
	rstack.push(r);
}

void AstFile::ASTViewer::Visit_VarAsgn(const AST::ASTVarAsgn* node)
{
	std::u8string r = u8"VarAsgn: \n";
	r += u8"\tidentifier: ";
	r += node->GetIdentifier();
	r += u8"\n\toperator: ";
	r += (const char8_t*)node->GetOperator()._to_string();
	r += u8"\n\tvalue: ";
	node->GetValue().get()->Accept(this);
	r += IndentStr(IndentStr(rstack.top()));
	rstack.pop();
	rstack.push(r);
}

void AstFile::ASTViewer::Visit_VarDeclAsgn(const AST::ASTVarDeclAsgn* node)
{
	std::u8string r = u8"VarDeclAsgn: \n"; 
	r += u8"\tidentifier: ";
	r += node->GetIdentifier();
	r += u8"\n\ttype: ";
	node->GetType().get()->Accept(this);
	r += rstack.top();
	r += u8"\n\tvalue: ";
	node->GetValue().get()->Accept(this);
	r += IndentStr(IndentStr(rstack.top()));
	rstack.pop();
	rstack.push(r);
}

void AstFile::ASTViewer::Visit_Primitive(const AST::ASTPrimitive* node)
{
	std::u8string r;

	switch (node->GetType()) {
	case Sapphire::PrimitiveType::I8:
		r += u8"8 bit integer";
		break;
	case Sapphire::PrimitiveType::I16:
		r += u8"16 bit integer";
		break;
	case Sapphire::PrimitiveType::I32:
		r += u8"32 bit integer";
		break;
	case Sapphire::PrimitiveType::I64:
		r += u8"64 bit integer";
		break;
	case Sapphire::PrimitiveType::BYTE:
		r += u8"byte";
		break;
	case Sapphire::PrimitiveType::U16:
		r += u8"16 bit unsigned integer";
		break;
	case Sapphire::PrimitiveType::U32:
		r += u8"32 bit unsigned integer";
		break;
	case Sapphire::PrimitiveType::U64:
		r += u8"64 bit unsigned integer";
		break;
	case Sapphire::PrimitiveType::F32:
		r += u8"32 bit floating point";
		break;
	case Sapphire::PrimitiveType::F64:
		r += u8"64 bit floating point";
		break;
	case Sapphire::PrimitiveType::BOOL:
		r += u8"boolean";
		break;
	default:
		r += u8"????";
	}

	rstack.push(r);
}
