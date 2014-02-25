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
void PizzaManagementMain();
void FinancesManagementMain();
void RemoveCustomer();


void Initialize()
{
	PizzaParser.Execute("CREATE TABLE customer (id INTEGER, name VARCHAR(64),phone VARCHAR(64) address VARCHAR(64));");
	PizzaParser.Execute("CREATE TABLE pizza (id INTEGER, toppingfk INTEGER, crustfk INTEGER, cheesefk INTEGER);");
	PizzaParser.Execute("CREATE TABLE crust (id INTEGER, name VARCHAR(64), cost INTEGER, calories INTEGER, glutten INTEGER, saturatedfat INTEGER);");
	PizzaParser.Execute("CREATE TABLE topping (id INTEGER, name VARCHAR(64), cost INTEGER, calories INTEGER, vegetarian INTEGER, saturatedfat INTEGER);");
	PizzaParser.Execute("CREATE TABLE cheese (id INTEGER, name VARCHAR(64), cost INTEGER, calories INTEGER, saturatedfat INTEGER);");

	PizzaParser.Execute("INSERT INTO customer VALUES FROM (\"Guy McThird\", \"324-340-3495\", \"1235 fakestreet\")");
	PizzaParser.Execute("INSERT INTO customer VALUES FROM (\"Guy McSecond\", \"324-320-3562\", \"4559 street\")");
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
		cout << "3) Finances" << endl;
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
			FinancesManagementMain();
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

	PizzaParser.Execute("DELETE FROM customer WHERE (name == \"" + Name + "\")");

}

void PizzaManagementMain()
{
	for (;;)
	{
		cout << "CURRENTLY IN PIZZA MANAGEMENT" << endl;
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
			//PizzaManagementMain();
			break;
		case '3':
			//FinancesManagementMain();
			break;
		case '4':
			return;
			break;
		default:
			cout << "\n\nERROR: CODE 497X: Invalid Command... Please try again.\n\n\n";
			break;
		}
	}
}

void FinancesManagementMain()
{
	for (;;)
	{
		cout << "CURRENTLY IN FINANCIAL MANAGEMENT" << endl;
		cout << "------------" << endl;

		cout << "1) Add Transaction" << endl;
		cout << "2) Update Transaction" << endl;
		cout << "3) Remove Transaction" << endl;
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