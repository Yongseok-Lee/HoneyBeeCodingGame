#include "Script.h"
#include <cctype>
#include <iostream>
#include "Error.h"

#define CASE_WHITE_SPACE	case ' ': case '\r': case '\t': case '\n':

#define CASE_LOWER_ALPHA	case 'a': case 'b': case 'c': case 'd': case 'e': case 'f': case 'g': case 'h': case 'i': \
							case 'j': case 'k': case 'l': case 'm': case 'n': case 'o': case 'p': case 'q': case 'r': \
							case 's': case 't': case 'u': case 'v': case 'w': case 'x': case 'y': case 'z': 

#define CASE_UPPER_ALPHA	case 'A': case 'B': case 'C': case 'D': case 'E': case 'F': case 'G': case 'H': case 'I': \
							case 'J': case 'K': case 'L': case 'M': case 'N': case 'O': case 'P': case 'Q': case 'R': \
							case 'S': case 'T': case 'U': case 'V': case 'W': case 'X': case 'Y': case 'Z': 

#define CASE_NUMBER			case '0': case '1': case '2': case '3': case '4': case '5': case '6': case '7': case '8': case '9':

#define CASE_PUNCT			case '+': case '-': case '*': case '%': case '=': case '<': case '>': case '!': case ':': \
							case '?': case '&': case '|': case '^':  case '.': case ',': case ';':

namespace script
{
	std::vector<Keyword> _keywordList = {
		   { TokenID::Type_Var, "var" },
		   { TokenID::Stmt_If, "if" },
		   { TokenID::Stmt_Else, "else" },
		   { TokenID::Stmt_Loop, "loop" },
		   { TokenID::Stmt_For, "for" },
		   { TokenID::Boolean_Value, "true" },
		   { TokenID::Boolean_Value, "false" },

		   { TokenID::Arithmetic_Add, "+" },
		   { TokenID::Arithmetic_Subtract, "-" },
		   { TokenID::Arithmetic_Multiply, "*" },
		   { TokenID::Arithmetic_Divide, "/" },
		   { TokenID::Arithmetic_Modulo, "%" },
		   { TokenID::Assignment, "=" },
		   { TokenID::Logical_And, "&&" },
		   { TokenID::Logical_Or, "||" },
		   { TokenID::Logical_Not, "!" },
		   { TokenID::Relational_Equal, "==" },
		   { TokenID::Relational_NotEqual, "!=" },
		   { TokenID::Relational_Greater, ">" },
		   { TokenID::Relational_Less, "<" },
		   { TokenID::Relational_GreaterEqual, ">=" },
		   { TokenID::Relational_LessEqual, "<=" },
		   { TokenID::OpenMedium, "{" },
		   { TokenID::OpenSmall, "(" },
		   { TokenID::CloseSmall, ")" },
		   { TokenID::CloseMedium, "}" },

		   //{ TokenID::Colon, ":" },
		   { TokenID::Comma, "," },
		   { TokenID::Semicolon, ";" },


		   { TokenID::Func_Call, "Go" },
		   { TokenID::Func_Call, "TurnLeft" },
		   { TokenID::Func_Call, "TurnRight" },
	};

	int _sourceLine = 0;

	Script::Script(const char* source) : _sourceCode(source)
	{
	}


	Script::~Script()
	{
	}


	void Script::LexStart()
	{
		for (auto iter = _sourceCode.begin(); iter != _sourceCode.end(); ++iter)
		{
			switch (*iter)
			{
			CASE_WHITE_SPACE
				if (*iter == '\n') { ++_sourceLine; }
				continue;
			case '(':
				_tokenList.emplace_back(TokenID::OpenSmall, _sourceLine, "(");
				break;
			case ')':
				_tokenList.emplace_back(TokenID::CloseSmall, _sourceLine, ")");
				break;
			case '{':
				_tokenList.emplace_back(TokenID::OpenMedium, _sourceLine, "{");
				break;
			case '}':
				_tokenList.emplace_back(TokenID::CloseMedium, _sourceLine, "}");
				break;
			case '/':
				if (*next(iter) == '/') { SkipToLineAnnotation(iter); }		/// slash-slash line annotation "//"
				else if (*next(iter) == '*') { SkipToBlockAnnotation(iter); }		/// slash-star block annotation "/*"
				else { LexPunct(iter); }		/// operators
				break;

			CASE_UPPER_ALPHA
			CASE_LOWER_ALPHA
			case '_':
				LexCharacter(iter);
				break;

				CASE_NUMBER
					LexNumber(iter);
				break;

				CASE_PUNCT
					LexPunct(iter);
				break;

			default:
				throw LexicalError(_sourceLine, "Unexcepted Token : " + *iter);
			}
		}
		

		_tokenList.emplace_back(TokenID::Eof, _sourceLine + 1);
	}

