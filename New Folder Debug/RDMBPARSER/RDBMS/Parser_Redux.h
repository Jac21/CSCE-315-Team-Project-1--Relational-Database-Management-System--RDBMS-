#pragma once

#include "Condition.h"
#include "Database.h"
#include "Relation.h"
#include "Commands.h"
#include "stdafx.h"
#include "targetver.h"
#include <vector>
#include <string>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <fstream>

using namespace std;

string ltrim(string &s);
string rtrim(string &s);
string trim(string &s);
char GrabChar(string&);
string GrabToken(string&);
vector<string>GrabAllTokens(string);
vector<_Data>GrabDataSets(string);
string removeParentheses(string);

class Parser
{
public:
	Database* DB;
	vector<Command*> Commands;

	string CurInput;

	Parser(Database* DB)
	{
		this->DB = DB;
		CurInput = "";
		Commands.push_back(new Insert());
		Commands.push_back(new Create());
		Commands.push_back(new Show());
		Commands.push_back(new Open());
		Commands.push_back(new Write());
		Commands.push_back(new Exiter());
	}

	bool Execute(string Input)
	{
		CurInput = Input;
		string Token = "";
		while (CurInput.length() != 0)
		{
			Token += GrabChar(CurInput);

			if (IsCommand(Token))
				ExecuteCommand(Token);
			if (IsQuery(Token))
				ExecuteQuery(Token);
		}
		return false;
	}

	void ExecuteCommand(string Token)
	{
		Command* C = GetCommand(Token);
		C->Execute(DB, CurInput);
	}

	void ExecuteQuery(string Token)
	{
		string QueryResultName = Token;
		QueryResultName.pop_back(); // Removes -
		QueryResultName.pop_back(); // Removes <
		trim(QueryResultName);
		//You have your query result name, and the rest of 'input' is your string.
	}

	bool IsCommand(string Token)
	{
		for (int i = 0; i < Commands.size(); i++)
		{
			if (Token == Commands[i]->Identifer)
				return true;
		}
		return false;
	}

	bool IsQuery(string Token)
	{
		if (Token.size() > 2)
		{
			if (Token[Token.size() - 2] == '<' && Token[Token.size() - 1] == '-')
				return true;
		}
		return false;
		// NOTE: Still needs implimentation 
	}

	Command* GetCommand(string Token)
	{
		Command* NewCommand;
		for (int i = 0; i < Commands.size(); i++)
		{
			if (Token == Commands[i]->Identifer)
			{
				NewCommand = Commands[i];
				return NewCommand;
			}
		}
		return NULL;
	}
};

class Query {
public:
	string Identifer;
	_Relation* relation;
	bool virtual IsQuery(string input) {
		if (input == Identifer)
			return true;
	} // end IsQuery(string) 

	bool virtual Execute(Database* DB, string Input) {
		return false;
	} // end Execute(Database*, string) 
}; // end class Query 

class RelationName : public Query {

public:
	RelationName();

	// sees if the input is a relation name (must start with a-z or A-Z)
	bool Execute(Database* DB, string input);
	bool isArrow(string input);
	bool digit(char d);
	bool alpha(char a);

}; // end class Query 

class Selection : public Query {

public:
	vector<Query*> querys;

	Selection();
	bool Execute(Database* DB, string input);

}; // end Selection 

class Projection : public Query {

public:
	vector<Query*> querys;

	Projection();
	bool Execute(Database* DB, string input);

}; // end Projection 

class Renaming : public Query {

public:
	vector<Query*> querys;

	Renaming();
	bool Execute(Database* DB, string input);

}; // end Renaming 

class SetUnion : public Query {

public:
	vector<Query*> querys;

	SetUnion();
	bool Execute(Database* DB, string input);
};

class SetDifference : public Query {

public:
	vector<Query*> querys;

	SetDifference();
	bool Execute(Database* DB, string input);

}; // end SetDifference 

class CrossProduct : public Query {

public:
	vector<Query*> querys;

	CrossProduct();
	bool Execute(Database* DB, string input);

}; // end CrossProduct

class NaturalJoin : public Query {

public:
	vector<Query*> querys;

	NaturalJoin();
	bool Execute(Database* DB, string input);

}; // end NaturalJoin 