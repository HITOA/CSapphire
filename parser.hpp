#pragma once
#include <memory>
#include <functional>
#include "lexer.hpp"
#include "ast.hpp"
#include "TokenConsumer.hpp"

//------------
//Expression Parse

std::vector<std::unique_ptr<ASTExpression>> ParseArgsAsgn(TokenConsumer& consumer);
std::vector<std::unique_ptr<ASTVariableDecl>> ParseArgsDecl(TokenConsumer& consumer);

std::unique_ptr<ASTFuncDecl> ParseFuncDecl(TokenConsumer& consumer);
std::unique_ptr<ASTConstantExpr> ParseConstantExpr(TokenConsumer& consumer);
std::unique_ptr<ASTVariableExpr> ParseVarExpr(TokenConsumer& consumer);

std::unique_ptr<ASTCallOperation> ParseCallOperation(TokenConsumer& consumer, std::unique_ptr<ASTExpression> lhs);
std::unique_ptr<ASTMemberAccessOperation> ParseMemberAccessOperation(TokenConsumer& consumer, std::unique_ptr<ASTExpression> lhs);

std::unique_ptr<ASTExpression> ParseParenExpr(TokenConsumer& consumer);
std::unique_ptr<ASTExpression> ParseIdentifierExpr(TokenConsumer& consumer);
std::unique_ptr<ASTExpression> ParseUnaryOp(TokenConsumer& consumer);
std::unique_ptr<ASTExpression> ParseBinOpRhs(TokenConsumer& consumer, int ExprPrec, std::unique_ptr<ASTExpression> lhs);

std::unique_ptr<ASTExpression> ParsePrimaryExpr(TokenConsumer& consumer);
std::unique_ptr<ASTExpression> ParseExpression(TokenConsumer& consumer);

//------------
//Statement Parse

std::unique_ptr<ASTVariableDecl> ParseVariableDecl(TokenConsumer& consumer);
std::unique_ptr<ASTVariableAsgn> ParseVariableAsgn(TokenConsumer& consumer);
std::unique_ptr<ASTReturnStatement> ParseReturnStmt(TokenConsumer& consumer);

std::unique_ptr<ASTStatement> ParseIdentifierStmt(TokenConsumer& consumer);
std::unique_ptr<ASTStatement> ParseKeywordStmt(TokenConsumer& consumer);

std::unique_ptr<ASTStatement> ParseStatement(TokenConsumer& consumer);

//------------

std::unique_ptr<ASTProgram> ParseProgram(TokenConsumer& consumer);

