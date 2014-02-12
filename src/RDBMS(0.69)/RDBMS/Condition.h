#pragma once

#include "Relation.h"
#include "std_lib_facilities.h"

/*------------------------------------|
	Authors: Jeffrey LeRoy & Lauren   |
	Group: Team C                     |
	RDBMS Project			          |
	Last Edited 2/10/14		          |
-------------------------------------*/

class Condition { 
public:
	virtual _Relation evaluate(_Relation r) = 0; 



}; // end class Condition 

class Comparison : public Condition { 
public:
	String attributeName;
	_Data compD;
	enum Comp { EQUALS, NOTEQUALS, LESSTHAN, GREATERTHAN, LESSTNANOREQUAL, GREATERTHANOREQUAL };
	Comp c;

	Comparison(void);
	Comparison(String attname, _Data t, Comp x);

	_Relation evaluate(_Relation r); 

}; // end class Comparison 

class OR : public Condition { 
public:
	Comparison c1;
	Comparison c2; 
	OR(Comparison c1, Comparison c2);
	_Relation evaluate(_Relation r); 

}; // end class OR

class AND : public Condition { 
public:
	Comparison c1;
	Comparison c2; 
	AND(void);
	AND(Comparison c1, Comparison c2);
	_Relation evaluate(_Relation r); 

}; // end class AND

