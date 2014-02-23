#pragma once

#include "stdafx.h"
#include "Relation.h"
#include "Database.h"
#include "Condition.h"
#include "Parser_Redux.h"
#include <vector>
#include <string>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <fstream>

using namespace std;

// TRIM FUNCTIONS BY Evan Teran 
string ltrim(string &s) {
	s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
	return s;
}
// trim from end
 string rtrim(string &s) {
	s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
	return s;
}
// trim from both ends
string trim(string &s) {
	return ltrim(rtrim(s));
}

// iterates and grabs every character from input
char GrabChar(string& S)
{
	string Newstring = "";
	S = ltrim(S);
	if (S.length() > 0)
	{
		for (int i = 1; i < S.length(); i++)
		{
			Newstring.push_back(S[i]);
		}
		char ReturnChar = S[0];
		S = Newstring;
		return ReturnChar;
	}
	return ' ';
}

string GrabToken(string& S)
{
	string Token;

	int Length = S.length();
	bool Quote = false;
	for (int i = 0; i < Length; i++)
	{
		if (S[i] != ' ' || Quote)
		{
			if (S[i] == '"')
			{
				if (Quote)
					Quote = false;
				else
					Quote = true;
			}
			else
				Token += S[i];
		}
		else
		{
			string Newstring = "";
			for (int i = Token.size(); i < S.size(); i++)
				Newstring.push_back(S[i]);
			S = Newstring;
			return Token;
		}
	}

	if (Token != "")
	{
		string Newstring = "";
		for (int i = Token.size(); i < S.size(); i++)
			Newstring.push_back(S[i]);
		S = Newstring;
		return Token;
	}
}

vector<string> GrabAllTokens(string S)
{
	string Token;
	vector<string> AllTokens;

	int Length = S.length();
	bool Quote = false;
	for (int i = 0; i < Length; i++)
	{

		if (S[i] != ' ' || Quote)
		{
			if (S[i] == '"')
			{
				if (Quote)
					Quote = false;
				else
					Quote = true;
			}
			else
				Token += S[i];
		}
		else
		{
			AllTokens.push_back(Token);
			Token = "";
		}
	}
	if (Token != "")
		AllTokens.push_back(Token);

	return AllTokens;
}

vector<_Data> GrabDataSets(string S)
{
	string Token;
	vector<_Data> AllTokens;

	int Length = S.length();
	bool Quote = false;
	for (int i = 0; i < Length; i++)
	{
		if (S[i] != '(' && S[i] != ')')
		{
			if (S[i] != ',')
			{
				if (S[i] == '"')
				{
					if (Quote)
					{
						Quote = false;
						AllTokens.push_back(_Data(trim(Token)));
						Token = "";
					}
					else
						Quote = true;
				}
				else
				{
					Token += S[i];
				}
			}
			else
			{
				if (Token != "")
				{
					AllTokens.push_back(_Data(atoi(Token.c_str())));
					Token = "";
				}
			}
		}
		if (S[i] == ')')
		{
			AllTokens.push_back(_Data(atoi(Token.c_str())));
			return AllTokens;
		}
	}
	if (Token != "")
		AllTokens.push_back(_Data(atoi(Token.c_str())));

	return AllTokens;
}

vector<string> GrabColumn(string S) {
	string Token;
	vector<string> AllTokens;

	int Length = S.length();
	for (int i = 0; i < Length; i++) {

		if (S[i] != '(' && S[i] != ')') {

			if (S[i] != ',') {

				Token += S[i];

			}
			else {

				AllTokens.push_back(trim(Token));

			} // end if else statement 

		}
		else {

			if (Token != "") {
				AllTokens.push_back(trim(Token));
				Token = "";
			} // end if statement 

		} // end if else statement 

		if (S[i] == ')') {

			return AllTokens;

		} // end of statement 

	}  // end for loop 

} // end GrabColumn

