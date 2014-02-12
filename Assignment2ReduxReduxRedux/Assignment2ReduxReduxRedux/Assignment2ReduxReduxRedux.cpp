// Assignment2ReduxRedux.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <iterator>

#define varchar string*
#define integer int*
#define null NULL

using namespace std;

enum _Type 
{
	VARCHAR, 
	INT
};

struct _Data
{
	_Type Type;
	void* Data;

	_Data(_Type Type, void* Data)
	{
		this->Type = Type;
		this->Data = Data;
	}

	_Data(int input)
	{
		Type = _Type::INT;
		int* data = new int(input);
		this->Data = data;
	}

	_Data(string input)
	{
		Type = _Type::VARCHAR;
		string* data = new string(input);
		this->Data = data;
	}
};

ostream& operator<<(ostream& out, const _Data& d)
{
	if(d.Type == _Type::VARCHAR)
	{
		string *string_Data = (string*)d.Data;
		out<<*string_Data;
	}
	if(d.Type == _Type::INT)
	{
		int *int_Data = (int*)d.Data;
		out<<*int_Data;
	}

	return out;
}


class _Column
{
public:
	string Name;
	_Type Type;
	int AutoKey;
	bool AutoIncrement;
	vector<_Data> Rows;
	_Column(){}
	_Column(string Name, bool AutoIncrement, _Type Type)
	{
		this->Name = Name;
		this->Type = Type;
		this->AutoIncrement = AutoIncrement;
		this->AutoKey = 0;
	}

	void AddRow(_Data a)
	{
		if(!AutoIncrement)
			Rows.push_back(a);
		else
		{
			AutoKey++;
			Rows.push_back(_Data(AutoKey));
		}
	}

	_Data Get_Data(int Row)
	{
		return Rows[Row];
	}
};

class _Relation
{
public:
	string Name;
	vector<_Column> Columns;

	_Relation(string Name)
	{
		this->Name = Name;
	}
	void AddColumn(_Column C)
	{
		Columns.push_back(C);
	}

	void AddRow(vector<_Data> A)
	{
		for(int i = 0, a = 0; i < Columns.size(); i++)
		{
			Columns[i].AddRow(A[a]);
			if(!Columns[i].AutoIncrement)
				a++;
		}
	}

	void RemoveRow(int Row)
	{
		for(int i = 0; i <= Columns[0].Rows.size();i++)
		{
			Columns[i].Rows.erase(Columns[i].Rows.begin() + Row);
		}
		cout<<"Row " << Row + 1 << " removed\n";
	}

	void Print()
	{
		for(int i = 0; i < Columns.size(); i++)
			for(int k = 0; k < Columns[0].Rows.size();k++)
			{
				cout << Columns[i].Get_Data(k);
				cout << "\n";
			}
	}
};


int _tmain(int argc, _TCHAR* argv[])
{
	_Relation DB("DBTest");

	DB.AddColumn(_Column("ID",true,_Type::INT));
	DB.AddColumn(_Column("Name",false,_Type::VARCHAR));
	DB.AddColumn(_Column("Age",false,_Type::VARCHAR));

	vector<_Data> TestInput;
	TestInput.push_back(_Data("not asdf"));
	TestInput.push_back(_Data(69));

	DB.AddRow(TestInput);

	vector<_Data> TestInput2;
	TestInput2.push_back(_Data("asdff333"));
	TestInput2.push_back(_Data(11));

	DB.AddRow(TestInput2);

	vector<_Data> TestInput3;
	TestInput3.push_back(_Data("aasd3"));
	TestInput3.push_back(_Data(1111551));

	DB.AddRow(TestInput3);

	//DB.Print();

	DB.RemoveRow(1); //Remove input is offset by 1

	DB.Print();

	return 0;
}

