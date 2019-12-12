#pragma once

#include <vector>
#include <string>

namespace script
{
	enum class TokenID
	{
		Eof = 0,

		Equal = '=',
		Cross = '+',
		Hyphen = '-',
		Asterisk = '*',
		ForwardSlash = '/',
		Percent = '%',
		Ampersand = '&',
		Bar = '|',
		Less = '<',
		Great = '>',
		OpenSmall = '(',
		CloseSmall = ')',
		OpenMedium = '{',
		CloseMedium = '}',
		Comma = ',',
		Semicolon = ';',

		Type_Var = 256,

		Stmt_If,
		Stmt_Else,
		Stmt_Loop,
		Stmt_For,

		Func_Call,

		Operator,

		Identifier,	//변수나 클래스나 함수등 사용자 지정 이름

		Literal_Integer, //정수

		Assignment,

		Arithmetic_Add,
		Arithmetic_Subtract,
		Arithmetic_Multiply,
		Arithmetic_Divide,
		Arithmetic_Modulo,
		Arithmetic_Power,

		Relational_Equal,
		Relational_NotEqual,
		Relational_Greater,
		Relational_Less,
		Relational_GreaterEqual,
		Relational_LessEqual,

		Logical_And,
		Logical_Or,
		Logical_Not,

		Boolean_Value,		/// true, false

		Object_Go,
		Object_TurnLeft,
		Object_TurnRight,
	};

	struct Token
	{
		TokenID _id;
		int _line;
		int _data;
		std::string _name;

		enum class TokenTypeTag
		{
			_notuse,
			_int
		} _type;

		explicit Token(TokenID id, int line, int number, std::string name = "") :
			_id(id),
			_line(line),
			_data(number),
			_name(name),
			_type(TokenTypeTag::_int)
		{ }

		Token(TokenID id, int line, std::string name = "") :
			_id(id),
			_line(line),
			_data(0),
			_name(name),
			_type(TokenTypeTag::_notuse)
		{ }

		void Expect(const TokenID id) const;
		bool Is(const TokenID id) const;
	};

	using TokenList = std::vector<Token>;
	using TokenIter = TokenList::iterator;
	using TokenPattern = std::vector<TokenID>;
	bool IsSamePattern(TokenIter iter, TokenPattern pattern);

	std::ostream& operator<<(std::ostream& os, Token& token);
	std::ostream& operator<<(std::ostream& os, const Token& token);

	struct Keyword
	{
		TokenID _id;
		std::string _word;

		Keyword(TokenID id, std::string word) : _id(id), _word(word) {}
	};
}