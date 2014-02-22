#include "Database.h"

/*--------------------------------------|
|	Author: Jeffrey LeRoy				|
|	Group: Team C						|
|	Last Edited: 2/10/14				|
|	Assignment: RDBMS Project			|
|--------------------------------------*/

/* Function that creates a new relation
...From the condition using the relationship passed */

_Relation* Database::getRelation(string Name)
{
	for (int i = 0; i < allRelations.size(); i++)
	if (allRelations[i].Name == Name)
		return &allRelations[i];
	return NULL;
}

_Relation Database::selection(Condition& c, _Relation r) {
	_Relation r2 = c.evaluate(&r);
	return r2;
}

_Relation Database::projection(vector<String> attributes, _Relation r) {

	vector<_Column> rAttributes;
	for (int i = 0; i < r.Columns.size(); ++i)
		rAttributes.push_back(r.Columns[i]);
	_Relation newRelation("sudo");

	for (int i = 0; i < rAttributes.size(); i++) {

		for (int j = 0; j < attributes.size(); j++) {

			if (attributes[i] == rAttributes[j].Name) {

				newRelation.AddColumn(r.Columns[j]);

			} // end if statement

		} // end inner for loop

	} // end outer for loop

	//newRelation.deleteDuplicateAttributes();

	return newRelation;

} // end projection(vector<string>, Relation&)

_Relation Database::setDifference(_Relation& a, _Relation& b) {
	vector<_Column> aAttributes;
	vector<_Column> bAttributes;
	for (int i = 0; i < a.Columns.size(); ++i)
		aAttributes.push_back(a.Columns[i]);
	for (int i = 0; i < b.Columns.size(); ++i)
		bAttributes.push_back(b.Columns[i]);
	_Relation newRelation("sudo");
	bool okAdd = false;

	if (aAttributes.size() != bAttributes.size()) {

		cerr << "Error, these two relations are not difference compatible.";

	} // end if statement 

	for (int i = 0; i < aAttributes.size(); i++) {

		if (aAttributes[i].Rows[i].Type != bAttributes[i].Rows[i].Type) {

			cerr << "Error, these two relations are not difference compatible.";

		} // end if else statement 

		newRelation.AddColumn(aAttributes[i]);

	} // end for loop with i

	//vector<Row> aRows = a.getRowList();
	//vector<Row> bRows = b.getRowList();

	for (int j = 0; j < aAttributes[0].Rows.size(); j++) {

		for (int k = 0; k < bAttributes[0].Rows.size(); k++) {
			for (int z = 0; z < aAttributes.size(); ++z) {
				if (aAttributes[z].Rows[j].Data != bAttributes[z].Rows[j].Data) {

					okAdd = true;

				}
				else {

					okAdd = false;
					break;

				} // end if else statement
			}
		} // end for loop with k 

		if (okAdd == true) {
			vector<_Data> dt;
			for (int i = 0; i < aAttributes.size(); ++i) {
				dt.push_back(aAttributes[i].Get_Data(j));
				newRelation.AddRow(dt);

			} // end if statement 

			okAdd = false;

		} // end for loop with j

		return newRelation;

	} // end setDifference(Relation&, Relation&)
}

void Database::renaming(vector<String> renames, _Relation& r) {
	vector<_Column> rAttributes;
	for (int i = 0; i < r.Columns.size(); ++i)
		rAttributes.push_back(r.Columns[i]);

	if (renames.size() == rAttributes.size()) {

		for (int i = 0; i < rAttributes.size(); i++) {

			r.Columns[i].Name == rAttributes[i].Name;

		} // end for loop

	}
	else {

		cout << "Error, not enough strings read in to reaname all attributes.";

	} // end if else statement

} // end renaming(vector<string>, Relation&)

_Relation Database::setUnion(_Relation a, _Relation b)
{
	vector<_Column> aAttributes;
	vector<_Column> bAttributes;
	for (int i = 0; i < a.Columns.size(); ++i)
		aAttributes.push_back(a.Columns[i]);
	for (int i = 0; i < b.Columns.size(); ++i)
		bAttributes.push_back(b.Columns[i]);
	_Relation newRelation("sudo");
	vector<_Column> newAttribute;

	if (aAttributes.size() != bAttributes.size()) {

		cerr << "Error, these two relations are not union compatible.";

	} // end if statement 

	for (int i = 0; i < aAttributes.size(); i++) {

		if (aAttributes[i].Rows[i].Type == bAttributes[i].Rows[i].Type) {

			newAttribute.push_back(aAttributes[i]);
			newAttribute.push_back(bAttributes[i]);

			if ((i + 1) == aAttributes.size()) {
				for (int i = 0; i < newAttribute.size(); ++i)
					newRelation.AddColumn(newAttribute[i]);
				newAttribute.push_back(_Column());

			} // end if statement 

		}
		else {

			cerr << "Error, these two relations are not union compatible.";

		} // end if else statement 

	} // end for loop 

	//newRelation.deleteDuplicateRows();
	return newRelation;

} // end setUnion(Relation&, Relation&)

