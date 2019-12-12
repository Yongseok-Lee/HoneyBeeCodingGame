#include "Statement.h"

#include "Token.h"
#include "Error.h"

#include "StatementImpl.h"
#include <iostream>

namespace script
{
	CStatement::~CStatement()
	{
	}

	void CStatement::Execute(SymbolTable &context)
	{
	}

	void CStatement::Expect(TokenID Expected, const Token &NowToken)
	{
/*
#ifdef _DEBUG
		std::cout << "Statement Token : " << NowToken._name << std::endl;
#endif*/

		if (Expected != NowToken._id)
			throw Error(std::string("StatementError : " + NowToken._name));
	}

	CStatement *CStatement::Create(TokenIter &itor)
	{
		switch (itor->_id)
		{
		case TokenID::Stmt_If:
			return new CIfStatement(itor);
		case TokenID::Stmt_Loop:
			return new CWhileStatement(itor);
		case TokenID::Stmt_For:
			return new CForStatement(itor);
		case TokenID::OpenMedium:
			return new CCompoundStatement(itor);
		case TokenID::Type_Var:
			return new CVarStatement(itor);
		case TokenID::Identifier:
			return new CAssignmentStatement(itor);
		case TokenID::Func_Call:
			return new CFunctionCallStatement(itor);
		default:
			throw(Error(std::string("Unexpected Statement : ") + itor->_name));
		}
	}

}
