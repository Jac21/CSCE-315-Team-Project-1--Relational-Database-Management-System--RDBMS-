#pragma once

#include "stdafx.h"
#include "Relation.h"
#include "Database.h"
#include "Condition.h"
#include "Parser_Redux.h"
#include <vector>
#include <string>
#include <algorithm> 
#include <functional> 
#include <cctype>
#include <locale>
#include <fstream>
#include <iomanip>
#include <iostream>

//Application entry point and DMS interface

Database PizzaDB;
Parser PizzaParser(&PizzaDB);
void Initialize();
void DirectAccess();
void PrintWelcome();
void CustomerManagementMain();
void EditCustomer();
void AddCustomer();
void AddPizza();
void AddCrust();
void AddTopping();
void AddCheese();
void PizzaManagementMain();
void OrderManagementMain();
void RemoveCustomer();


void Initialize()
{
	PizzaParser.Execute("CREATE TABLE customer (id INTEGER, name VARCHAR(64),phone VARCHAR(64) address VARCHAR(64));");
	PizzaParser.Execute("CREATE TABLE pizza (id INTEGER,name VARCHAR(64), toppingfk INTEGER, crustfk INTEGER, cheesefk INTEGER);");

	PizzaParser.Execute("CREATE TABLE crust (id INTEGER, name VARCHAR(64), cost INTEGER, calories INTEGER, gluten INTEGER, saturatedfat INTEGER);");
	PizzaParser.Execute("CREATE TABLE topping (id INTEGER, name VARCHAR(64), cost INTEGER, calories INTEGER, vegetarian INTEGER, saturatedfat INTEGER);");
	PizzaParser.Execute("CREATE TABLE cheese (id INTEGER, name VARCHAR(64), cost INTEGER, calories INTEGER, saturatedfat INTEGER);");

	PizzaParser.Execute("INSERT INTO customer VALUES FROM (\"Guy\", \"324-340-3495\", \"1235 fakestreet\")");
	PizzaParser.Execute("INSERT INTO customer VALUES FROM (\"Guy McSecond\", \"324-320-3562\", \"4559 street\")");
	PizzaParser.Execute("INSERT INTO crust VALUES FROM (\"Thin\", 250 , 800,1, 6)");
	PizzaParser.Execute("INSERT INTO crust VALUES FROM (\"Hand Tossed\", 300 , 1000,1, 10)");
	PizzaParser.Execute("INSERT INTO crust VALUES FROM (\"Deep Dish\", 400 , 1500,1, 7)");
	PizzaParser.Execute("INSERT INTO crust VALUES FROM (\"Brooklyn\", 300 , 1100,1, 6)");
	PizzaParser.Execute("INSERT INTO crust VALUES FROM (\"Gluten Free Hand Toss\", 350 , 900,0, 5)");

	PizzaParser.Execute("INSERT INTO topping VALUES FROM (\"Pepperoni\", 120 ,250,0, 20)");
	PizzaParser.Execute("INSERT INTO topping VALUES FROM (\"Onions\", 100 ,150,1, 2)");
	PizzaParser.Execute("INSERT INTO topping VALUES FROM (\"Peppers\", 100 ,105,1, 0)");
	PizzaParser.Execute("INSERT INTO topping VALUES FROM (\"Sausage\", 150 ,120,0, 10)");
	PizzaParser.Execute("INSERT INTO topping VALUES FROM (\"Pineapple\", 150 ,100,1, 2)");

	PizzaParser.Execute("INSERT INTO cheese VALUES FROM (\"Cheddar\", 100 ,200, 2)");
	PizzaParser.Execute("INSERT INTO cheese VALUES FROM (\"White\", 100 ,200, 2)");
}

int main()
{
	Initialize();
	system("CLS");
	PrintWelcome();
	getchar();
	system("CLS");
	for (;;)
	{
		cout << "CURRENTLY IN MAIN MENU" << endl;
		cout << "------------" << endl;
		cout << "1) Customer Management" << endl;
		cout << "2) Pizza Management" << endl;
		cout << "3) Order Management" << endl;
		cout << "4) Enter Direct Database Access:" << endl;
		cout << "5) Quit" << endl;
		cout << "------------" << endl;
		cout << "CHOICE: ";

		string Choice;
		getline(cin, Choice);
		system("CLS");
		switch (Choice[0])
		{
		case '1':
			CustomerManagementMain();
			break;
		case '2':
			PizzaManagementMain();
			break;
		case '3':
			OrderManagementMain();
			break;
		case '4':
			DirectAccess();
			break;
		case '5':
			return 0;
			break;
		default:
			cout << "\n\nERROR: CODE 496X: Invalid Command... Please try again.\n\n\n";
			break;
		}
	}
}
void DirectAccess()
{
	cout << "\n== Type'quit' to Leave ==\n";
	for (;;)
	{
		string input; 
		cout << "\nEntry Command/Query: ";
		getline(cin, input);
		PizzaParser.Execute(input);
		if (input == "quit")
			return;

	}
}


