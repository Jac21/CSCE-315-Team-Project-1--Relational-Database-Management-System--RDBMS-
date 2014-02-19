#include "Database.h"
#include "Relation.h"
#include "Condition.h"
#include <iostream>

using namespace std;
#include "Database.h"

int main()
{
	char exit;
	Database database = Database();
	string pizzaS = "Pizza Store";
	vector<_Column> columnsP;
	columnsP.push_back(_Column("Type of Pizza", false , _Type::VARCHAR));
	columnsP.push_back(_Column("Pizza ID", false, _Type::INT));
	columnsP.push_back(_Column("Store ID", false, _Type::INT));
	columnsP.push_back(_Column("Store Address", false , _Type::VARCHAR));
	columnsP.push_back(_Column("Whatever",false,_Type::VARCHAR));
	vector<string> keysP;
	keysP.push_back("Type of Pizza");
	keysP.push_back("Store ID");
	database.createTable(pizzaS, columnsP);
	vector<_Data> pizzaD;
	pizzaD.push_back(_Data("Pepperoni"));
	pizzaD.push_back(_Data(1));
	pizzaD.push_back(_Data(1));
	pizzaD.push_back(_Data("5426 Glen Vista Dr"));
	pizzaD.push_back(_Data("shumadong"));
	vector<_Data> pizzaD1;
	pizzaD1.push_back(_Data("Sausage"));
	pizzaD1.push_back(_Data(2));
	pizzaD1.push_back(_Data(1));
	pizzaD1.push_back(_Data("5426 Glen Vista Dr"));
	pizzaD1.push_back(_Data("shumadong"));
	vector<_Data> pizzaD2;
	pizzaD2.push_back(_Data("Ham"));
	pizzaD2.push_back(_Data(3));
	pizzaD2.push_back(_Data(2));
	pizzaD2.push_back(_Data("4309 College Main St"));
	pizzaD2.push_back(_Data("shumadong"));
	vector<_Data> pizzaD3;
	pizzaD3.push_back(_Data("Bacon"));
	pizzaD3.push_back(_Data(4));
	pizzaD3.push_back(_Data(2));
	pizzaD3.push_back(_Data("4309 College Main St"));
	pizzaD3.push_back(_Data("shumadong"));
	vector<_Data> pizzaD4;
	pizzaD4.push_back(_Data("Pepperoni"));
	pizzaD4.push_back(_Data(1));
	pizzaD4.push_back(_Data(1));
	pizzaD4.push_back(_Data("5426 Glen Vista Dr"));
	pizzaD4.push_back(_Data("shumadong"));
	database.insertInto(pizzaS, pizzaD);
	database.insertInto(pizzaS, pizzaD1);
	database.insertInto(pizzaS, pizzaD2);
	database.insertInto(pizzaS, pizzaD3);
	database.insertInto(pizzaS, pizzaD4);
	database.show(pizzaS);

	string pizzaS2 = "Second Pizza Store";
	vector<_Column> columnsP2;
	columnsP2.push_back(_Column("Type of Pizza", false, _Type::VARCHAR));
	columnsP2.push_back(_Column("Pizza ID", false, _Type::INT));
	columnsP2.push_back(_Column("Store ID", false, _Type::INT));
	columnsP2.push_back(_Column("Store Address", false, _Type::VARCHAR));
	vector<string> keysP2;
	keysP2.push_back("Type of Pizza");
	keysP2.push_back("Store ID");
	database.createTable(pizzaS2, columnsP2);
	vector<_Data> pizzaD5;
	pizzaD5.push_back(_Data("Pepperoni"));
	pizzaD5.push_back(_Data(1));
	pizzaD5.push_back(_Data(1));
	pizzaD5.push_back(_Data("5426 Glen Vista Dr"));
	database.insertInto(pizzaS2, pizzaD4);
	vector<_Data> pizzaD6;
	pizzaD6.push_back(_Data("Macaroni"));
	pizzaD6.push_back(_Data(6));
	pizzaD6.push_back(_Data(2));
	pizzaD6.push_back(_Data("4309 College Main St"));
	database.insertInto(pizzaS2, pizzaD5);
	database.show(pizzaS2);
	_Relation r = *(database.naturalJoin(*(database.getRelation(pizzaS)), *(database.getRelation(pizzaS2))));
	database.addRelation(r);
	database.show("sudo");
	cin >> exit;
}