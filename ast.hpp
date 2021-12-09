#pragma once
#include <string>
#include <vector>
#include <memory>
#include <optional>
#include <functional>

enum class ASTNodeType {
	ASTNode,
	ASTProgram,
	//Expression
	ConstantExpr,
	VariableExpr,
	FuncDeclExpr,
	OperatorDeclExpr,
	ClassDeclExpr,
	CallOpExpr,
	MemberAccessExpr,
	BinOpExpr,
	//Statement
	VariableDecl,
	VariableAsgn,
	ReturnStmt
};

class ASTNode;

class ASTVisitor {
public:
	virtual void Visit(const ASTNode& node) const = 0;
};

class ASTNode {
public:
	virtual ASTNodeType GetType() const { return ASTNodeType::ASTNode; };
	virtual void Accept(ASTVisitor* visitor) const { visitor->Visit(*this); };
public:
	~ASTNode() {};
};

#pragma region SuperType

class ASTStatement : public ASTNode {
public:
	~ASTStatement() {};
};

class ASTExpression : public ASTStatement {
public:
	~ASTExpression() {};
};

#pragma endregion

class ASTProgram : public ASTNode {
public:
	ASTProgram(std::vector<std::unique_ptr<ASTStatement>> stmts) : stmts{ std::move(stmts) } {};
public:
	ASTNodeType GetType() const { return ASTNodeType::ASTProgram; };
public:
	std::vector<std::unique_ptr<ASTStatement>> stmts;
};

class ASTVariableDecl : public ASTStatement {										// VARIABLE DECLARATION
public:
	ASTVariableDecl(const std::string& identifier) : identifier{ identifier } {};
	ASTVariableDecl(const std::string& identifier, const std::string& hint) : identifier{ identifier }, hint{ hint } {};
public:
	ASTNodeType GetType() const { return ASTNodeType::VariableDecl; };
public:
	std::string identifier;
	std::optional<std::string> hint;
};

class ASTVariableAsgn : public ASTStatement {										// VARIABLE ASSIGNATION
public:
	ASTVariableAsgn(const std::string& identifier, const std::string& op, std::unique_ptr<ASTExpression> expression) :
		op{ op },
		identifier{ identifier },
		expression{ std::move(expression) }
	{};
public:
	ASTNodeType GetType() const { return ASTNodeType::VariableAsgn; };
public:
	std::string op;
	std::string identifier;
	std::unique_ptr<ASTExpression> expression;
};


class ASTReturnStatement : public ASTStatement {
public:
	ASTReturnStatement() = default;
	ASTReturnStatement(std::unique_ptr<ASTExpression> expression) : expression{ std::move(expression) } {};
public:
	ASTNodeType GetType() const { return ASTNodeType::ReturnStmt; };
public:
	std::optional<std::unique_ptr<ASTExpression>> expression;
};
//----------------------------------------------------------------------------------------------------------------





class ASTFuncDecl : public ASTExpression {											// FUNCTION DECLARATION
public:
	ASTFuncDecl(std::vector<std::unique_ptr<ASTStatement>> body, std::vector<std::unique_ptr<ASTVariableDecl>> args) :
		body{ std::move(body) },
		args{ std::move(args) }
	{};
public:
	ASTNodeType GetType() const { return ASTNodeType::FuncDeclExpr; };
public:
	std::vector<std::unique_ptr<ASTStatement>> body;
	std::vector<std::unique_ptr<ASTVariableDecl>> args;
};

class ASTOperatorDecl : public ASTExpression {										// OPERATOR DECLARATION
public:
	ASTOperatorDecl(const std::string& op, std::vector<std::unique_ptr<ASTStatement>> body, std::vector<std::unique_ptr<ASTVariableDecl>> args) :
		op{ op },
		body{ std::move(body) },
		args{ std::move(args) }
	{};
public:
	ASTNodeType GetType() const { return ASTNodeType::OperatorDeclExpr; };
public:
	std::string op;
	std::vector<std::unique_ptr<ASTStatement>> body;
	std::vector<std::unique_ptr<ASTVariableDecl>> args;
};

class ASTClassDecl : public ASTExpression {											// CLASS DECLARATION
public:
	ASTClassDecl(std::vector<std::unique_ptr<ASTFuncDecl>> funcs, std::vector<std::unique_ptr<ASTOperatorDecl>> opers) :
		funcs{ std::move(funcs) },
		opers{ std::move(opers) }
	{};
public:
	ASTNodeType GetType() const { return ASTNodeType::ClassDeclExpr; };
public:
	std::vector<std::unique_ptr<ASTFuncDecl>> funcs;
	std::vector<std::unique_ptr<ASTOperatorDecl>> opers;
};


class ASTCallOperation : public ASTExpression {										// CALL OPERATION
public:
	ASTCallOperation(std::unique_ptr<ASTExpression> callable) : callable{ std::move(callable) } {};
	ASTCallOperation(std::unique_ptr<ASTExpression> callable, std::vector<std::unique_ptr<ASTExpression>> args) : 
		callable{ std::move(callable) }, 
		args{ std::move(args) } 
	{};
public:
	ASTNodeType GetType() const { return ASTNodeType::CallOpExpr; };
public:
	std::vector<std::unique_ptr<ASTExpression>> args;
	std::unique_ptr<ASTExpression> callable;
};

class ASTMemberAccessOperation : public ASTExpression {								// MEMBER ACCESS OPERATION
public:
	ASTMemberAccessOperation(std::unique_ptr<ASTExpression> object, const std::string& attribute) : object{ std::move(object) }, attribute{ attribute } {};
public:
	ASTNodeType GetType() const { return ASTNodeType::MemberAccessExpr; };
public:
	std::unique_ptr<ASTExpression> object;
	std::string attribute;
};

class ASTVariableExpr : public ASTExpression {										// VARIABLE EXPRESSION
public:
	ASTVariableExpr(const std::string& name) : name{ name } {};
public:
	ASTNodeType GetType() const { return ASTNodeType::VariableExpr; };
public:
	std::string name;
};

class ASTConstantExpr : public ASTExpression {										// CONSTANT EXPRESSION
public:
	ASTConstantExpr(const std::string& value) : value{ value } {};
public:
	ASTNodeType GetType() const { return ASTNodeType::ConstantExpr; };
public:
	std::string value;
};

class ASTBinOperation : public ASTExpression {										// BINARY OPERATION
public:
	ASTBinOperation(const std::string& op, std::unique_ptr<ASTExpression> lhs, std::unique_ptr<ASTExpression> rhs):
		op{ op },
		lhs{ std::move(lhs) },
		rhs{ std::move(rhs) }
	{};
public:
	ASTNodeType GetType() const { return ASTNodeType::BinOpExpr; };
public:
	std::string op;
	std::unique_ptr<ASTExpression> lhs;
	std::unique_ptr<ASTExpression> rhs;
};