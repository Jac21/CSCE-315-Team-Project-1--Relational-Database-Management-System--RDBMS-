#include "Database.h"

/*--------------------------------------|
|	Author: Jeffrey LeRoy				|
|	Group: Team C						|
|	Last Edited: 2/10/14				|
|	Assignment: RDBMS Project			|
|--------------------------------------*/

/* Function that creates a new relation
...From the condition using the relationship passed */

using namespace std; 

void Database::addRelation(_Relation r) {
	allRelations.push_back(r);
}

_Relation* Database::getRelation(string name) {
	_Relation* r;
	for (int i = 0; i < allRelations.size(); ++i) {
		if (allRelations[i].Name == name) {
			r = &allRelations[i];
			return r;
		}
	}
	cerr << "Error: Relation not found.\n";
}

_Relation* Database::selection(Condition& c, _Relation r) {
	_Relation *r2 = c.evaluate(&r);
	return r2;
}

_Relation* Database::projection(vector<string> attributes, _Relation r) {
	vector<_Column> rAttributes;
	for (int i = 0; i < r.Columns.size(); ++i)
		rAttributes.push_back(r.Columns[i]);
	_Relation *newRelation = new _Relation("sudo");
	if (attributes.size() == 0 || rAttributes.size() == 0) {
		cerr << "Error: attributes is equal to 0\n";
	}
	else {
		for (int i = 0; i < attributes.size(); i++) {
			for (int j = 0; j < rAttributes.size(); j++) {
				if (attributes[i] == rAttributes[j].Name) {
					newRelation->AddColumn(r.Columns[j]);
				} // end if statement
			} // end inner for loop
		} // end outer for loop
		return newRelation;
	}
} // end projection(vector<string>, Relation&)

_Relation* Database::setDifference(_Relation& a, _Relation& b) {
	_Relation *newRelation = new _Relation("sudo");
	for (int i = 0; i < a.Columns.size(); ++i) {
		newRelation->AddColumn(_Column(a.Columns[i].Name, a.Columns[i].AutoIncrement, a.Columns[i].Type));
	}
	if (a.Columns.size() != b.Columns.size()) {
		cerr << "Error: The two relations are not difference compatible.\n";
		return newRelation;
	}
	for (int i = 0; i < a.Columns.size(); ++i) {
		if (a.Columns[i].Type != b.Columns[i].Type) {
			cerr << "Error: The two relations are not difference compatible.\n";
			return newRelation;
		}
	}
	for (int i = 0; i < a.Columns[0].Rows.size(); ++i){
		bool okAdd = true;
		for (int j = 0; j < b.Columns[0].Rows.size(); ++j){
			int z = 0;
			for (int k = 0; k < a.Columns.size(); ++k) {
				++z;
				if (a.Columns[k].Type == _Type::INT) {
					int *p = (int*)a.Columns[k].Rows[i].Data;
					int* p2 = (int*)b.Columns[k].Rows[j].Data;
					if (*p != *p2) {
						break;
					}
				}
				else {
					string *p = (string*)a.Columns[k].Rows[i].Data;
					string *p2 = (string*)b.Columns[k].Rows[j].Data;
					if (*p != *p2) {
						break;
					}
				}
			}
			if (z == a.Columns.size()) {
				okAdd = false;
			}
		}
		if (okAdd == true) {
			newRelation->AddRow(a.GetRow(i));
		}
	}
	return newRelation;
}

void Database::renaming(vector<string> renames, _Relation* r) {
	vector<_Column> rAttributes;
	if (renames.size() == renames.size()) {
		for (int i = 0; i < renames.size(); i++) {
			r->Columns[i].Name = renames[i];
		} // end for loop
	}
	else {
		cout << "Error, not enough strings read in to reaname all attributes.";
	} // end if else statement
} // end renaming(vector<string>, Relation&)