void CustomerManagementMain()
{
	for (;;)
	{
		cout << "CURRENTLY IN CUSTOMER MANAGEMENT" << endl;
		cout << "------------" << endl;
		cout << "1) View Customers" << endl;
		cout << "2) Add Customer" << endl;
		cout << "3) Edit Customer" << endl;
		cout << "4) Remove Customer" << endl;
		cout << "5) Quit" << endl;
		cout << "------------" << endl;
		cout << "CHOICE:";

		string Choice;
		getline(cin, Choice);
		system("CLS");
		switch (Choice[0])
		{
		case '1':
			PizzaDB.show("customer");
			break;
		case '2':
			AddCustomer();
			break;
		case '3':
			EditCustomer();
			break;
		case '4':
			RemoveCustomer();
			break;
		case '5':
			return;
			break;
		default:
			cout << "\n\nERROR: CODE 496X: Invalid Command... Please try again.\n\n\n";
			break;
		}
	}
}

void AddCustomer()
{
	string Name;
	string Phone;
	string Address;
	cout << "\n====================\n";
	cout << "===ADDING CUSTOMER==\n";
	cout << "====================\n";
	cout << "Insert Name: ";
	getline(cin, Name);
	cout << "\nInsert Phone: ";
	getline(cin, Phone);
	cout << "\nInsert Address: ";
	getline(cin, Address);

	PizzaParser.Execute("INSERT INTO customer VALUES FROM (\"" + Name + "\", \"" + Phone + "\", \"" + Address + "\")");
	cout << "\n Inserted" + Name + " " + Phone + " " + Address + "\n";
}

void EditCustomer()
{
	string Name;
	cout << "\n====================\n";
	cout << "===EDIT CUSTOMER==\n";
	cout << "====================\n";
	cout << "Insert Name of Customer to Edit: ";
	//QUERY WORK
}

void RemoveCustomer()
{
	string Name;
	PizzaDB.show("customer");
	cout << "\n====================\n";
	cout << "===ADDING CUSTOMER==\n";
	cout << "====================\n";
	cout << "Insert Name of customer to delete: ";
	getline(cin, Name);

	PizzaParser.Execute("DELETE FROM customer WHERE name == \"" + Name + "\"");

}

void PizzaManagementMain()
{
	for (;;)
	{
		cout << "CURRENTLY IN PIZZA MANAGEMENT" << endl;
		cout << "------------" << endl;
		cout << "1) Create Pizza Type" << endl;
		cout << "2) Create Cheese Type" << endl;
		cout << "3) Create Crust Type" << endl;
		cout << "4) Create Toppings Type" << endl;
		cout << "5) View Pizza Types" << endl;
		cout << "6) View Cheese Types" << endl;
		cout << "7) View Crust Types" << endl;
		cout << "8) View Toppings Types" << endl;
		cout << "9) Quit" << endl;
		cout << "------------" << endl;
		cout << "CHOICE:";

		string Choice;
		getline(cin, Choice);
		system("CLS");
		switch (Choice[0])
		{
		case '1':
			AddPizza();
			break;
		case '2':
			AddCheese();
			break;
		case '3':
			AddCrust();
			break;
		case '4':
			AddTopping();
			break;
		case '5':
			//ShowPizza(); this function needs to be made using queries. Pizza contains 
			break; //3 forign IDS. Lookup the ID and display the name for that item.
		case '6'://for example, a pizza with the cheesefk needs to look at the cheese
			PizzaDB.show("cheese");
			break; //relation for the cheese with the id of cheesefk etc.
		case '7':
			PizzaDB.show("crust");
			break;
		case '8':
			PizzaDB.show("topping");
			break;
		case '9':
			return;
			break;
		default:
			cout << "\n\nERROR: CODE 497X: Invalid Command... Please try again.\n\n\n";
			break;
		}
	}
}

void AddPizza()
{
	//"CREATE TABLE pizza (id INTEGER, toppingfk INTEGER, crustfk INTEGER, cheesefk INTEGER);"
	string Name;
	string ToppingFK;
	string CrustFK;
	string CheeseFK;
	cout << "\n====================\n";
	cout << "===ADDING PIZZA==\n";
	cout << "====================\n";
	cout << "\n What is the name of the pizza?\n";
	cout << "\nInsert Pizza Name: ";
	getline(cin, Name);
	PizzaDB.show("topping");
	cout << "\nInsert Topping ID: ";
	getline(cin, ToppingFK);
	PizzaDB.show("crust");
	cout << "\nInsert Crust ID: ";
	getline(cin, CrustFK);
	PizzaDB.show("cheese");
	cout << "\nInsert Cheese ID: ";
	getline(cin, CheeseFK);

	PizzaParser.Execute("INSERT INTO pizza VALUES FROM (\"" + Name + "\", " + ToppingFK + "," + CrustFK + "," + CheeseFK + ")");
	cout << "\n Inserted" + ToppingFK + CrustFK +  CheeseFK + "\n";
}