static string removeParentheses(string S) {

	S = trim(S);
	string newstring = "";
	if (S[0] == '(' && S[S.size() - 1] == ')') {

		for (int i = 1; i < S.size() - 2; i++) {

			newstring += S[i];

		} // end for loop

		return newstring;

	} // end if statement 

	newstring = S;
	return newstring;

} // end removeParentheses(string)

RelationName::RelationName() {

	Identifer = "";
	// NOTE: Relation name doesn't have a real identifier. 

} // end constructor 

// sees if the input is a relation name (must start with a-z or A-Z)
bool RelationName::Execute(Database* DB, string input) {

	string isName = GrabToken(input);
	ltrim(input);

	if (GrabToken(input) != "<-") {
		return false;
	} // end if statements 

	/*if (isalpha(isName[0]) && (isName[0] == '_') == false) 
		return false;*/

	if (alpha(isName[0]) == false) return false;

	for (int i = 1; i < isName.size(); i++) {

		if (alpha(isName[i]) == false || isdigit(isName[i]) == false) {
		//if (isalpha(isName[i]) == false || isName[i] == '_' == false || isdigit(isName[i]) == false) {

			Identifer = "";
			return false;

		} // end if statement - NOTE: obvious why do this?

	} // end for loop  - NOTE: please stop

	// NOTE: This needs to account for new relations created by relations too in the case of relation_name <- (expr) 
	relation = DB->getRelation(isName);
	return true;

} // end execute(Database, string)

bool RelationName::isArrow(string input) {

	string isArrow = "" + input[0] + input[1];
	string shouldBe = "<-";

	if (isArrow.compare(shouldBe)) {

		return true;

	} // end if statement

	return false;

} // end isArrow(string)

bool RelationName::digit(char d) {

	switch (d) {
	case '0': return true;
	case '1': return true;
	case '2': return true;
	case '3': return true;
	case '4': return true;
	case '5': return true;
	case '6': return true;
	case '7': return true;
	case '8': return true;
	case '9': return true;
	default: return false;
	} // end switch statement

} // end digit(char)

bool RelationName::alpha(char a) {

	switch (a) {
	case 'a': return true;
	case 'A': return true;
	case 'b': return true;
	case 'B': return true;
	case 'c': return true;
	case 'C': return true;
	case 'd': return true;
	case 'D': return true;
	case 'e': return true;
	case 'E': return true;
	case 'f': return true;
	case 'F': return true;
	case 'g': return true;
	case 'G': return true;
	case 'h': return true;
	case 'H': return true;
	case 'i': return true;
	case 'I': return true;
	case 'j': return true;
	case 'J': return true;
	case 'k': return true;
	case 'K': return true;
	case 'l': return true;
	case 'L': return true;
	case 'm': return true;
	case 'M': return true;
	case 'n': return true;
	case 'N': return true;
	case 'o': return true;
	case 'O': return true;
	case 'p': return true;
	case 'P': return true;
	case 'q': return true;
	case 'Q': return true;
	case 'r': return true;
	case 'R': return true;
	case 's': return true;
	case 'S': return true;
	case 't': return true;
	case 'T': return true;
	case 'u': return true;
	case 'U': return true;
	case 'v': return true;
	case 'V': return true;
	case 'w': return true;
	case 'W': return true;
	case 'x': return true;
	case 'X': return true;
	case 'y': return true;
	case 'Y': return true;
	case 'z': return true;
	case 'Z': return true;
	case '_': return true;
	default: return false;
	} // end switch statement

}


Selection::Selection() {

	Identifer = "select";

} // end constructor 

bool Selection::Execute(Database* DB, string input) {

	querys.push_back(new RelationName);
	querys.push_back(new Selection);
	querys.push_back(new Projection);
	querys.push_back(new Renaming);
	querys.push_back(new SetUnion);
	querys.push_back(new SetDifference);
	querys.push_back(new CrossProduct);
	querys.push_back(new NaturalJoin);

	//NOTE: How did you do condition? This is needed here. 
	return false;

} // end Exectue 

Projection::Projection() {

	Identifer = "project";

} // end constructor 

