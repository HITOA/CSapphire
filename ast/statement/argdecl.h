#pragma once
#include "../statement.hpp"
#include "../asttype.hpp"
#include "../expression.hpp"

#include <string>
#include <memory>
#include <optional>

namespace AST {
	class ASTArgDecl : public ASTStatement {
	public:
		ASTArgDecl(std::u8string_view identifier,
			std::shared_ptr<ASTType> type) :
			identifier{ identifier },
			type{ type } {};
		ASTArgDecl(std::u8string_view identifier,
			std::shared_ptr<ASTType> type,
			std::shared_ptr<ASTExpression> defaultValue) :
			identifier{ identifier },
			type{ type },
			defaultValue{ defaultValue }{};
	public:
		void Accept(ASTVisitor* visitor) const {
			visitor->Visit_ArgDecl(this);
		}
	public:
		std::u8string_view GetIdentifier() const { return identifier; }
		std::shared_ptr<ASTType> GetType() const { return type; }
		std::shared_ptr<ASTExpression> GetDefaultValue() const 
		{ 
			if (defaultValue.has_value())
				return defaultValue.value();
			return nullptr;
		}
		bool HasDefaultValue() const { return defaultValue.has_value(); }
	private:
		std::u8string_view identifier;
		std::shared_ptr<ASTType> type;
		std::optional<std::shared_ptr<ASTExpression>> defaultValue;
	};
}