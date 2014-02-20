#include "Condition.h"
#include "Database.h"
#include "Relation.h"
#include "stdafx.h"
#include "targetver.h"
#include <vector>


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