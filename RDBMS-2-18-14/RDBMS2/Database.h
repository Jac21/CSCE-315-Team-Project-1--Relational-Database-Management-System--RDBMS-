/*
Database.h
Created by Lauren Fuller on 2/1/14.
*/

#pragma once

#include "Condition.h"
#include <vector>

class Database {
   
   // Variables associated with the Database
   std::vector<_Relation> allRelations;
   
public:
   
   // seven necessary operations 
   _Relation* selection(Condition& c, _Relation r);
   _Relation* projection(std::vector<string> attributes, _Relation r);
   void renaming(std::vector<std::string> renames, _Relation* r);
   _Relation* setUnion(_Relation a, _Relation b);
   _Relation* setDifference(_Relation& a, _Relation& b);
   _Relation* crossProduct(_Relation a, _Relation b);
   _Relation* naturalJoin(_Relation a, _Relation b);

   // other operations 
   void createTable(std::string name, std::vector<_Column> attributes);
   void dropTable(_Relation r);
   void insertInto(std::string name, std::vector<_Data> literals);
   void insertInto(std::string name, _Relation& r);  
   void update(std::string rName, std::vector<std::string> attributes, std::vector<_Data> newValues, Condition& c); 
   void deleteFrom(std::string name, Condition& c);
   void show(std::string name);

   void addRelation(_Relation r);
   _Relation* getRelation(std::string name);
}; // end class Database
