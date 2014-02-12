#pragma once
#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include "Relation.h"
#include "Database.h"

using namespace std;

class Relation;
class Parser
{
public:
	string input;
	Database* DB;

	//helper functions
	bool ParseQuery(string);
	string ParseRelation();

	bool ParseExpression();

	//operation parsing
	bool ParseSelection();

	bool ParseProjection();

	bool ParseRenaming();

	bool ParseSetUnion();

	bool ParseSetDifference();

	bool ParseCrossProduct();

	bool ParseNaturalJoin();

	bool ParseCreateTable();
	bool ParseDropTable();
	bool ParseInsertInto(string);
	bool ParseUpdate();
	bool ParseDeleteFrom();
	bool ParseShow();

	Parser(Database* DB);
	~Parser();
};

