#include "stdafx.h"
#include "CppUnitTest.h"
#include "..\RDBMS\Relation.h"
#include "..\RDBMS\Database.h"
#include "..\RDBMS\Condition.h"

using namespace Microsoft::VisualStudio::CppUnitTestFramework;

namespace SUPERTEST
{		
	TEST_CLASS(UnitTest1)
	{
	public:
		TEST_METHOD(TestMethod1)
		{
			Database DB;
			
			//_Relation DB("DBTest");
			/*
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

			DB.RemoveRow(1); //Remove input is offset by 1

			DB.Print();*/

		}

	};
}