#include "Relation.h"

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

/* -- RELATION CLAS --
   -- METHODS       -- */
string _Relation::GetName() {
	return Name;
}

void _Relation::AddColumn(_Column c) {
	Columns.push_back(c);
}

void _Relation::AddRow(vector<_Data> A) {
	for (int i = 0, a = 0; i < Columns.size(); ++i) {
		Columns[i].AddRow(A[a]);
		if (!Columns[i].AutoIncrement)
			a++;
	}
}

void _Relation::RemoveRow(int Row) {
	for (int i = 0; i <= Columns[0].Rows.size(); ++i) {
		Columns[i].Rows.erase(Columns[i].Rows.begin() + Row);
	}
	cout << "Row " << Row + 1 << " removed.\n";
}

void _Relation::Print() {
	for (int i = 0; i < Columns.size(); ++i) {
		for (int k = 0; k < Columns[0].Rows.size(); ++k) {
			cout << Columns[i].Get_Data(k);
			cout << '\n';
		}
	}
}

vector<void*> _Relation::GetRow(int index) {
	vector<void*> TestRow;
	for(int i=0; i<=Columns[0].Rows.size(); ++i)
		TestRow.push_back(Columns[i].Rows[index].Data);
	return TestRow;
}

vector<void*> _Relation::CombineRows(int index1, int index2) {
	vector<void*> TestRow1 = GetRow(index1);
	vector<void*> TestRow2 = GetRow(index2);

	for(int i=0; i<TestRow2.size(); ++i)
		TestRow1.push_back(TestRow2[i]);

	return TestRow1;
}

_Data _Relation::GetRow(int Column, int Row) {
	return Columns[Column].Get_Data(Row);
}