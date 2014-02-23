#pragma once

#include "Commands.h"
#include "Parser_Redux.h"

using namespace std;

Command::Command()
{
}

// creates a new relation
Create::Create()
{
	Identifer = "CREATETABLE";
}
bool Create::IsCommand(string Input)
{
	if (Input == Identifer)
		return true;
}
bool Create::Execute(Database* DB, string Input)
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

// inserts values into chosen relation
Insert::Insert()
{
	Identifer = "INSERTINTO";
}

bool Insert::IsCommand(string Input)
{
	if (Input == Identifer)
		return true;
}

bool Insert::Execute(Database* DB, string Input)
{
	Input = ltrim(Input);
	string RelationName = GrabToken(Input); //Expecting Relation
	_Relation* Relation = DB->getRelation(RelationName);

	if (Relation != NULL)
	{
		string Token;
		while (Input.length() != 0)
		{
			Token += GrabChar(Input);
			if (Token == "VALUESFROM") //Expecting Values from
			{
				vector<_Data> SetData = GrabDataSets(Input);
				Relation->AddRow(SetData);
				return true;
			}
			else if (Token.length() > 10)
				return false;
		}
	}
	return false;
}

// prints a relation
Show::Show()
{
	Identifer = "SHOW";
}

bool Show::IsCommand(string Input)
{
	if (Input == Identifer)
		return true;
}

bool Show::Execute(Database *DB, string Input)
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

// loads a relation from a database file
Open::Open()
{
	Identifer = "OPEN";
}

bool Open::IsCommand(string Input)
{
	if (Input == Identifer)
		return true;
}

bool Open::Execute(Database *DB, string Input)
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

// create a new relation file as [relationname].db
Write::Write()
{
	Identifer = "WRITE";
}

bool Write::IsCommand(string Input)
{
	if (Input == Identifer)
		return true;
}

bool Write::Execute(Database *DB, string Input)
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

// exits from the DML interpreter
Exiter::Exiter()
{
	Identifer = "EXIT";
}

bool Exiter::IsCommand(string Input)
{
	if (Input == Identifer)
		return true;
}

bool Exiter::Execute(Database *DB, string Input)
{
	Input = ltrim(Input);
	while (Input == "EXIT")
	{
		break;
	}
	return true;
}

Update::Update()
{
	Identifer = "UPDATE";
}

bool Update::IsCommand(string Input)
{
	if (Input == Identifer)
		return true;
}

bool Update::Execute(Database* DB, string Input)
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
	_Relation Result = (asdf.evaluate(DB->getRelation(RelationName)));	//ERROR DUE TO OUTDATED CONDITION FILE, CORRECTION CAN BE FOUND IN "MostUpDateVer2" on Dropbox
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