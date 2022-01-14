#pragma once
#include <utility>
#include <string_view>
#include <vector>
#include <assert.h>

namespace Tokenizer 
{
	template<typename cT, typename dT>
	class BaseTokenizer
	{
	public:
		using ValueType = cT;
		using DataType = dT;
		using StringType = std::basic_string_view<ValueType>;
	public:
		class Consumer {
		public:
			using TokenType = std::pair<StringType, DataType>;
		public:
			Consumer() = default;
			Consumer(const Consumer& consumer) : tokens{ consumer.tokens } {};
			Consumer(std::vector<TokenType> tokens) : tokens{ tokens } {};
			Consumer& operator=(const Consumer& consumer) { tokens = consumer.tokens; };
			~Consumer() { tokens.clear(); };
		public:
			const TokenType& Peek() const
			{
				assert(("index out of bound", tokens.size() > 0));
				return tokens[0];
			}

			const TokenType& Peek(int n) const
			{
				assert(("index out of bound", n < tokens.size()));
				return tokens[n];
			};

			TokenType Consume()
			{
				if (tokens.size() == 0)
					return TokenType{};
				TokenType token = tokens[0];
				tokens.erase(tokens.begin());
				return token;
			};

			TokenType Consume(int n)
			{
				if (tokens.size() <= n)
					return TokenType{};
				TokenType token = tokens[n];
				tokens.erase(tokens.begin() + n);
				return token;
			};

			size_t GetSize() const 
			{
				return tokens.size();
			}
		private:
			std::vector<TokenType> tokens;
		};
	public:
		virtual std::pair<StringType, DataType> Tokenize(const StringType&) = 0;

		std::vector<std::pair<StringType, DataType>> TokenizeAll(StringType str)
		{
			std::vector<std::pair<StringType, DataType>> tokens{};

			for (int i = 0; i < str.size();) {
				StringType nstr{ str.data() + i, str.size() - i };
				std::pair<StringType, DataType> r = Tokenize(nstr);

				assert(("tokenizer must return token with value greater than 0", r.first.length() > 0));
				assert(("tokenizer return value outside bound of string", r.first.length() + i <= str.length()));

				tokens.push_back(r);
				i += r.first.length();
			}

			return tokens;
		}

		Consumer TokenizeAndGetConsumer(StringType str)
		{
			return Consumer{ TokenizeAll(str) };
		}

		StringType TokenizeSymbol(const StringType& str)
		{
			return str.substr(0, 1);
		}

		StringType TokenizeWord(const StringType& str, int size)
		{
			assert(("word length is greater than str length", str.length() >= size));
			return str.substr(0, size);
		}

		StringType TokenizeEnclosure(const StringType& str, ValueType endv, bool takeLast = true)
		{
			assert(("length must be greater than 1", str.length() > 1));
			for (int i = 1; i < str.length(); i++)
				if (str[i] == endv)
					return str.substr(0, (size_t)(takeLast ? i + 1 : i));
			return StringType{};
		}

		StringType TokenizeSequence(const StringType& str, std::vector<ValueType> values)
		{
			for (int i = 0; i < str.length(); i++)
				if (std::find(values.begin(), values.end(), str[i]) == values.end())
					return str.substr(0, i);
			return StringType{};
		}

		StringType TryTokenizeSymbol(const StringType& str, std::vector<ValueType> symbols)
		{
			for (const ValueType& symbol : symbols)
				if (str[0] == symbol)
					return str.substr(0, 1);
			return StringType{};
		}

		StringType TryTokenizeWord(const StringType& str, std::vector<StringType> words)
		{
			for (const StringType& word : words)
				if (str.length() >= word.length() && str.substr(0, word.length()) == word)
					return str.substr(0, word.length());
			return StringType{};
		}

		StringType TryTokenizeEnclosure(const StringType& str, std::vector<ValueType> endvs, bool takeLast = true)
		{
			assert(("length must be greater than 1", str.length() > 1));
			for (int i = 1; i < str.length(); i++)
				if (std::find(endvs.begin(), endvs.end(), str[i]) != endvs.end())
					return str.substr(0, (size_t)(takeLast ? i + 1 : i));
			return StringType{};
		}
	};

	template<typename dT>
	using U8BaseTokenizer = BaseTokenizer<char8_t, dT>;

	template<typename dT>
	using U16BaseTokenizer = BaseTokenizer<char16_t, dT>;

	template<typename dT>
	using U32BaseTokenizer = BaseTokenizer<char32_t, dT>;

}