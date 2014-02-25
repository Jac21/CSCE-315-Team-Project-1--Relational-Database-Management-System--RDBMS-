#pragma once

#include "Database.h"
#include <vector>
#include <string>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <fstream>

using namespace std;

class Command
{
public:
	Command();
	bool Insert();
	bool Show();
	bool Open();
	bool Write();
	bool Exiter();
	bool Update();
	bool Delete();
	string Identifer;
	bool virtual IsCommand(string Input)
	{
		if (Input == Identifer)
			return true;
	}

	bool virtual Execute(Database* DB, string Input)
	{
		return false;
	}
};

class Create : public Command
{
public:
	Create();
	bool IsCommand(string Input);
	bool Execute(Database* DB, string Input);
};

class Insert : public Command
{
public:
	Insert();
	bool IsCommand(string Input);
	bool Execute(Database* DB, string Input);
};

class Show : public Command
{
public:
	Show();
	bool IsCommand(string Input);
	bool Execute(Database* DB, string Input);
};

class Open : public Command
{
public:
	Open();
	bool IsCommand(string Input);
	bool Execute(Database* DB, string Input);
};

class Write : public Command
{
public:
	Write();
	bool IsCommand(string Input);
	bool Execute(Database* DB, string Input);
};

class Exiter : public Command
{
public:
	Exiter();
	bool IsCommand(string Input);
	bool Execute(Database* DB, string Input);
};

class Update : public Command
{
public:
	Update();
	bool IsCommand(string Input);
	bool Execute(Database* DB, string Input);
};

class Delete : public Command
{
public:
	Delete();
	bool IsCommand(string Input);
	bool Execute(Database* DB, string Input);
};





