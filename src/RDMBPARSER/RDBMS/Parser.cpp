#include "stdafx.h"
#include "Relation.h"
#include "Database.h"
#include "Condition.h"
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
		_Relation Result = asdf.evaluate(DB->getRelation(RelationName));	//ERROR DUE TO OUTDATED CONDITION FILE, CORRECTION CAN BE FOUND IN "MostUpDateVer2" on Dropbox
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

