#include "SymbolTable.h"
#include "Error.h"

namespace script
{
	int SymbolTable::GetValue(std::string variable)
	{
		VarTable::iterator iter = _varTable.find(variable);			//변수 탐색
		if (iter == _varTable.end())				//탐색실패.
			throw Error(std::string("Undefined Variable : ") + variable);
		
		return iter->second->_data;
	}

	int SymbolTable::SetValue(std::string variable, int value)
	{
		if (_varTable.find(variable) == _varTable.end())			//변수 탐색
			throw Error(std::string("Undeclared Variable : ") + variable);		//탐색 실패.
		
		return _varTable[variable]->_data = value;
	}

	void SymbolTable::DeleteValue(std::string variable)
	{
		VarTable::iterator iter = _varTable.find(variable);
		if (iter == _varTable.end())
			throw Error(std::string("Undeclared Variable ") + variable);
		
		_varTable.erase(iter);
	}

	void SymbolTable::AddValue(std::string variable)
	{
		VarTable::iterator itor;
		int i = _varTable.size();
		if (i != 0)		//변수 테이블에 아무런 값이 없으면 바로 넣음, 있으면 중복 검사.
		{
			if (CheckValue(variable))			//탐색 성공 -> 변수가 이미 등록되어있다.
				throw Error(std::string("Already Registered Variable : ") + variable);
		}

		_varTable[variable] = new Symbol(variable, 0); //탐색 실패 -> 변수를 등록할 수 있다.
	}

	bool SymbolTable::CheckValue(std::string variable)
	{
		VarTable::iterator itor = _varTable.find(variable);			//변수 탐색
		return itor != _varTable.end();
	}
}
