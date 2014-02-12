#include "Parser.h"
#include <iostream>

using namespace std;
vector<string> Tokens;
vector<string> split(string);

//String tokenizer

char grabChar(string& str)
{
	if (str.size() > 0)
	{
		string newStr;
		for (int i = 1; i < str.size(); i++)
			newStr.push_back(str[i]);
		return str[0];
	}
	return ' ';
}

string getToken(string& str)
{
	int size = str.size();
	string temp = "";
	bool quotations = false;
	for (int i = 0; i < size; i++)
	{
		if (str[i] == '\"')
		{
			if (quotations == true)
				quotations = false;
			else
				quotations = true;
		}
		if (str[i] != ' ' || quotations)
			temp += str[i];
		else
		{
			return (temp);
			temp = "";
		}
	}

	return temp;

}
vector<string> split(string str)
{
	vector<string> result;
	int size = str.size();
	string temp = "";
	bool quotations = false;
	for (int i = 0; i < size; i++)
	{
		if (str[i] == '\"')
		{
			if (quotations == true)
				quotations = false;
			else
				quotations = true;
		}
		if (str[i] != ' ' || quotations)
			temp += str[i];
		else
		{
			result.push_back(temp);
			temp = "";
		}
	}
	if (temp != "")
		result.push_back(temp);

	return result;
}

Parser::Parser(Database* DB)
{
	this->DB = DB;
}


Parser::~Parser()
{
}

bool Parser::ParseQuery(string Input) 
{
	vector<string> Buffer;
	string Temp = "";
	while (true)
	{
		Temp += getToken(Input) + " ";
		if (Temp == "INSERTINTO")
		{
			return ParseInsertInto(Input);
		}
	}
	return false;

}

bool Parser::ParseInsertInto(string Input)
{
	string Temp = ""; //LOOKING FOR TABLE NO SPACES IN NAME

	Temp += getToken(Input);

	if (DB->getTable(Temp) != NULL)
	{
		vector<string> arguments = ParseArgumentString(Input);
	}
	return false;
}

vector<string> Parser::ParseArgumentString(string Input)
{
	string Temp = "";
	vector<string> arguments;

	Temp += getToken(Input);
	{
	
	}
	if (DB->getTable(Temp) != NULL)
		vector<string> arguments = ParseArgumentString(Input);

	return arguments;
}

bool Parser::ParseExpression()
{
	return ParseSelection() ||
		ParseProjection() ||
		ParseRenaming() ||
		ParseSetUnion() ||
		ParseSetDifference() ||
		ParseCrossProduct() ||
		ParseNaturalJoin();
}

bool Parser::ParseSelection()
{
	//Tokenize command-line input
}