bool Projection::Execute(Database* DB, string input) {

	querys.push_back(new RelationName);
	querys.push_back(new Selection);
	querys.push_back(new Projection);
	querys.push_back(new Renaming);
	querys.push_back(new SetUnion);
	querys.push_back(new SetDifference);
	querys.push_back(new CrossProduct);
	querys.push_back(new NaturalJoin);

	input = ltrim(input);
	vector<string> attributeList = GrabColumn(input); // NOTE: This function should grab a list of the form (attribute_name1, attribute_name2, ... , attribute_nameN)
	string id;
	bool success;
	_Relation* newRelation = new _Relation("sudo");
	_Relation* Relation = new _Relation("sudo");

	input = removeParentheses(input); // NOTE: This should remove the parentheses around a (expr) 

	for (int i = 0; i < querys.size(); i++) {

		id = GrabToken(input); // NOTE: this should grab the identifier of the function (if it has one) 
		id = trim(id);

		if (id != "select" || id != "project" || id != "rename" && i == 0) {

			id = ""; // NOTE: this should set the identifier to "" in the case of which it doesn't have a real identifier 

		} // end if statement 

		success = querys[i]->Execute(DB, input);  // NOTE: this should evaluate to true if it was able to parse the input for that type of query 
		if (success && querys[i]->IsQuery(id)) {

			Relation = querys[i]->relation; // NOTE: if success was true, then we succeeded in parsing the relation 

		} // end if statement

	} // end for loop

	if (Relation != NULL) {

		newRelation = (DB->projection(attributeList, *(Relation)));
		relation = newRelation;
		return true;
	} // end if statement 

	return false;

} // end Execute 


Renaming::Renaming() {

	Identifer = "rename";

} // end constructor 

bool Renaming::Execute(Database* DB, string input) {

	// NOTE: this is the same as Projection (pretty much) 
	querys.push_back(new RelationName);
	querys.push_back(new Selection);
	querys.push_back(new Projection);
	querys.push_back(new Renaming);
	querys.push_back(new SetUnion);
	querys.push_back(new SetDifference);
	querys.push_back(new CrossProduct);
	querys.push_back(new NaturalJoin);

	input = ltrim(input);
	vector<string> attributeList = GrabColumn(input);
	string id;
	bool success;
	_Relation* newRelation = new _Relation("sudo");
	_Relation* Relation = new _Relation("sudo");

	input = removeParentheses(input);

	for (int i = 0; i < querys.size(); i++) {

		id = GrabToken(input);
		id = trim(id);

		if (id != "select" || id != "project" || id != "rename" && i == 0) {

			id = "";

		} // end if statement 

		success = querys[i]->Execute(DB, input);
		if (success && querys[i]->IsQuery(id)) {

			Relation = querys[i]->relation;

		} // end if statement

	} // end for loop
	if (Relation != NULL) {

		// NOTE: This still needs the renamed relation to give to relation 
		DB->renaming(attributeList, *(Relation));
		DB->addRelation(*(newRelation));
		relation = newRelation;
		return true;

	} // end if statement 

	return false;

} // end Execute 


SetUnion::SetUnion() {

	Identifer = "";

} // end constructor  

