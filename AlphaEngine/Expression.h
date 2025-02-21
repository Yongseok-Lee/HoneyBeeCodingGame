#pragma once

#include "Token.h"
#include "SymbolTable.h"

namespace script
{
	class CExpression		//표현식의 가장 기본이 되는 최상위 부모 클래스
	{
	public:
		typedef std::shared_ptr<CExpression> ExprPtr;		//shared_ptr로 한번 랩핑해준다.
		typedef std::list<ExprPtr> Exprs;			//리스트를 정의
		typedef std::shared_ptr<Exprs> ExprsPtr;		//리스트도 shared_ptr로 한번 랩핑해준다.
		static ExprPtr Parse(TokenIter &it);			//표현식을 분석하고 파스트리를 만든다.
		virtual int Evaluate(SymbolTable &context) = 0;		//분석된 파스트리를 바탕으로 값을 계산해간다.
		virtual ~CExpression() = default;
	protected:
		static void Expect(TokenID Expected, const Token &NowToken);		//문법에 맞는지 확인한다.
	};

}
