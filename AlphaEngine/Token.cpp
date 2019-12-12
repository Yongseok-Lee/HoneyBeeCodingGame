#include "Token.h"
#include "Error.h"

namespace script
{
	void Token::Expect(const TokenID id) const
	{
		if (this->_id != id)
			throw ParsingError(*this, "Unexpected Token");
	}

	bool Token::Is(const TokenID id) const
	{
		return this->_id == id;
	}

	bool IsSamePattern(TokenIter iter, TokenPattern pattern)
	{
		for (auto id : pattern)
		{
			if (iter->Is(id))
				return false;
			iter = std::next(iter);
		}
		return true;
	}

	std::ostream& operator<<(std::ostream& os, Token& token)
	{
		os << token._name << ":" << std::to_string(token._line) << "> Token id:" << static_cast<int>(token._id) << ", ";
		switch (token._type)
		{
		case Token::TokenTypeTag::_notuse:
			os << token._name << std::endl;
			break;
		case Token::TokenTypeTag::_int:
			os << token._data << std::endl;
			break;
		}
		return os;
	}

	std::ostream& operator<<(std::ostream& os, const Token& token)
	{
		os << token._name << ":" << std::to_string(token._line) << "> Token id:" << static_cast<int>(token._id) << ", ";
		switch (token._type)
		{
		case Token::TokenTypeTag::_notuse:
			os << token._name << std::endl;
			break;
		case Token::TokenTypeTag::_int:
			os << token._data << std::endl;
			break;
		}
		return os;
	}
}
