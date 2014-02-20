#include "stdafx.h"
#include "Relation.h"
#include "Database.h"
#include "Condition.h"
#include "Parser.h"
#include <vector>
#include <string>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <fstream>

using namespace std;

string ltrim(string &s);
string rtrim(string &s);
string trim(string &s);
char GrabChar(string&);
string GrabToken(string&);
vector<string>GrabAllTokens(string);
vector<_Data>GrabDataSets(string);
class Command
{
public:
	string Identifer;
	bool virtual IsCommand(string Input)
	{
		if(Input == Identifer)
			return true;
	}

	bool virtual Execute(Database* DB, string Input)
	{
		return false;
	}
};

// creates a new relation
class Create : public Command
{
public:
	Create(){
		Identifer = "CREATETABLE";
	}

	bool Execute(Database* DB,  string Input)
	{
		Input = ltrim(Input);
		string RelationName = GrabToken(Input); //Expecting Relation
		vector<_Column> Columns;

		Input = ltrim(Input);
		GrabChar(Input); //Gets rid of ')'

		while (Input[0] != ')')
		{
			Input = ltrim(Input);
			string ColName = GrabToken(Input);
			Input = ltrim(Input);


			string ColType = "";
			while (ColType != "VARCHAR" && ColType != "INTEGER")
				ColType += GrabChar(Input);


			if (ColType == "VARCHAR")
			{
				Input = ltrim(Input);
				GrabChar(Input); // Gets rid of '('

				string Size = "";
				while (Input[0] != ')') //Until ')'
					Size += GrabChar(Input); //Grabs varchar size

				GrabChar(Input); // Gets rid of ')'

				int RealSize = atoi(Size.c_str());

				Columns.push_back(_Column(ColName, false, _Type::VARCHAR));
				//R.AddColumn(_Column(ColName,false,_Type::VARCHAR));
			}
			if (ColType == "INTEGER")
			{
				if (ColName == "ID")
					Columns.push_back(_Column(ColName, true, _Type::INT));
				else
					Columns.push_back(_Column(ColName, false, _Type::INT));
			}

			ColType = "";
			ColName = "";

			Input = ltrim(Input);
			if (Input[0] == ',')
				GrabChar(Input);
		}
		DB->createTable(RelationName, Columns);
		return true;
	}
};

// inserts values into chosen relation
class Insert : public Command
{
public:
	Insert()
	{
		Identifer = "INSERTINTO";
	}

	bool Execute(Database* DB, string Input)
	{
		Input = ltrim(Input);
		string RelationName = GrabToken(Input); //Expecting Relation
		_Relation* Relation = DB->getRelation(RelationName);

		if(Relation != NULL)
		{
			string Token;
			while(Input.length() != 0)
			{
				Token += GrabChar(Input);
				if(Token == "VALUESFROM") //Expecting Values from
				{
					vector<_Data> SetData = GrabDataSets(Input);
					Relation->AddRow(SetData);
					return true;
				}
				else if(Token.length() > 10)
					return false;
			}
		}
		return false;
	}
};

// prints a relation
class Show : public Command
{
public:
	Show()
	{
		Identifer = "SHOW";
	}

	bool Execute(Database *DB,  string Input)
	{
		Input = ltrim(Input);
		string RelationName = GrabToken(Input);
		_Relation* Relation = DB->getRelation(RelationName);

		if (Relation != NULL)
		{
			string Token;
			while (Input.length() != 0)
			{
				Token += GrabChar(Input);
				Relation->Print();
				return true;
			}
		}
		return false;
	}

};

// loads a relation from a database file
class Open : public Command {
public:
	Open() {
		Identifer = "OPEN";
	}

	bool Execute(Database *DB,  string Input)
	{
		Input = ltrim(Input);
		string RelationName = GrabToken(Input); //Expecting a relation
		_Relation* Relation = DB->getRelation(RelationName);

		ifstream inf(RelationName + ".db");

		if (!inf)
		{
			cout << "Error, please enter valid filename.\n";
		}

		while (inf)
		{
			// read stuff from the file into a string and print it
			string strInput;
			inf >> strInput;
			//P->Execute(strInput);
			//cout << strInput << endl;
		}
		return true;
	}
};