/* Function that creates a relation
...with the specified name, attributes, and keys */
void Database::createTable(String name, Vector<_Column> attributes) {
	_Relation r = _Relation(name);
	for (int i = 0; i < attributes.size(); ++i) {
		r.AddColumn(attributes[i]);
	}
	allRelations.push_back(r);
}

/* Deletes the relation from the database
..with the same name as Relation r */
void Database::dropTable(_Relation r) {
	for (int i = 0; i < allRelations.size(); ++i) {
		if (allRelations[i].GetName() == r.GetName()) {
			allRelations.erase(allRelations.begin() + i);
		}
	}
}

/* Add a row to the Relation
...with the smme name as name*/
void Database::insertInto(String name, Vector<String> literals) {
	vector<_Data> d;
	for (int i = 0; i < literals.size(); ++i) {
		_Data dt = _Data(literals[i]);
		d.push_back(i);
	}

	for (int i = 0; i < allRelations.size(); ++i) {
		if (allRelations[i].GetName() == name) {
			allRelations[i].AddRow(d);
		}
	}
}

/* After evaluating the expression and giving a relation
...Takes the relation and adds it to the relation
...with the given name.							    */
void Database::insertInto(String name, _Relation& r) {
	for (int i = 0; i < allRelations.size(); ++i) {
		if (allRelations[i].GetName() == name) {
			for (int i = 0; i < r.Columns.size(); ++i) {
				vector<_Data> d;
				for (int j = 0; j < r.Columns[i].Rows.size(); ++j) {
					d.push_back(r.GetRow(i, j));
				}
				allRelations[i].AddRow(d);
			}
		}
	}
}

// Outputs the Relation with the matching Name
void Database::show(String name) {
	int i;
	for (i = 0; i < allRelations.size(); ++i) {
		if (allRelations[i].GetName() == name) {
			break;
		}
	}
	cout << "Relation: " << allRelations[i].GetName() << endl;
	cout << "---------------------------------------------------------\n\n";
	allRelations[i].Print();
}

/* Updates an attribute that meets the condition
...With its new value						*/
void Database::update(String rname, vector<String> attributes, vector<_Data> newvalues, Condition& c)
{
	int indr = 0;
	for (indr = 0; indr < allRelations.size(); ++indr)
	{
		if (allRelations[indr].GetName() == rname)
			break;
	}

	_Relation r2 = c.evaluate(&allRelations[indr]);
	for (int i = 0; i < r2.Columns[0].Rows.size(); ++i) {
		for (int j = 0; j < allRelations[indr].Columns[0].Rows.size(); ++j) {
			bool upd = true;
			for (int k = 0; k < allRelations[indr].Columns.size(); ++i) {
				if (r2.Columns[k].Rows[i].Data != allRelations[indr].Columns[k].Rows[j].Data) {
					upd = false;
					break;
				}
			}
			if (upd == true) {
				for (int y = 0; y < attributes.size(); ++y) {
					for (int z = 0; z < allRelations[indr].Columns.size(); ++z) {
						if (attributes[y] == allRelations[indr].Columns[z].Name) {
							allRelations[indr].Columns[z].Rows[j] = newvalues[y];
						}
					}
				}
			}
		}
	}
}
/* Deletes elements from the relation named Name
...That meet the condition */
void Database::deleteFrom(String name, Condition& c) {
	int i;
	for (i = 0; i < allRelations.size(); ++i){
		if (allRelations[i].GetName() == name)
			break;
	}

	_Relation r2 = c.evaluate(&allRelations[i]);
	for (int j = 0; i < r2.Columns[0].Rows.size(); ++i) {
		for (int k = 0; k < allRelations[i].Columns[0].Rows.size(); ++k) {
			bool del = true;
			for (int z = 0; z < allRelations[i].Columns.size(); ++z) {
				if (r2.Columns[z].Rows[j].Data != allRelations[i].Columns[z].Rows[k].Data) {
					del = false;
					break;
				}
			}
			if (del == true) {
				allRelations[i].RemoveRow(k);
				break;
			}
		}
	}
}

// Computes the cross product of the two relations
_Relation* Database::crossProduct(_Relation& a, _Relation& b) {
	vector<vector<_Data> > aRows;
	vector<vector<_Data> > bRows;
	int i, j;
	_Relation* newRelation = new _Relation("sudo");
	for (i = 0; i < a.Columns.size(); i++) {
		newRelation->AddColumn(_Column(a.Columns[i].Name, a.Columns[i].AutoIncrement, a.Columns[i].Type));
	} // end for loop
	for (j = 0; j < b.Columns.size(); j++) {
		newRelation->AddColumn(_Column(b.Columns[j].Name, b.Columns[j].AutoIncrement, b.Columns[j].Type));
	} // end for loop
	for (i = 0; i < a.Columns[0].Rows.size(); i++) {
		//vector<_Data> blah = a.GetRow(i);
		//aRows.push_back(a.GetRow(i));			NEED TO FIX CONVERSION ERROR
	} // end for loop 
	for (j = 0; j < b.Columns[0].Rows.size(); j++) {
		//bRows.push_back(b.GetRow(j));			NEED TO FIX CONVERSION ERROR
	} // end for loop
	vector<_Data> combinedRow;
	for (int k = 0; k < aRows.size(); k++) {
		for (int l = 0; l < bRows.size(); l++) {
			//combinedRow = a.CombineRows(aRows[k], bRows[l]);		NEED TO FIX CONVERSION ERROR
			//newRelation->AddRow(combinedRow);
		} // end for loop for l
	} // end for loop for k
	return newRelation;
} // end crossProduct(Relation&, Relation&)