bool SetUnion::Execute(Database* DB, string input) {

	querys.push_back(new RelationName);
	querys.push_back(new Selection);
	querys.push_back(new Projection);
	querys.push_back(new Renaming);
	querys.push_back(new SetUnion);
	querys.push_back(new SetDifference);
	querys.push_back(new CrossProduct);
	querys.push_back(new NaturalJoin);

	input = ltrim(input);
	string relation1;
	string relation2;
	bool r = false;
	for (int i = 0; i < input.size(); i++) {

		if (input[i] != '+' && r == false) {

			relation1 += input[0];

		}
		else if (input[i] == '+') {  // NOTE: this won't find the correct + if there is more than one in the query (same for SetDifference, CrossProduct, and NaturalJoin) 

			r = true;

		} // end if else 

		if (r) {

			relation2 += input[i];

		} // end if statement 

	} // end for loop 

	// This should do what Projection does to evaluate a (expr), except for two (same for SetDifference, CrossProduct, and NaturalJoin) 
	string id1;
	string id2;
	bool success1;
	bool success2;
	_Relation *newRelation;
	_Relation* Relation1 = new _Relation("sudo");
	_Relation* Relation2 = new _Relation("sudo");

	relation1 = removeParentheses(relation1);
	relation2 = removeParentheses(relation2);

	for (int i = 0; i < querys.size(); i++) {

		id1 = GrabToken(relation1);
		id1 = trim(id1);
		id2 = GrabToken(relation2);
		id2 = trim(id2);

		if (id1 != "select" || id1 != "project" || id1 != "rename" && i == 0) {

			id1 = "";

		} // end if statement 

		if (id2 != "select" || id2 != "project" || id2 != "rename" && i == 0) {

			id2 = "";

		} // end if statement 

		success1 = querys[i]->Execute(DB, relation1);
		success2 = querys[i]->Execute(DB, relation2);

		if (success1 && querys[i]->IsQuery(id1)) {

			Relation1 = querys[i]->relation;

		} // end if statement

		if (success2 && querys[i]->IsQuery(id2)) {

			Relation2 = querys[i]->relation;

		} // end if statement

	} // end for loop

	newRelation = (DB->setUnion(*(Relation1), *(Relation2)));
	DB->addRelation(*(newRelation));
	relation = newRelation;
	return true;

} // end Execute(Database*, string)


SetDifference::SetDifference() {

	Identifer = "";

} // end constructor  

bool SetDifference::Execute(Database* DB, string input) {

	querys.push_back(new RelationName);
	querys.push_back(new Selection);
	querys.push_back(new Projection);
	querys.push_back(new Renaming);
	querys.push_back(new SetUnion);
	querys.push_back(new SetDifference);
	querys.push_back(new CrossProduct);
	querys.push_back(new NaturalJoin);

	input = ltrim(input);
	string relation1;
	string relation2;
	bool r = false;
	for (int i = 0; i < input.size(); i++) {

		if (input[i] != '-' && r == false) {

			relation1 += input[0];

		}
		else if (input[i] == '-') {

			r = true;

		} // end if else 

		if (r) {

			relation2 += input[i];

		} // end if statement 

	} // end for loop 

	string id1;
	string id2;
	bool success1;
	bool success2;
	_Relation* newRelation;
	_Relation* Relation1 = new _Relation("sudo");
	_Relation* Relation2 = new _Relation("sudo");

	relation1 = removeParentheses(relation1);
	relation2 = removeParentheses(relation2);

	for (int i = 0; i < querys.size(); i++) {

		id1 = GrabToken(relation1);
		id1 = trim(id1);
		id2 = GrabToken(relation2);
		id2 = trim(id2);

		if (id1 != "select" || id1 != "project" || id1 != "rename" && i == 0) {

			id1 = "";

		} // end if statement 

		if (id2 != "select" || id2 != "project" || id2 != "rename" && i == 0) {

			id2 = "";

		} // end if statement 

		success1 = querys[i]->Execute(DB, relation1);
		success2 = querys[i]->Execute(DB, relation2);

		if (success1 && querys[i]->IsQuery(id1)) {

			Relation1 = querys[i]->relation;

		} // end if statement

		if (success2 && querys[i]->IsQuery(id2)) {

			Relation2 = querys[i]->relation;

		} // end if statement

	} // end for loop

	newRelation = (DB->setDifference(*(Relation1), *(Relation2)));
	DB->addRelation(*(newRelation));
	relation = newRelation;
	return true;

} // end Execute(Database*, string)


CrossProduct::CrossProduct() {

	Identifer = "";

} // end constructor  

