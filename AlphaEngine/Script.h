#pragma once
#include "Token.h"
#include "Statement.h"

namespace script
{
	struct Script
	{
		Script(const char* source);
		~Script();

		void LexStart();
		void LexCharacter(std::string::iterator& iter);
		void LexNumber(std::string::iterator& iter);
		void LexPunct(std::string::iterator& iter);
		void SkipToLineAnnotation(std::string::iterator& iter);
		void SkipToBlockAnnotation(std::string::iterator& iter);

		void Exec();

		std::string _sourceCode;
		TokenList _tokenList;
		StatementList _stmtList;

		SymbolTable _symbolTable;
	};
}