// Helper function that gets the Columns to go in the natural Join Relation
vector<_Column>* getColumns(_Relation tempRelation, _Relation* crossRelation) {
	vector<_Column>* columns = new vector<_Column>();
	for (int i = 0; i < tempRelation.Columns.size(); ++i) {
		bool addCommon = false;
		int j;
		for (j = i + 1; j < tempRelation.Columns.size(); ++j) {
			if (tempRelation.Columns[i].Name == tempRelation.Columns[j].Name) {
				addCommon = true;
				break;
			}
		}
		if (addCommon) {
			columns->push_back(_Column(crossRelation->Columns[i].Name,
				crossRelation->Columns[i].AutoIncrement,
				crossRelation->Columns[i].Type));
			if (tempRelation.removeColumn(j) == -1) {
				cerr << "Error: Column could not be removed in natural join.\n";
				return NULL;
			}
		}
		else {
			columns->push_back(_Column(crossRelation->Columns[i].Name,
				crossRelation->Columns[i].AutoIncrement,
				crossRelation->Columns[i].Type));
		}
	}
	return columns;
}

// Add the Columns to the Relation
void addColumnsToNJ(_Relation* naturalJoin, vector<_Column>* columns) {
	if (columns != NULL) {
		for (int i = 0; i < columns->size(); ++i) {
			naturalJoin->AddColumn(columns->at(i));
		}
	}
	else {
		cerr << "Error: Get Columns Failed.\n";
	}
}
// Compare Rows to see if it is able to be added
// Returns false if it is not to be added  true if it is
bool compareRow(_Relation* crossRelation, int i) {
	for (int j = 0; j < crossRelation->Columns.size(); ++j) {
		for (int k = j + 1; k < crossRelation->Columns.size(); ++k) {
			if (crossRelation->Columns[j].Name == crossRelation->Columns[k].Name) {
				if (crossRelation->Columns[j].Type == crossRelation->Columns[k].Type
					&& crossRelation->Columns[k].Type == _Type::INT) {
					int p = *((int*)crossRelation->Columns[j].Rows[i].Data);
					int p2 = *((int*)crossRelation->Columns[k].Rows[i].Data);
					if (p != p2) {
						return false;
					}
				}
				if (crossRelation->Columns[j].Type == crossRelation->Columns[k].Type
					&& crossRelation->Columns[k].Type == _Type::VARCHAR) {
					string* p = (string*)crossRelation->Columns[j].Rows[i].Data;
					string* p2 = (string*)crossRelation->Columns[k].Rows[i].Data;
					if (*p != *p2) {
						return false;
					}
				}
				break;
			}
		}
	}
	return true;
}

// Checks if the string has already been used
bool notUsed(string k, vector<string> a) {
	for (int i = 0; i < a.size(); ++i) {
		if (a[i] == k) {
			return false;
		}
	}
	return true;
}

// Adds the row i from crossRelation to the relation naturalJoin
void addRowToNJ(_Relation* naturalJoin, _Relation* crossRelation, int z) {
	vector<string> alreadyused;
	for (int i = 0; i < naturalJoin->Columns.size(); ++i) {
		for (int j = 0; j < crossRelation->Columns.size(); ++j) {
			if (naturalJoin->Columns[i].Name == crossRelation->Columns[j].Name) {
				if (notUsed(naturalJoin->Columns[i].Name, alreadyused)) {
					naturalJoin->Columns[i].Rows.push_back(crossRelation->Columns[j].Rows[z]);
					alreadyused.push_back(naturalJoin->Columns[i].Name);
				}
				break;
			}
		}
	}
}

// Adds the rows supposed to go into the natural join relation
void addRows(_Relation* naturalJoin, _Relation* crossRelation) {
	vector<string> names;
	for (int i = 0; i < crossRelation->Columns.size(); ++i) {
		names.push_back(crossRelation->Columns[i].Name);
	}
	for (int i = 0; i < crossRelation->Columns[0].Rows.size(); ++i) {
		bool addRow;
		addRow = compareRow(crossRelation, i);
		if (addRow) {
			addRowToNJ(naturalJoin, crossRelation, i);
		}
	}
}

// Natural Join of two Relations/Tables
_Relation* Database::naturalJoin(_Relation& a, _Relation& b) {
	_Relation* crossRelation = crossProduct(a, b);
	_Relation tempRelation = *(crossRelation);
	_Relation* naturalJoin = new _Relation("Sudo");
	vector<_Column>* columns;
	columns = getColumns(tempRelation, crossRelation);
	addColumnsToNJ(naturalJoin, columns);
	addRows(naturalJoin, crossRelation);
	delete crossRelation;
	return naturalJoin;
}