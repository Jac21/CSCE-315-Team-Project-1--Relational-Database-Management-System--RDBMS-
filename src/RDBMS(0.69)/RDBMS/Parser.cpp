#include "Parser.h"
#include <iostream>

using namespace std;

Parser::Parser(string input)
{
	this->input = input;
}


Parser::~Parser()
{
}

bool Parser::ParseQuery() 
{

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