// create a new relation file as [relationname].db
class Write : public Command
{
public:
	Write() {
		Identifer = "WRITE";
	}

	bool Execute(Database *DB,  string Input)
	{
		Input = ltrim(Input);
		string RelationName = GrabToken(Input); //Expecting a relation
		_Relation* Relation = DB->getRelation(RelationName);

		if (Relation != NULL)
		{
			ofstream myfile; //file to be written to
			myfile.open(RelationName + ".db", ios::out);
			myfile << "CREATE TABLE " << Relation->Name << " (";
			for (int i = 0; i < Relation->Columns.size(); i++) //manages relation types
			{
				myfile << Relation->Columns[i].Name << " ";
				if (Relation->Columns[i].Type == _Type::INT)
					myfile << "INTEGER";
				if (Relation->Columns[i].Type == _Type::VARCHAR)
					myfile << "VARCHAR";

				if (i != Relation->Columns.size() - 1)
					myfile << ",";
				else
					myfile << ")\n";
			}

			for (int i = 0; i < Relation->Columns[0].Rows.size(); i++) //manages data sets
			{
				myfile << "INSERT INTO " << RelationName << " VALUES FROM (";

				vector<_Data*> TestRow;
				for (int j = 0; j < Relation->Columns.size(); ++j)
					TestRow.push_back(&Relation->Columns[j].Rows[i]);

				for (int j = 0; j < TestRow.size(); j++)
				{
					if (TestRow[j]->Type == _Type::VARCHAR)
					{
						myfile << "\"";
						myfile << *(TestRow[j]);
						myfile << "\"";
					}
					if (TestRow[j]->Type == _Type::INT)
					{
						myfile << *(TestRow[j]);
					}

					if (j < TestRow.size() - 1)
						myfile << ",";
				}
				myfile << ");\n";
			}

			string Token;
			while (Input.length() != 0)
			{
				Token += GrabChar(Input);
				if (Token == "CLOSE") // saves all changes to the relation in th database file and closes it
				{
					GrabToken(Input); //Expecting Relation
					myfile.close(); //end of file write
					return true;
				}
				else if (Token.length() > 10)
					return false;
			}

		}
		return false;
	}
};

// exits from the DML interpreter
class Exiter : public Command
{
public:
	Exiter() {
		Identifer = "EXIT";
	}

	bool Execute(Database *DB,  string Input)
	{
		Input = ltrim(Input);
		while (Input == "EXIT")
		{
			break;
		}
		return true;
	}

};

class Update : public Command
{
public:
	Update(){
		Identifer = "UPDATE";
	}

