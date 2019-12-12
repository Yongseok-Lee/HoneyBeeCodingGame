
#include "StatementImpl.h"

#include "Error.h"

namespace script
{
	// CIfStatement

	CIfStatement::CIfStatement(TokenIter &iter) : ifStatement(nullptr), elseStatement(nullptr)
	{
		iter->Expect(TokenID::Stmt_If);
		++iter;
		m_expression = CExpression::Parse(iter);
		ifStatement = StatementPtr(CStatement::Create(iter));

		if (iter->Is(TokenID::Stmt_Else))
		{
			++iter;
			elseStatement = StatementPtr(CStatement::Create(iter));
		}
	}

	void CIfStatement::Execute(SymbolTable &context)
	{
		if (m_expression->Evaluate(context))
			ifStatement->Execute(context);
		else if (elseStatement != nullptr)
			elseStatement->Execute(context);
	}

	// CWhileStatement

	CWhileStatement::CWhileStatement(TokenIter &iter)
	{
		iter->Expect(TokenID::Stmt_Loop);
		++iter;
		m_expression = CExpression::Parse(iter);
		m_statement = StatementPtr(CStatement::Create(iter));
	}

	void CWhileStatement::Execute(SymbolTable &context)
	{
		int i = 0;
		while (i < m_expression->Evaluate(context))
		{
			m_statement->Execute(context);
			i++;
		}
	}

	// CForStatement

	CForStatement::CForStatement(TokenIter &iter)
	{
		Expect(TokenID::Stmt_For, *iter++);
		m_init = StatementPtr(new CAssignmentStatement(iter));			//초기식
		Expect(TokenID::Semicolon, *iter++);
		m_condition = CExpression::Parse(iter);					//조건식
		Expect(TokenID::Semicolon, *iter++);
		m_increment = StatementPtr(new CAssignmentStatement(iter));			//증감문

		m_statement = StatementPtr(CStatement::Create(iter));
	}

	void CForStatement::Execute(SymbolTable &context)
	{
		for (m_init->Execute(context); m_condition->Evaluate(context); m_increment->Execute(context))
			m_statement->Execute(context);
	}

	// CCompoundStatement

	CCompoundStatement::CCompoundStatement(TokenIter &iter)
	{
		Expect(TokenID::OpenMedium, *iter++);
		while (iter->_id != TokenID::CloseMedium)
			m_statements.push_back(StatementPtr(CStatement::Create(iter)));
		Expect(TokenID::CloseMedium, *iter++);
	}

	void CCompoundStatement::Execute(SymbolTable &context)
	{
		for (StatementList::iterator iter = m_statements.begin(); iter != m_statements.end(); iter++)
		{
			iter->get()->Execute(context);
		}
	}

	// CVarStatement

	CVarStatement::CVarStatement(TokenIter &it)
	{
		m_bAssign = false;
		Expect(TokenID::Type_Var, *it++);
		Expect(TokenID::Identifier, *it);
		m_variable = it->_name;
		if ((++it)->_id == TokenID::Assignment)
		{
			it++;
			m_expression = CExpression::Parse(it);
			m_bAssign = true;
		}
	}

	void CVarStatement::Execute(SymbolTable &context)
	{
		context.AddValue(m_variable);
		if (m_bAssign)
			context.SetValue(m_variable, m_expression->Evaluate(context));
	}

	// CAssignmentStatement

	CAssignmentStatement::CAssignmentStatement(TokenIter &iter)
	{
		Expect(TokenID::Identifier, *iter);
		m_variable = iter->_name;
		++iter;
		Expect(TokenID::Assignment, *iter++);
		m_expression = CExpression::Parse(iter);
	}

	void CAssignmentStatement::Execute(SymbolTable &context)
	{
		context.SetValue(m_variable, m_expression->Evaluate(context));
	}

}