_Relation* Database::setUnion(_Relation a, _Relation b) {
	_Relation *newRelation = new _Relation("sudo");
	for (int i = 0; i < a.Columns.size(); ++i) {
		newRelation->AddColumn(_Column(a.Columns[i].Name, a.Columns[i].AutoIncrement, a.Columns[i].Type));
	}
	if (a.Columns.size() != b.Columns.size()) {
		cerr << "Error: The two relations are not union compatible.\n";
		return newRelation;
	}
	for (int i = 0; i < a.Columns.size(); ++i) {
		if (a.Columns[i].Type != b.Columns[i].Type) {
			cerr << "Error: The two relations are not union compatible.\n";
			return newRelation;
		}
	}
	for (int i = 0; i < a.Columns[0].Rows.size(); ++i) {
		newRelation->AddRow(a.GetRow(i));
	}
	for (int i = 0; i < b.Columns[0].Rows.size(); ++i) {
		newRelation->AddRow(b.GetRow(i));
	}
	return newRelation;
} // end setUnion(Relation&, Relation&)

/* Function that cretes a relation
...with the specified name, attributes, and keys */
void Database::createTable(string name, vector<_Column> attributes) {
	_Relation r = _Relation(name);
	for (int i = 0; i < attributes.size(); ++i) {
		r.AddColumn(attributes[i]);
	}
	allRelations.push_back(r);
}

/* Deletes the relation from the database
..with the same name as Relation r */
void Database::dropTable(_Relation r) {
	for(int i=0; i < allRelations.size(); ++i) {
		if(allRelations[i].GetName() == r.GetName()) {
			allRelations.erase(allRelations.begin() + i);
		}
	}
}

/* Add a row to the Relation
...with the smme name as name*/
void Database::insertInto(string name, vector<_Data> literals) {
	int i;
	for(i=0; i < allRelations.size(); ++i) {
		if(allRelations[i].GetName() == name) {
			break;
		}
	}
	allRelations[i].AddRow(literals);
}

/* After evaluating the expression and giving a relation
...Takes the relation and adds it to the relation
...with the given name.							    */
void Database::insertInto(string name, _Relation& r) {
	for(int i=0; i<allRelations.size(); ++i) {
		if(allRelations[i].GetName() == name) {
			for(int j=0; j<r.Columns[0].Rows.size(); ++j) {
				vector<_Data> d;
				for (int k = 0; k < r.Columns.size(); ++k) {
					d.push_back(r.Columns[k].Rows[j]);
				}
				allRelations[i].AddRow(d);
			}
		}
	}		
}

// Outputs the Relation with the matching Name
void Database::show(string name) {
	int i;
	for (i = 0; i < allRelations.size(); ++i) {
		if (allRelations[i].GetName() == name) {
			cout << setw(45) << allRelations[i].GetName() << endl << endl;
			allRelations[i].Print();
			break;
		}
	}

}