void AddTopping()
{
	//"CREATE TABLE topping (id INTEGER, name VARCHAR(64), cost INTEGER, calories INTEGER, vegetarian INTEGER);"
	string Name;
	string Cost;
	string Calories;
	string Vegetarian;
	cout << "\n====================\n";
	cout << "===ADDING TOPPING==\n";
	cout << "====================\n";
	cout << "\nInsert name: ";
	getline(cin, Name);
	cout << "\nInsert cost: ";
	getline(cin, Cost);
	cout << "\nInsert calories: ";
	getline(cin, Calories);
	cout << "\nInsert vegetarian (1 or 0): ";
	getline(cin, Vegetarian);

	PizzaParser.Execute("INSERT INTO topping VALUES FROM (\"" + Name + "\"," + Cost + "," + Calories + "," + Vegetarian + ")");
	cout << "\n Inserted" + Name + " " + Cost + " " + Calories + "\n";
}

void AddCrust()
{
	//"CREATE TABLE crust (id INTEGER, name VARCHAR(64), cost INTEGER, calories INTEGER, glutten INTEGER, saturatedfat INTEGER);"
	string Name;
	string Cost;
	string Calories;
	string Gluten;
	string SaturatedFat;
	cout << "\n====================\n";
	cout << "===ADDING CRUST==\n";
	cout << "====================\n";
	cout << "\nInsert name: ";
	getline(cin, Name);
	cout << "\nInsert cost: ";
	getline(cin, Cost);
	cout << "\nInsert calories: ";
	getline(cin, Calories);
	cout << "\nInsert Gluten (0 or 1): ";
	getline(cin, Gluten);
	cout << "\nInsert SaturatedFat (0 or 1): ";
	getline(cin, SaturatedFat);

	PizzaParser.Execute("INSERT INTO crust VALUES FROM (\"" + Name + "\"," + Cost + "," + Calories + "," + Gluten + "," + SaturatedFat + ")");
	cout << "\n Inserted" + Name + " " + Cost + " " + Calories + " " + Gluten + " " + SaturatedFat + "\n";
}

void AddCheese()
{
	//"CREATE TABLE cheese (id INTEGER, name VARCHAR(64), cost INTEGER, calories INTEGER);"
	string Name;
	string Cost;
	string Calories;
	cout << "\n====================\n";
	cout << "===ADDING CHEESE==\n";
	cout << "====================\n";
	cout << "\nInsert name: ";
	getline(cin, Name);
	cout << "\nInsert cost: ";
	getline(cin, Cost);
	cout << "\nInsert calories: ";
	getline(cin, Calories);

	PizzaParser.Execute("INSERT INTO crust VALUES FROM (\"" + Name + "," + Cost + "," + Calories + ")");
	cout << "\n Inserted" + Name + " " + Cost + " " + Calories + "\n";
}

void OrderManagementMain()
{
	for (;;)
	{
		cout << "CURRENTLY IN ORDER MANAGEMENT" << endl;
		cout << "------------" << endl;
		cout << "1) Place Order" << endl;
		cout << "2) Update Order" << endl;
		cout << "3) Remove Order" << endl;
		cout << "4) Quit" << endl;
		cout << "------------" << endl;
		cout << "CHOICE:";

		string Choice;
		getline(cin, Choice);
		system("CLS");
		switch (Choice[0])
		{
		case '1':
			//CustomerManagementMain();
			break;
		case '2':
			//FinancesManagementMain();
			break;
		case '3':
			break;
		case '4':
			return;
			break;
		default:
			cout << "\n\nERROR: CODE 498X: Invalid Command... Please try again.\n\n\n";
			break;
		}
	}
}

void PrintWelcome()
{
	cout <<  "'||''|.   ||                                                    ||  '        " << endl;
	cout << " ||   || ...  ......  ......   ....   ... ..    ...   .. ...   ...     ....  " << endl;
	cout << " ||...|'  ||  '  .|'  '  .|'  '' .||   ||' '' .|  '|.  ||  ||   ||    ||. '  " << endl;
	cout << " ||       ||   .|'     .|'    .|' ||   ||     ||   ||  ||  ||   ||    . '|.. " << endl;
	cout << ".||.     .||. ||....| ||....| '|..'|' .||.     '|..|' .||. ||. .||.   |'..|' " << endl;
	cout << "                                                                             " << endl;
	cout << "                                                                             " << endl;
	cout << "'||''|.   ||                                                                 " << endl;
	cout << " ||   || ...  ......  ......   ....                                          " << endl;
	cout << " ||...|'  ||  '  .|'  '  .|'  '' .||                                         " << endl;
	cout << " ||       ||   .|'     .|'    .|' ||                                         " << endl;
	cout << ".||.     .||. ||....| ||....| '|..'|'                                        " << endl;
	cout << "                                                                             " << endl;
	cout << "                                                                             " << endl;

	cout << "\nWelcome to Pizzaroni's Pizzria\n";
	cout << "Press -ENTER- To enter the DMS\n";
	cout << "------------------------------\n";
}