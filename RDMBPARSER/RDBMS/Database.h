/*
Database.h
Created by Lauren Fuller on 2/1/14.
*/

#pragma once

#include "Condition.h"

class Database {

	// Variables associated with the Database
	vector<_Relation> allRelations;

public:

	// seven necessary operations 
	_Relation* getRelation(string Name);
	_Relation* selection(Condition& c, _Relation r);
	_Relation* projection(vector<string> attributes, _Relation r);
	_Relation* renaming(vector<string> renames, _Relation r);
	_Relation* setUnion(_Relation a, _Relation b);
	_Relation* setDifference(_Relation& a, _Relation& b);
	_Relation* crossProduct(_Relation& a, _Relation& b);
	_Relation* naturalJoin(_Relation& a, _Relation& b);

	// other operations 
	void createTable(string name, vector<_Column> attributes);
	void dropTable(_Relation r);
	void insertInto(string name, vector<string> literals);
	void insertInto(string name, _Relation& r);
	void update(string rName, vector<string> attributes, vector<_Data> newValues, Condition& c);
	void deleteFrom(string name, Condition& c);
	void show(string name);
	void addRelation(_Relation& r) { allRelations.push_back(r); } // end function 




}; // end class Database
