#pragma once

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <string>
#include <iterator>

using namespace std;

enum _Type
{
	VARCHAR,
	INT
};

struct _Data {
	_Type Type;
	void* Data;

	_Data(void);
	_Data(_Type Type, void* Data);
	_Data(int input);
	_Data(string input);

	int GetInt()
	{
		return *((int*)Data);
	}
};

ostream& operator<<(ostream& out, const _Data& d);

class _Column {
public:
	string Name;
	_Type Type;
	int AutoKey;
	bool AutoIncrement;
	vector<_Data> Rows;

	_Column();
	_Column(string Name, bool AutoIncrement, _Type Type);

	void AddRow(_Data a);
	_Data Get_Data(int Row);
};

class _Relation {
public:
	string Name;
	vector<_Column> Columns;

	_Relation(string Name);

	string GetName();
	_Data GetRow(int Column, int Row);
	_Data GetRow2(int Column, int Row);

	void AddColumn(_Column c);
	void AddRow(vector<_Data> a);
	void RemoveRow(int Row);

	vector<_Data> GetRow(int index);
	vector<_Data> CombineRows(vector<_Data> a, vector<_Data> b);

	_Data* GetElementByKey(_Data* ID, int ColID);
	int removeColumn(int a);
	void Print();
};
