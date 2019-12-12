#include "Expression.h"
#include "Error.h"
#include "ExpressionImpl.h"
#include <iostream>

namespace script
{
	void CExpression::Expect(TokenID Expected, const Token &NowToken)
	{
#ifdef _DEBUG
		std::cout << "Expression Token : " << NowToken._name << std::endl;
#endif

		if (Expected != NowToken._id)
			throw (Error(std::string("SyntaxError : ") + NowToken._name));
	}

	CExpression::ExprPtr CExpression::Parse(TokenIter &it)
	{
		//연산자 우선순위에 의해서 제일 마지막에 하는 or연산부터 아래로 파스트리 작성.
		return ExprPtr(CComparisonExp::Parse(it));
	}

	// OrExp
}
