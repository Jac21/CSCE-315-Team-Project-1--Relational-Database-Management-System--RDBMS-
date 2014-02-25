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

vector<string> GrabCondition(string& S) {

	string Token;
	vector<string> AllTokens;

	int Length = S.length();
	for (int i = 0; i < Length; i++) {

		if (S[i] != '(' && S[i] != ')') {

			if (S[i] != ' ' && S[i] != '"') {

				Token += S[i];

			} else {

				if (Token != "") {
					AllTokens.push_back(trim(Token));
					Token = "";
				} // end if statement 

			} // end if else statement 

		} else {

			if (Token != "") {
				AllTokens.push_back(trim(Token));
				Token = "";
			} // end if statement 

		} // end if else statement 

		if (S[i] == ')') {

			string newS = "";
			for (int j = i + 1; j < S.size(); j++) {

				newS += S[j];

			} // end for loop
			S = newS;

			return AllTokens;

		} // end of statement 

	}  // end for loop 

} // end GrabColumn(string)

string removeParentheses(string S) {

	S = trim(S);
	string newstring = "";
	if (S[0] == '(' && S[S.size() - 1] == ')') {

		for (int i = 1; i < S.size() - 1; i++) {

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

	ltrim(input);
	string test = input;
	string isName = GrabToken(input);
	ltrim(input);

	if (isName[(isName.size() - 1)] == ';') {
		isName.pop_back();
		test.pop_back();
	} // end if statement 

	if (isName != test) return false;

	if (alpha(isName[0]) == false) return false;

	for (int i = 1; i < isName.size(); i++) {

		if (alpha(isName[i]) == true || digit(isName[i]) == true) {

			// empty -- we're good if this is true 

		}
		else {

			// if we get here, it was neither an alpha or a digit and is therefore bad 
			return false;

		} // end if else statment 

	} // end for loop  - NOTE: please stop

	relation = DB->getRelation(isName);
	if (relation == NULL) {

		cout << "This relation is not in the database.";
		return false;
	} // end if statement 

	return true;

} // end execute(Database, string)

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

} // end alpha(char)


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

	input = ltrim(input);
	if (input[input.size() - 1] == ';') {
		input.pop_back();
	} // end if statement 
	string id = GrabToken(input);
	if (id != Identifer) return false;
	input = ltrim(input);
	vector<string> condition = GrabCondition(input);
	string AttributeNameWhere = condition[0];
	string cond = condition[1];
	string Argument = condition[2];
	input = ltrim(input);
	Comparison c;
	if (cond == "==") {

		c = Comparison(AttributeNameWhere, _Data(Argument), Comparison::EQUALS);

	}
	else if (cond == ">=") {

		c = Comparison(AttributeNameWhere, _Data(Argument), Comparison::GREATERTHANOREQUAL);

	}
	else if (cond == "<=") {

		c = Comparison(AttributeNameWhere, _Data(Argument), Comparison::LESSTNANOREQUAL);

	}
	else if (cond == "!=") {

		c = Comparison(AttributeNameWhere, _Data(Argument), Comparison::NOTEQUALS);

	}
	else if (cond == ">") {

		c = Comparison(AttributeNameWhere, _Data(Argument), Comparison::GREATERTHAN);

	}
	else if (cond == "<") {

		c = Comparison(AttributeNameWhere, _Data(Argument), Comparison::LESSTHAN);

	} // end if else statement for condition 

	bool success;
	_Relation* newRelation = NULL;
	_Relation* Relation = NULL;

	input = removeParentheses(input); // NOTE: This should remove the parentheses around a (expr) 

	for (int i = 0; i < querys.size(); i++) {

		success = querys[i]->Execute(DB, input);  // NOTE: this should evaluate to true if it was able to parse the input for that type of query 
		if (success && querys[i]->IsQuery(id)) {

			Relation = querys[i]->relation; // NOTE: if success was true, then we succeeded in parsing the relation 
			break;

		} // end if statement

	} // end for loop

	if (Relation != NULL) {

		newRelation = (DB->selection(c, *(Relation)));
		relation = newRelation;
		return true;
	} // end if statement 

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
	if (input[input.size() - 1] == ';') {
		input.pop_back();
	} // end if statement 
	string id = GrabToken(input);
	if (id != Identifer) return false;
	input = ltrim(input);
	vector<string> attributeList = GrabColumn(input); // this function should grab a list of the form (attribute_name1, attribute_name2, ... , attribute_nameN)
	input = ltrim(input);
	bool success;
	_Relation* newRelation = NULL;
	_Relation* Relation = NULL;

	input = removeParentheses(input); // NOTE: This should remove the parentheses around a (expr) 

	for (int i = 0; i < querys.size(); i++) {

		success = querys[i]->Execute(DB, input);  // NOTE: this should evaluate to true if it was able to parse the input for that type of query 
		if (success && querys[i]->IsQuery(id)) {

			Relation = querys[i]->relation; // NOTE: if success was true, then we succeeded in parsing the relation 
			break;

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
	if (input[input.size() - 1] == ';') {
		input.pop_back();
	} // end if statement 
	string id = GrabToken(input);
	if (id != Identifer) return false;
	input = ltrim(input);
	vector<string> attributeList = GrabColumn(input); // this function should grab a list of the form (attribute_name1, attribute_name2, ... , attribute_nameN)
	input = ltrim(input);
	bool success;
	_Relation* newRelation = NULL;
	_Relation* Relation = NULL;

	input = removeParentheses(input);

	for (int i = 0; i < querys.size(); i++) {

		success = querys[i]->Execute(DB, input);
		if (success && querys[i]->IsQuery(id)) {

			Relation = querys[i]->relation;
			break;

		} // end if statement

	} // end for loop
	if (Relation != NULL) {

		// NOTE: This still needs the renamed relation to give to relation 
		newRelation = DB->renaming(attributeList, *(Relation));
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
	if (input[input.size() - 1] == ';') {
		input.pop_back();
	} // end if statement 
	string relation1 = "";
	string relation2 = "";
	bool r = false;
	for (int i = 0; i < input.size(); i++) {

		if (input[i] != '+' && r == false) {

			relation1 += input[i];

		}
		else if (input[i] == '+') {  // NOTE: this won't find the correct + if there is more than one in the query (same for SetDifference, CrossProduct, and NaturalJoin) 

			r = true;

		} // end if else 

		if (input[i] != '+' && r) {

			relation2 += input[i];

		} // end if statement 

	} // end for loop 

	relation1 = trim(relation1);
	relation2 = trim(relation2);
	if (relation1 == "") return false;
	if (relation2 == "") return false; 

	// This should do what Projection does to evaluate a (expr), except for two (same for SetDifference, CrossProduct, and NaturalJoin) 
	bool success1;
	bool success2;
	_Relation *newRelation;
	_Relation* Relation1 = NULL;
	_Relation* Relation2 = NULL;
	bool got1 = false;
	bool got2 = false;

	relation1 = removeParentheses(relation1);
	relation2 = removeParentheses(relation2);

	string r1 = "";
	string r2 = "";

	for (int i = 0; i < querys.size(); i++) {

		r1 = relation1;
		success1 = querys[i]->Execute(DB, r1);

		if (success1 && !got1) {

			Relation1 = querys[i]->relation;
			got1 = true;

		} // end if statement

		r2 = relation2;
		success2 = querys[i]->Execute(DB, r2);

		if (success2 && !got2) {

			Relation2 = querys[i]->relation;
			got2 = true;

		} // end if statement

		if (got1 && got2) break;

	} // end for loop

	if (Relation1 != NULL && Relation2 != NULL) {

		newRelation = (DB->setUnion(*(Relation1), *(Relation2)));
		DB->addRelation(*(newRelation));
		relation = newRelation;
		return true;

	} // end if statement 

	return false;

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
	if (input[input.size() - 1] == ';') {
		input.pop_back();
	} // end if statement 
	string relation1 = "";
	string relation2 = "";
	bool r = false;
	for (int i = 0; i < input.size(); i++) {

		if (input[i] != '-' && r == false) {

			relation1 += input[i];

		}
		else if (input[i] == '-') {  // NOTE: this won't find the correct + if there is more than one in the query (same for SetDifference, CrossProduct, and NaturalJoin) 

			r = true;

		} // end if else 

		if (input[i] != '-' && r) {

			relation2 += input[i];

		} // end if statement 

	} // end for loop 

	relation1 = trim(relation1);
	relation2 = trim(relation2);
	if (relation1 == "") return false;
	if (relation2 == "") return false;

	// This should do what Projection does to evaluate a (expr), except for two (same for SetDifference, CrossProduct, and NaturalJoin) 
	string id1;
	string id2;
	bool success1;
	bool success2;
	_Relation *newRelation;
	_Relation* Relation1 = NULL;
	_Relation* Relation2 = NULL;
	bool got1 = false;
	bool got2 = false;

	relation1 = removeParentheses(relation1);
	relation2 = removeParentheses(relation2);

	string r1 = "";
	string r2 = "";

	for (int i = 0; i < querys.size(); i++) {

		r1 = relation1;
		success1 = querys[i]->Execute(DB, r1);

		if (success1 && !got1) {

			Relation1 = querys[i]->relation;
			got1 = true;

		} // end if statement

		r2 = relation2;
		success2 = querys[i]->Execute(DB, r2);

		if (success2 && !got2) {

			Relation2 = querys[i]->relation;
			got2 = true;

		} // end if statement

		if (got1 && got2) break;

	} // end for loop

	if (Relation1 != NULL && Relation2 != NULL) {

		newRelation = (DB->setDifference(*(Relation1), *(Relation2)));
		DB->addRelation(*(newRelation));
		relation = newRelation;
		return true;

	} // end if statement 

	return false;

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
	if (input[input.size() - 1] == ';') {
		input.pop_back();
	} // end if statement 
	string relation1 = "";
	string relation2 = "";
	bool r = false;
	for (int i = 0; i < input.size(); i++) {

		if (input[i] != '*' && r == false) {

			relation1 += input[i];

		}
		else if (input[i] == '*') {  // NOTE: this won't find the correct + if there is more than one in the query (same for SetDifference, CrossProduct, and NaturalJoin) 

			r = true;

		} // end if else 

		if (input[i] != '*' && r) {

			relation2 += input[i];

		} // end if statement 

	} // end for loop 

	relation1 = trim(relation1);
	relation2 = trim(relation2);
	if (relation1 == "") return false;
	if (relation2 == "") return false;

	// This should do what Projection does to evaluate a (expr), except for two (same for SetDifference, CrossProduct, and NaturalJoin) 
	string id1;
	string id2;
	bool success1;
	bool success2;
	_Relation *newRelation;
	_Relation* Relation1 = NULL;
	_Relation* Relation2 = NULL;
	bool got1 = false;
	bool got2 = false;

	relation1 = removeParentheses(relation1);
	relation2 = removeParentheses(relation2);

	string r1 = "";
	string r2 = "";

	for (int i = 0; i < querys.size(); i++) {

		r1 = relation1;
		success1 = querys[i]->Execute(DB, r1);

		if (success1 && !got1) {

			Relation1 = querys[i]->relation;
			got1 = true;

		} // end if statement

		r2 = relation2;
		success2 = querys[i]->Execute(DB, r2);

		if (success2 && !got2) {

			Relation2 = querys[i]->relation;
			got2 = true;

		} // end if statement

		if (got1 && got2) break;

	} // end for loop

	if (Relation1 != NULL && Relation2 != NULL) {

		newRelation = (DB->crossProduct(*(Relation1), *(Relation2)));
		DB->addRelation(*(newRelation));
		relation = newRelation;
		return true;

	} // end if statement 

	return false;

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
	input = ltrim(input);
	string join = GrabToken(input); // NOTE: it should be able to grab JOIN here
	input = ltrim(input);
	string relation2 = GrabToken(input);
	bool r = false;
	relation1 = trim(relation1);
	relation2 = trim(relation2);
	join = trim(join);

	if (join != "JOIN") {

		return false;

	} // end if statement 

	bool success1;
	bool success2;
	_Relation *newRelation;
	_Relation* Relation1 = NULL;
	_Relation* Relation2 = NULL;
	bool got1 = false;
	bool got2 = false;

	relation1 = removeParentheses(relation1);
	relation2 = removeParentheses(relation2);

	string r1 = "";
	string r2 = "";

	for (int i = 0; i < querys.size(); i++) {

		r1 = relation1;
		success1 = querys[i]->Execute(DB, r1);

		if (success1 && !got1) {

			Relation1 = querys[i]->relation;
			got1 = true;

		} // end if statement

		r2 = relation2;
		success2 = querys[i]->Execute(DB, r2);

		if (success2 && !got2) {

			Relation2 = querys[i]->relation;
			got2 = true;

		} // end if statement

		if (got1 && got2) break;

	} // end for loop

	if (Relation1 != NULL && Relation2 != NULL) {

		newRelation = (DB->naturalJoin(*(Relation1), *(Relation2)));
		DB->addRelation(*(newRelation));
		relation = newRelation;
		return true;

	} // end if statement 

	return false;

} // end Execute(Database*, string)

int main() {

	Database DB;
	/*
	vector<_Column> Columns;
	Columns.push_back(_Column("ID", true, _Type::INT));
	Columns.push_back(_Column("Name", false, _Type::VARCHAR));
	Columns.push_back(_Column("Pet", false, _Type::VARCHAR));
	Columns.push_back(_Column("Age", false, _Type::INT));
	DB.createTable("animals", Columns);
	*/
	Parser P(&DB);


	P.Execute("CREATE TABLE animals (ID INTEGER, Name VARCHAR(20), PET VARCHAR(20), Age INTEGER)");
	P.Execute("INSERT INTO animals VALUES FROM (\"Joe helloasdf\", \"cat\", 4);");
	P.Execute("INSERT INTO animals VALUES FROM (\"hello\", \"dog\", 234);");
	P.Execute("WRITE animals");
	P.Execute("CLOSE animals");
	DB.getRelation("animals")->Print();
	//	P.Execute("name_test <- animals;"); 
	//	P.Execute("project_test <- project (Name, PET) animals;"); 
	// P.Execute("rename_test <- rename (v_ID, v_Name, v_Pet, v_age) animals;"); 
	// P.Execute("SHOW project_test;"); 
	P.Execute("CREATE TABLE friends (fname VARCHAR(20), lname VARCHAR(20), personality VARCHAR(20), value INTEGER) PRIMARY KEY (fname, lname);");
	P.Execute("INSERT INTO friends VALUES FROM (\"X\", \"N/A\", \"Awesome!\", 100);");
	P.Execute("INSERT INTO friends VALUES FROM (\"Smith\", \"Smith\", \"Annoying\", 5);");
	P.Execute("INSERT INTO friends VALUES FROM (\"Algebra\", \"Homework\", \"Boring\", -100);");
	P.Execute("CREATE TABLE enemies (fname VARCHAR(20), lname VARCHAR(20), personality VARCHAR(20), value INTEGER) PRIMARY KEY (fname, lname);");
	P.Execute("INSERT INTO enemies VALUES FROM (\"X\", \"N/A\", \"Awesome!\", 100);");
	P.Execute("INSERT INTO enemies VALUES FROM (\"The\", \"Penguin\", \"Weird\", 100);");
	P.Execute("INSERT INTO enemies VALUES FROM (\"Joker\", \"N/A\", \"Weird\", 150);");
	//	P.Execute("good_and_bad_guys <- friends + enemies;"); 
	P.Execute("CREATE TABLE baseball_players (fname VARCHAR(20), lname VARCHAR(30), team VARCHAR(20), homeruns INTEGER, salary INTEGER) PRIMARY KEY (fname, lname);");
	P.Execute("INSERT INTO baseball_players VALUES FROM (\"Joe\", \"Swatter\", \"Pirates\", 40, 1000000);");
	P.Execute("INSERT INTO baseball_players VALUES FROM (\"Sarah\", \"Batter\", \"Dinosaurs\", 100, 5000000);");
	P.Execute("INSERT INTO baseball_players VALUES FROM (\"Snoopy\", \"Slinger\", \"Pirates\", 3, 200000);");
	P.Execute("INSERT INTO baseball_players VALUES FROM (\"Donald\", \"Runner\", \"Dinosaurs\", 89, 200000);");
	P.Execute("INSERT INTO baseball_players VALUES FROM (\"Alexander\", \"Smith\", \"Pirates\", 2, 150000);");
//	P.Execute("high_hitters <- select (homeruns >= 40) baseball_players;");
//	P.Execute("diff_test <- friends - enemies;");
//	P.Execute("high_hit_pirates <- select (team == \"Pirates\") (select (homeruns >= 40) baseball_players);");
	P.Execute("high_hit_players <- (select (team == \"Priates\") baseball_players) * (select (homeruns >= 40) baseball_players);"); 



	P.Execute("EXIT");

} // end main 