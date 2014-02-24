#include "Relation.h"

//Testing
/* -- DATA CLTestRow --
   -- CONSTRUCTOR -- */
_Data::_Data(void) {}

_Data::_Data(_Type Type, void* Data){
	this->Type = Type;
	this->Data = Data;
}

_Data::_Data(int input){
	Type = _Type::INT;
	int* data = new int(input);
	this->Data = data;
}

_Data::_Data(string input){
	Type = _Type::VARCHAR;
	string* data = new string(input);
	this->Data = data;
}

// Overloaded << operator to output Data
ostream& operator<<(ostream& out, const _Data& d){
	if (d.Type == _Type::VARCHAR) {
		string *string_Data = (string*)d.Data;
		out << *string_Data;
	}
	if (d.Type == _Type::INT) {
		int *int_Data = (int*)d.Data;
		out << *int_Data;
	}
	return out;
}

/* -- COLUMN CLTestRow --
   -- CONSTRUCTORS -- */
_Column::_Column() {}

_Column::_Column(string Name, bool AutoIncrement, _Type Type) {
	this->Name = Name;
	this->Type = Type;
	this->AutoIncrement = AutoIncrement;
	this->AutoKey = 0;
}

/* -- COLUMN CLTestRow --
   -- METHODS      -- */
void _Column::AddRow(_Data a) {
	if (!AutoIncrement)
		Rows.push_back(a);
	else {
		AutoKey++;
		Rows.push_back(_Data(AutoKey));
	}
}

_Data _Column::Get_Data(int Row) {
	return Rows[Row];
}


/* -- RELATION CLTestRow --
   -- CONSTRUCTORS   -- */
_Relation::_Relation(string Name) {
	this->Name = Name;
}

/* -- RELATION CLASS --
   -- METHODS       -- */

// Removes the Column designated with the index a
// ... Returns 0 if succeeded -1 if it did not
int _Relation::removeColumn(int a) {
	int c = Columns.size();
	Columns.erase(Columns.begin() + a);
	if (Columns.size() < c) {
		return 0;
	}
	else {
		return -1;
	}

}

string _Relation::GetName() {
	return Name;
}

void _Relation::AddColumn(_Column c) {
	Columns.push_back(c);
}

void _Relation::AddRow(vector<_Data> A) {

	for (int i = 0, a = 0; i < Columns.size(); i++)
	{
		Columns[i].AddRow(A[a]);
		if (!Columns[i].AutoIncrement)
			a++;
	}
}

_Data* _Relation::GetElementByKey(_Data* ID, int ColID)
{
	for (int i = 0; i < Columns[0].Rows.size(); i++)
	{
		if (Columns[0].Rows[i].Data == ID)
			return &Columns[ColID].Rows[i];
	}
}

void _Relation::RemoveRow(int Row) {
	for (int i = 0; i <= Columns[0].Rows.size(); ++i) {
		Columns[i].Rows.erase(Columns[i].Rows.begin() + Row);
	}
	cout << "Row " << Row + 1 << " removed.\n";
}

void _Relation::Print() {
	for (int i = 0; i < Columns.size(); i++)
	{
		for (int k = 0; k < Columns[0].Rows.size(); k++)
		{
			cout << Columns[i].Get_Data(k);
			cout << '\n';
		}
	}
}

vector<_Data> _Relation::GetRow(int index) {
	vector<_Data> TestRow;
	if (Columns.size() == 0) {
		cerr << "Error there are no columns.\n";
		return TestRow;
	}
	if (Columns[0].Rows.size() == 0) {
		cerr << "Error: there are no rows in the columns.\n";
		return TestRow;
	}
	for (int i = 0; i < Columns.size(); ++i) {
		TestRow.push_back(Columns[i].Rows[index]);
	}
	return TestRow;
}

vector<_Data> _Relation::CombineRows(vector<_Data> TestRow1, vector<_Data> TestRow2) {

	for (int i = 0; i < TestRow2.size(); ++i)
		TestRow1.push_back(TestRow2[i]);

	return TestRow1;
}

_Data _Relation::GetRow(int Column, int Row) {
	return Columns[Column].Get_Data(Row);
}