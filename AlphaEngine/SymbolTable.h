#pragma once
#include <string>
//#include <functional>
#include <unordered_map>
#include "PlayerAction.h"

namespace script
{
	struct Symbol
	{
		std::string _name;
		int _data;

		Symbol(std::string name, int data) : _name(name), _data(data) {}
		~Symbol() = default;
	};

	class SymbolTable
	{
	public:
		int GetValue(std::string variable);			//값을 받아온다.
		int SetValue(std::string variable, int value);			//값을 저장하고 반환한다.
		bool CheckValue(std::string variable);			//변수가 있는지 확인한다.
		void DeleteValue(std::string variable);

		void AddValue(std::string variable);			//변수를 등록한다.

		void AddAction(PlayerAction action)
		{
			_actions.push_back(action);
		}

		std::vector<PlayerAction> GetActions()
		{
			return _actions;
		}
	private:
		typedef std::unordered_map<std::string, Symbol*> VarTable;

		VarTable _varTable;
		std::vector<PlayerAction> _actions;
	};
}
