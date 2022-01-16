#pragma once

#include "visitor.hpp"

namespace AST {
	class ASTNode {
	public:
		~ASTNode() {};
	public:
		virtual void Accept(ASTVisitor* visitor) const = 0;
	};
}