	void Script::LexCharacter(std::string::iterator& iter)
	{
		/// Identifier ::= [a-zA-Z_][0-9a-zA-z_]*

		std::string word;
		for (; isalnum(*iter) || *iter == '_'; word += *iter, ++iter) {}
		--iter;

		/// 키워드인지 비교.
		for (auto kw : _keywordList)
		{
			if (word == kw._word)
			{
				_tokenList.emplace_back(kw._id, _sourceLine, word);
				return;
			}
		}

		/// identifier로 추가.
		_tokenList.emplace_back(TokenID::Identifier, _sourceLine, word);
	}

	void Script::LexNumber(std::string::iterator& iter)
	{
		/// Integer	::= [0-9][0-9]*
		/// Hexa	::= 0x[0-9a-fA-F][0-9a-fA-F]*
		/// Octal	::= 0o[0-7][0-7]*
		/// Binary	::= 0b[01][01]*
		/// Double	::= [0-9][0-9]*.[0-9][0-9]*

		std::string word;

		/// 0xAB, 0b01010, 0o17등 2,8,16진수 처리
		word = *iter;

		if (*iter == 0 && iter != _sourceCode.end())
			word += *next(iter);

		/// 특이 케이스 처리.
		if (word == "0x" || word == "0o" || word == "0b")
		{
			int base = 10;		/// 2 - binary, 8 - octa, 10 - decimal, 16 - hexa
			switch (*++iter)
			{
			case 'b': base = 2; break;
			case 'o': base = 8; break;
			case 'x': base = 16; break;
			}

			auto start = ++iter;

			for (; iter != _sourceCode.end(); ++iter)
			{
				if (!std::isxdigit(*iter))
					break;		/// xdigit이 아닐때 까지 iter를 증가한다.
			}

			std::string number(start, prev(iter));

			_tokenList.emplace_back(TokenID::Literal_Integer, _sourceLine, stoi(number, nullptr, base), number);
		}
		else
		{
			auto start = iter;
			for (; iter != _sourceCode.end(); ++iter)
			{
				if (!std::isdigit(*iter))
					break;
			}

			std::string number(start, iter--);

			_tokenList.push_back(Token(TokenID::Literal_Integer, _sourceLine, stoi(number, nullptr), number));

		}
	}

	void Script::LexPunct(std::string::iterator& iter)
	{
		auto start = iter;
		for (; iter != _sourceCode.end() && std::ispunct(*iter) && *iter != '\"' && *iter != '\'' && *iter != '('&& *iter != ')'; ++iter) {}
		std::string operatorString(start, iter--);

		for (auto it : _keywordList)
		{
			if (operatorString == it._word)
			{
				_tokenList.emplace_back(it._id, _sourceLine, it._word);
				return;
			}
		}
	}

	void Script::SkipToLineAnnotation(std::string::iterator& iter)
	{
		for (; *iter != '\n'; ++iter);		/// 개행문자를 만날 때 까지 iter증가
	}

	void Script::SkipToBlockAnnotation(std::string::iterator& iter)
	{
		for (; *prev(iter) != '*' && *iter != '/'; ++iter);		/// */까지 iter증가
	}

	void Script::Exec()
	{
		// lex
		LexStart();
		//

		//parse tree gen
		TokenIter tokenitor = _tokenList.begin();
		while (tokenitor->_id != TokenID::Eof)
			_stmtList.push_back(StatementPtr(CStatement::Create(tokenitor)));
		//

		// exec
		StatementList::iterator iter;
		for (iter = _stmtList.begin(); iter != _stmtList.end(); iter++)		//실행문들을 수행!
		{
			iter->get()->Execute(_symbolTable);
		}
		//
	}
}