	bool Execute(Database* DB, string Input)
	{
		Input = ltrim(Input);
		string RelationName = GrabToken(Input); //Expecting RelationName

		Input = ltrim(Input);

		if (GrabToken(Input) != "SET")
			return false;

		vector<string> AttributesToChange;
		vector<_Data> ChangeToThis;

		string Token = "";

		for (;;)
		{
			Input = ltrim(Input);
			string AttributeName = GrabToken(Input);
			Input = ltrim(Input);

			if (GrabToken(Input) != "=")
				return false;

			Input = ltrim(Input);

			if (Input[0] == '\"') // is varchar
			{
				GrabChar(Input); // gets rid of first quote
				string D = "";

				while (Input[0] != '\"')
					D += GrabChar(Input);

				GrabChar(Input); // Removes second quote

				AttributesToChange.push_back(AttributeName);
				ChangeToThis.push_back(_Data(D));
			}
			else
			{
				string D = "";

				while (isdigit(Input[0]))
					D += GrabChar(Input);

				Input = ltrim(Input);

				AttributesToChange.push_back(AttributeName);
				ChangeToThis.push_back(_Data(atoi(D.c_str())));
			}

			Input = ltrim(Input);
			if (Input[0] != '\,')
				break;
		}

		Input = ltrim(Input);

		if (GrabToken(Input) != "WHERE")
			return false;

		//Then do condition in this example we are expecting atribute = X
		Input = ltrim(Input);
		string AttributeNameWhere = GrabToken(Input);
		Input = ltrim(Input);
		string condition = GrabToken(Input);
		Input = ltrim(Input);
		string Argument = GrabToken(Input);
		Argument = rtrim(Argument);
		if (Argument[Argument.size() - 1] == ';')
			Argument.pop_back();

		Comparison asdf(AttributeNameWhere, _Data(Argument), Comparison::EQUALS);
		_Relation* Original = DB->getRelation(RelationName);
		_Relation Result = *(asdf.evaluate(DB->getRelation(RelationName)));	//ERROR DUE TO OUTDATED CONDITION FILE, CORRECTION CAN BE FOUND IN "MostUpDateVer2" on Dropbox
		for (int i = 0; i < Result.Columns.size(); i++)
		{
			for (int j = 0; j < AttributesToChange.size(); j++)
			{
				if (Result.Columns[i].Name == AttributesToChange[j])
				{
					for (int k = 0; k < Result.Columns[i].Rows.size(); k++)
					{
						_Data* Element = Original->GetElementByKey(&Result.Columns[0].Rows[k], i);
						Element->Data = new _Data(ChangeToThis[j]); // no results in comparison WHY????
					}
				}
			}
		}
		return true;
	}
};


class Parser
{
public:
	Database* DB;
	vector<Command*> Commands;

	string CurInput;

	Parser(Database* DB)
	{
		this->DB = DB;
		CurInput = "";
		Commands.push_back(new Insert);
		Commands.push_back(new Create);
		Commands.push_back(new Show);
		Commands.push_back(new Open);
		Commands.push_back(new Write);
		Commands.push_back(new Exiter);
	}

	bool Execute(string Input)
	{
		CurInput = Input;
		string Token = "";
		while(CurInput.length() != 0)
		{
			Token += GrabChar(CurInput);

			if(IsCommand(Token))
				ExecuteCommand(Token);
			if(IsQuery(Token))
				ExecuteQuery(Token);
		}
		return false;
	}

	void ExecuteCommand(string Token)
	{
		Command* C = GetCommand(Token);
		C->Execute(DB,CurInput);
	}

	void ExecuteQuery(string Input)
	{
		// NOTE: Still needs implimentation 
	}

	bool IsCommand(string Token)
	{
		for(int i = 0 ; i < Commands.size();i++)
		{
			if(Token == Commands[i]->Identifer)
				return true;
		}
		return false;
	}

	bool IsQuery(string Token)
	{
		return false;
		// NOTE: Still needs implimentation 
	}

	Command* GetCommand(string Token)
	{
		Command* NewCommand;
		for(int i = 0 ; i < Commands.size();i++)
		{
			if(Token == Commands[i]->Identifer)
			{
				NewCommand = Commands[i];
				return NewCommand;
			}
		}
		return NULL;
	}
};

// TRIM FUNCTIONS BY Evan Teran 
static string ltrim(string &s) {
	s.erase(s.begin(), find_if(s.begin(), s.end(), not1(ptr_fun<int, int>(isspace))));
	return s;
}

// trim from end
static  string rtrim(string &s) {
	s.erase(find_if(s.rbegin(), s.rend(), not1(ptr_fun<int, int>(isspace))).base(), s.end());
	return s;
}

// trim from both ends
static string trim(string &s) {
	return ltrim(rtrim(s));
}

// iterates and grabs every character from input
static char GrabChar(string& S)
{
	string NewString = "";
	S = ltrim(S);
	if (S.length() > 0)
	{
		for (int i = 1; i < S.length(); i++)
		{
			NewString.push_back(S[i]);
		}
		char ReturnChar = S[0];
		S = NewString;
		return ReturnChar;
	}
	return ' ';
}

