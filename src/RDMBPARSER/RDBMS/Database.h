/*
Database.h
Created by Lauren Fuller on 2/1/14.
*/

#pragma once

#include "Condition.h"
#include "std_lib_facilities.h"

class Database {

	// Variables associated with the Database
	Vector<_Relation> allRelations;

public:

	// seven necessary operations 
	_Relation* getRelation(string Name);
	_Relation selection(Condition& c, _Relation r);
	_Relation projection(vector<String> attributes, _Relation r);
	void renaming(vector<String> renames, _Relation& r);
	_Relation setUnion(_Relation a, _Relation b);
	_Relation setDifference(_Relation& a, _Relation& b);
	_Relation* crossProduct(_Relation& a, _Relation& b);
	_Relation* naturalJoin(_Relation& a, _Relation& b);

	// other operations 
	void createTable(String name, Vector<_Column> attributes);
	void dropTable(_Relation r);
	void insertInto(String name, Vector<String> literals);
	void insertInto(String name, _Relation& r);
	void update(String rName, vector<String> attributes, vector<_Data> newValues, Condition& c);
	void deleteFrom(String name, Condition& c);
	void show(String name);
	void addRelation(_Relation& r) { allRelations.push_back(r); } // end function 




}; // end class Database
