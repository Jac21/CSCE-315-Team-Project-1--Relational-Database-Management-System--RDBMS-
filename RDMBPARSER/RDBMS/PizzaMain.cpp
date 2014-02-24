#pragma once

#include "stdafx.h"
#include "Relation.h"
#include "Database.h"
#include "Condition.h"
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

void PrintWelcome();
void CustomerManagementMain();
void PizzaManagementMain();
void FinancesManagementMain();

/*
int main()
{
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
		cout << "4) Quit" << endl;
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
			return 0;
			break;
		default:
			cout << "\n\nERROR: CODE 496X: Invalid Command... Please try again.\n\n\n";
			break;
		}
	}
}
*/ 
void CustomerManagementMain()
{
	for (;;)
	{
		cout << "CURRENTLY IN CUSTOMER MANAGEMENT" << endl;
		cout << "------------" << endl;
		cout << "1) Add Customer" << endl;
		cout << "2) Edit Customer" << endl;
		cout << "3) Remove Customer" << endl;
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
			cout << "\n\nERROR: CODE 496X: Invalid Command... Please try again.\n\n\n";
			break;
		}
	}
}

void PizzaManagementMain()
{
	for (;;)
	{
		cout << "CURRENTLY IN PIZZA MANAGEMENT" << endl;
		cout << "------------" << endl;
		cout << "1) Place Order" << endl;
		cout << "2) Edit Order" << endl;
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
		cout << "2) Edit Transaction" << endl;
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
			//PizzaManagementMain();
			break;
		case '3':
			//FinancesManagementMain();
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