static string GrabToken(string& S)
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
			string NewString = "";
			for (int i = Token.size(); i < S.size(); i++)
				NewString.push_back(S[i]);
			S = NewString;
			return Token;
		}
	}

	if (Token != "")
	{
		string NewString = "";
		for (int i = Token.size(); i < S.size(); i++)
			NewString.push_back(S[i]);
		S = NewString;
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
	for(int i = 0; i < Length; i++) {

		if(S[i] != '(' && S[i] != ')') {

			if(S[i] != ',') {

				Token += S[i]; 

			} else { 

				AllTokens.push_back(trim(Token));

			} // end if else statement 

		} else {
				
			if(Token != "") {
					AllTokens.push_back(trim(Token));
					Token = "";
			} // end if statement 

		} // end if else statement 
		
		if(S[i] == ')') {

			return AllTokens;

		} // end of statement 
	
	}  // end for loop 

} // end GrabColumn

string removeParentheses(string S) { 

	S = trim(S); 
	string newString = ""; 
	if (S[0] == '(' && S[S.size()-1] == ')') { 

		for (int i=1; i < S.size()-2; i++) { 

			newString += S[i]; 

		} // end for loop

		return newString; 

	} // end if statement 

	newString = S;
	return newString;

} // end removeParentheses(string)

	RelationName::RelationName() { 

		Identifer = ""; 
		// NOTE: Relation name doesn't have a real identifier. 

	} // end constructor 
    
    // sees if the input is a relation name (must start with a-z or A-Z)
    bool RelationName::Execute(Database* DB, string input) {
        
		string isName = GrabToken(input);
		isName = ltrim(input); 
        
		if(GrabToken(input) != "<-") {

			return false;

		} // end if statements 
        
        if (alpha(isName[0]) == false) return false;
        
        for (int i = 1; i < isName.size(); i++) {
            
            if (alpha(isName[i]) == false || digit(isName[i]) == false) {
                
                Identifer = ""; 
                return false;
                
            } // end if statement
            
        } // end for loop
        
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
            case '0' : return true;
            case '1' : return true;
            case '2' : return true;
            case '3' : return true;
            case '4' : return true;
            case '5' : return true;
            case '6' : return true;
            case '7' : return true;
            case '8' : return true;
            case '9' : return true;
            default : return false;
        } // end switch statement
        
    } // end digit(char)
    
    bool RelationName::alpha(char a) {
        
        switch (a) {
            case 'a' : return true;
            case 'A' : return true;
            case 'b' : return true;
            case 'B' : return true;
            case 'c' : return true;
            case 'C' : return true;
            case 'd' : return true;
            case 'D' : return true;
            case 'e' : return true;
            case 'E' : return true;
            case 'f' : return true;
            case 'F' : return true;
            case 'g' : return true;
            case 'G' : return true;
            case 'h' : return true;
            case 'H' : return true;
            case 'i' : return true;
            case 'I' : return true;
            case 'j' : return true;
            case 'J' : return true;
            case 'k' : return true;
            case 'K' : return true;
            case 'l' : return true;
            case 'L' : return true;
            case 'm' : return true;
            case 'M' : return true;
            case 'n' : return true;
            case 'N' : return true;
            case 'o' : return true;
            case 'O' : return true;
            case 'p' : return true;
            case 'P' : return true;
            case 'q' : return true;
            case 'Q' : return true;
            case 'r' : return true;
            case 'R' : return true;
            case 's' : return true;
            case 'S' : return true;
            case 't' : return true;
            case 'T' : return true;
            case 'u' : return true;
            case 'U' : return true;
            case 'v' : return true;
            case 'V' : return true;
            case 'w' : return true;
            case 'W' : return true;
            case 'x' : return true;
            case 'X' : return true;
            case 'y' : return true;
            case 'Y' : return true;
            case 'z' : return true;
            case 'Z': return true;
			case '_' : return true; 
            default : return false;
        } // end switch statement
        
    } // end alpha(char)


	Selection::Selection() { 

		Identifer = "select";

	} // end constructor 

	bool Selection::Execute(Database* DB,string input) { 

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
		std::string id; 
		bool success; 
		_Relation* newRelation = new _Relation(); 
		_Relation* Relation = new _Relation();

		input = removeParentheses(input); // NOTE: This should remove the parentheses around a (expr) 

		for (int i=0; i < querys.size(); i++) { 

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

		if(Relation != NULL) {

			newRelation = DB->projection(attributeList, *(Relation)); 
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
		_Relation* newRelation = new _Relation(); 
		_Relation* Relation = new _Relation();

		input = removeParentheses(input);

		for (int i=0; i < querys.size(); i++) { 

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
		if(Relation != NULL) {

			// NOTE: This still needs the renamed relation to give to relation 
			DB->renaming(attributeList, Relation); 
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

			} else if (input[i] == '+') {  // NOTE: this won't find the correct + if there is more than one in the query (same for SetDifference, CrossProduct, and NaturalJoin) 

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
		_Relation* newRelation; 
		_Relation* Relation1 = new _Relation();
		_Relation* Relation2 = new _Relation(); 

		relation1 = removeParentheses(relation1);
		relation2 = removeParentheses(relation2); 

		for (int i=0; i < querys.size(); i++) { 

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

		newRelation = DB->setUnion(*(Relation1), *(Relation2)); 
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

			} else if (input[i] == '-') {

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
		_Relation* Relation1 = new _Relation();
		_Relation* Relation2 = new _Relation(); 

		relation1 = removeParentheses(relation1);
		relation2 = removeParentheses(relation2); 

		for (int i=0; i < querys.size(); i++) { 

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

		newRelation = DB->setDifference(*(Relation1), *(Relation2)); 
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

			} else if (input[i] == '*') {

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
		_Relation* Relation1 = new _Relation();
		_Relation* Relation2 = new _Relation(); 

		relation1 = removeParentheses(relation1);
		relation2 = removeParentheses(relation2); 

		for (int i=0; i < querys.size(); i++) { 

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

		newRelation = DB->crossProduct(*(Relation1), *(Relation2)); 
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
		string relation2 = GrabToken (input);
		bool r = false; 

		if (join != "JOIN") { 

			return false; 

		} // end if statement 
		
		string id1; 
		string id2; 
		bool success1;
		bool success2;
		_Relation* newRelation; 
		_Relation* Relation1 = new _Relation();
		_Relation* Relation2 = new _Relation(); 

		relation1 = removeParentheses(relation1);
		relation2 = removeParentheses(relation2);

		for (int i=0; i < querys.size(); i++) { 

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

		newRelation = DB->crossProduct(*(Relation1), *(Relation2)); 
		DB->addRelation(*(newRelation)); 
		relation = newRelation;

	} // end Execute(Database*, string)


int main()
{
	Database DB;
	vector<_Column> Columns;
	Columns.push_back(_Column("ID", true, _Type::INT));
	Columns.push_back(_Column("Name", false, _Type::VARCHAR));
	Columns.push_back(_Column("Pet", false, _Type::VARCHAR));
	Columns.push_back(_Column("Age", false, _Type::INT));
	DB.createTable("animals", Columns);
	Parser P(&DB);


	P.Execute("CREATE TABLE animals (ID INTEGER, Name VARCHAR(20), PET VARCHAR(20), Age INTEGER)");
	P.Execute("INSERT INTO animals VALUES FROM (\"Joe helloasdf\", \"cat\", 4);");
	P.Execute("INSERT INTO animals VALUES FROM (\"hello\", \"dog\", 234);");
	P.Execute("WRITE animals");
	P.Execute("CLOSE animals");
	DB.getRelation("animals")->Print();
	P.Execute("EXIT");
}

