#include "Database.h"
#include "Parser.h"
#include "Relation.h"
#include "Condition.h"
#include <iostream>

using namespace std;


int main()
{
	Database DB;
	_Relation Table("Table");
	Table.AddColumn(_Column("ID", true, _Type::INT));
	Table.AddColumn(_Column("Name", false, _Type::VARCHAR));
	Table.AddColumn(_Column("Age", false, _Type::VARCHAR));

	/*vector<_Data> TestInput;
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

	DB.RemoveRow(1); //Remove input is offset by 1
	*/
	Parser P(&DB);

	P.input()
	DB.Print();
}