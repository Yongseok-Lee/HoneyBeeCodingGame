#pragma once

#include "Expression.h"

namespace script
{
	class CMultExp : public CExpression			//곱셈처리
	{
	public:
		static CExpression::ExprPtr Parse(TokenIter &it);
		int Evaluate(SymbolTable &context) override;
	private:
		Exprs m_exprs;			//표현식들
		std::list<TokenID> m_ops;		//연산자, *, /, %를 이 클래스에서 한번에 처리한다.
	};

	class CAddExp : public CExpression			//덧셈처리
	{
	public:
		static CExpression::ExprPtr Parse(TokenIter &it);
		int Evaluate(SymbolTable &context) override;
	private:
		Exprs m_exprs;			//표현식들
		std::list<bool> m_ops;			//참 : 덧셈, 거짓 : 뺄셈
	};

	class CComparisonExp : public CExpression		//비교연산처리
	{
	public:
		CComparisonExp(CExpression::ExprPtr lhs, CExpression::ExprPtr rhs, TokenID tok);
		static CExpression::ExprPtr Parse(TokenIter &it);
		int Evaluate(SymbolTable &context) override;
	private:
		CExpression::ExprPtr m_lhs;			//좌항
		CExpression::ExprPtr m_rhs;			//우항
		TokenID m_tok;			//비교연산자
		int result;
	};

	class CAtom : public CExpression			//단항연산자 처리
	{
	public:
		static CExpression::ExprPtr Parse(TokenIter &it);
	};

	class CNumber : public CAtom			//일반 숫자
	{
	public:
		CNumber(int number) : m_number(number) {}
		int Evaluate(SymbolTable &context) { return m_number; }
	private:
		int m_number;
	};

	class CNot : public CAtom				//논리연산 not
	{
	public:
		CNot(CExpression::ExprPtr expr) : m_expr(expr) {}
		int Evaluate(SymbolTable &context) { return !m_expr->Evaluate(context); }
	private:
		CExpression::ExprPtr m_expr;
	};

	class CIdentifier : public CAtom			//변수호출
	{
	public:
		CIdentifier(std::string identifier) : m_identifier(identifier) {}
		int Evaluate(SymbolTable &context) { return context.GetValue(m_identifier); }
	private:
		std::string m_identifier;
	};

	class CCompoundAtom : public CAtom			//괄호처리
	{
	public:
		CCompoundAtom(CExpression::ExprPtr expr) : m_CExpression(expr) {}
		int Evaluate(SymbolTable &context) { return m_CExpression->Evaluate(context); }
	private:
		CExpression::ExprPtr m_CExpression;
	};
}