/* Updates an attribute that meets the condition
...With its new value						*/
void Database::update(string rname, vector<string> attributes, vector<_Data> newvalues, Condition& c)
{
	int indr= 0;
	for(indr = 0; indr < allRelations.size(); ++indr)
	{
		if(allRelations[indr].GetName() == rname) 
			break;
	}

	_Relation r2 = *(c.evaluate(&allRelations[indr]));
	if (r2.Columns.size() == 0) {
		cerr << "Error: size of columns is equal to 0\n";
	}
	else {
		for (int i = 0; i < r2.Columns[0].Rows.size(); ++i) {
			for (int j = 0; j < allRelations[indr].Columns[0].Rows.size(); ++j) {
				bool upd = true;
				for (int k = 0; k < allRelations[indr].Columns.size(); ++k) {
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
}

/* Deletes elements from the relation named Name
...That meet the condition */
void Database::deleteFrom(string name, Condition& c) {
	int i;
	for (i = 0; i < allRelations.size(); ++i){
		if (allRelations[i].GetName() == name)
			break;
	}

	_Relation r2 = *(c.evaluate(&allRelations[i]));
	for (int j = 0; j < r2.Columns[0].Rows.size(); ++j) {
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
_Relation* Database::crossProduct(_Relation a, _Relation b) {
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
		aRows.push_back(a.GetRow(i)); 
	} // end for loop 
	for (j = 0; j < b.Columns[0].Rows.size(); j++) { 
		bRows.push_back(b.GetRow(j));
	} // end for loop
	vector<_Data> combinedRow;  
	for (int k = 0; k < aRows.size(); k++) { 
		for (int l = 0; l < bRows.size(); l++) { 
			combinedRow = a.CombineRows(aRows[k], bRows[l]);
			newRelation->AddRow(combinedRow);
		} // end for loop for l
	} // end for loop for k
	return newRelation;
} // end crossProduct(Relation&, Relation&)

// Helper function that gets the Columns to go in the natural Join Relation
vector<_Column>* getColumns(_Relation tempRelation, _Relation* crossRelation) {
		vector<_Column>* columns = new vector<_Column>();
		for(int i=0; i<tempRelation.Columns.size(); ++i) {
		bool addCommon = false;
		int j;
		for(j=i+1; j<tempRelation.Columns.size(); ++j) {
			if(tempRelation.Columns[i].Name == tempRelation.Columns[j].Name) {
				addCommon = true;
				break;
			}
		}
		if(addCommon) {
			columns->push_back(_Column(crossRelation->Columns[i].Name,
									crossRelation->Columns[i].AutoIncrement, 
									crossRelation->Columns[i].Type));
			if(tempRelation.removeColumn(j) == -1) {
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
	if(columns != NULL) {
		for(int i=0; i<columns->size(); ++i) {
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
	for(int j=0; j<crossRelation->Columns.size(); ++j) {
		for(int k = j+1; k<crossRelation->Columns.size(); ++k) {
			if(crossRelation->Columns[j].Name == crossRelation->Columns[k].Name) {
				if(crossRelation->Columns[j].Type == crossRelation->Columns[k].Type 
					&& crossRelation->Columns[k].Type == _Type::INT) {
						int p = *((int*)crossRelation->Columns[j].Rows[i].Data);
						int p2 = *((int*)crossRelation->Columns[k].Rows[i].Data);
						if(p != p2) {
							return false;
						}
				}
			if(crossRelation->Columns[j].Type == crossRelation->Columns[k].Type 
					&& crossRelation->Columns[k].Type == _Type::VARCHAR) {
						string* p = (string*)crossRelation->Columns[j].Rows[i].Data;
						string* p2 = (string*)crossRelation->Columns[k].Rows[i].Data;
						if(*p != *p2) {
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
	for(int i=0; i<a.size(); ++i) {
		if(a[i] == k) {
			return false;
		}
	}
	return true;
}

// Adds the row i from crossRelation to the relation naturalJoin
void addRowToNJ(_Relation* naturalJoin, _Relation* crossRelation, int z) {
	vector<string> alreadyused;
	for(int i=0; i<naturalJoin->Columns.size(); ++i) {
		for(int j=0; j<crossRelation->Columns.size(); ++j) {
			if(naturalJoin->Columns[i].Name == crossRelation->Columns[j].Name) {
				if(notUsed(naturalJoin->Columns[i].Name, alreadyused)) {
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
	for(int i=0; i<crossRelation->Columns.size(); ++i ) {
		names.push_back(crossRelation->Columns[i].Name);
	}
	for(int i=0; i<crossRelation->Columns[0].Rows.size(); ++i) {
		bool addRow;
		addRow = compareRow(crossRelation, i);
		if(addRow) {
			addRowToNJ(naturalJoin, crossRelation, i);
		}
	}
}

// Natural Join of two Relations/Tables
_Relation* Database::naturalJoin(_Relation a, _Relation b) {
	_Relation* crossRelation = crossProduct(a,b);
	_Relation tempRelation = *(crossRelation);
	_Relation* naturalJoin = new _Relation("Sudo");
	vector<_Column>* columns;
	columns = getColumns(tempRelation, crossRelation);
	addColumnsToNJ(naturalJoin, columns);
	addRows(naturalJoin, crossRelation);
	delete crossRelation;
	return naturalJoin;
}