bool CrossProduct::Execute(Database* DB, string input) {

	querys.push_back(new RelationName);
	querys.push_back(new Selection);
	querys.push_back(new Projection);
	querys.push_back(new Renaming);
	querys.push_back(new SetUnion);
	querys.push_back(new SetDifference);
	querys.push_back(new CrossProduct);
	querys.push_back(new NaturalJoin);

	input = ltrim(input);
	string relation1;
	string relation2;
	bool r = false;
	for (int i = 0; i < input.size(); i++) {

		if (input[i] != '*' && r == false) {

			relation1 += input[0];

		}
		else if (input[i] == '*') {

			r = true;

		} // end if else 

		if (r) {

			relation2 += input[i];

		} // end if statement 

	} // end for loop 

	string id1;
	string id2;
	bool success1;
	bool success2;
	_Relation* newRelation;
	_Relation* Relation1 = new _Relation("sudo");
	_Relation* Relation2 = new _Relation("sudo");

	relation1 = removeParentheses(relation1);
	relation2 = removeParentheses(relation2);

	for (int i = 0; i < querys.size(); i++) {

		id1 = GrabToken(relation1);
		id1 = trim(id1);
		id2 = GrabToken(relation2);
		id2 = trim(id2);

		if (id1 != "select" || id1 != "project" || id1 != "rename" && i == 0) {

			id1 = "";

		} // end if statement 

		if (id2 != "select" || id2 != "project" || id2 != "rename" && i == 0) {

			id2 = "";

		} // end if statement 

		success1 = querys[i]->Execute(DB, relation1);
		success2 = querys[i]->Execute(DB, relation2);

		if (success1 && querys[i]->IsQuery(id1)) {

			Relation1 = querys[i]->relation;

		} // end if statement

		if (success2 && querys[i]->IsQuery(id2)) {

			Relation2 = querys[i]->relation;

		} // end if statement

	} // end for loop

	newRelation = (_Relation *)(DB->crossProduct(*(Relation1), *(Relation2)));
	DB->addRelation(*(newRelation));
	relation = newRelation;
	return true;

} // end Execute(Database*, string)


NaturalJoin::NaturalJoin() {

	Identifer = "";

} // end constructor 

bool NaturalJoin::Execute(Database* DB, string input) {

	querys.push_back(new RelationName);
	querys.push_back(new Selection);
	querys.push_back(new Projection);
	querys.push_back(new Renaming);
	querys.push_back(new SetUnion);
	querys.push_back(new SetDifference);
	querys.push_back(new CrossProduct);
	querys.push_back(new NaturalJoin);

	input = ltrim(input);
	string relation1 = GrabToken(input);
	string join = GrabToken(input); // NOTE: it should be able to grab JOIN here
	string relation2 = GrabToken(input);
	bool r = false;

	if (join != "JOIN") {

		return false;

	} // end if statement 

	string id1;
	string id2;
	bool success1;
	bool success2;
	_Relation* newRelation;
	_Relation* Relation1 = new _Relation("sudo");
	_Relation* Relation2 = new _Relation("sudo");

	relation1 = removeParentheses(relation1);
	relation2 = removeParentheses(relation2);

	for (int i = 0; i < querys.size(); i++) {

		id1 = GrabToken(relation1);
		id1 = trim(id1);
		id2 = GrabToken(relation2);
		id2 = trim(id2);

		if (id1 != "select" || id1 != "project" || id1 != "rename" && i == 0) {

			id1 = "";

		} // end if statement 

		if (id2 != "select" || id2 != "project" || id2 != "rename" && i == 0) {

			id2 = "";

		} // end if statement 


		success1 = querys[i]->Execute(DB, relation1);
		success2 = querys[i]->Execute(DB, relation2);

		if (success1 && querys[i]->IsQuery(id1)) {

			Relation1 = querys[i]->relation;

		} // end if statement

		if (success2 && querys[i]->IsQuery(id2)) {

			Relation2 = querys[i]->relation;

		} // end if statement

	} // end for loop

	newRelation = (_Relation *)(DB->naturalJoin(*(Relation1), *(Relation2)));
	DB->addRelation(*(newRelation));
	relation = newRelation;

} // end Execute(Database*, string)

