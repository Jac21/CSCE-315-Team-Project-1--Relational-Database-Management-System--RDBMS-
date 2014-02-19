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

// Helper function for natural join that gets the common columns names
vector<_Column>* getcommonCols(_Relation a, _Relation b) {
	vector<_Column>* c = new vector<_Column>();
	for (int i = 0; i < a.Columns.size(); ++i) {
		for (int j = 0; j < b.Columns.size(); ++j) {
			if (a.Columns[i].Name == b.Columns[j].Name) {
				c->push_back(_Column(a.Columns[i].Name, a.Columns[i].AutoIncrement, a.Columns[i].Type));
				break;
			}
		}
	}
	return c;
}

// Helper function for combineComCol
vector<int> getCommonInd(vector<_Column> c, _Relation a) {
	vector<int> i;
	for(int j = 0; j < c.size(); ++j) {
		for(int k = 0; k < a.Columns.size(); ++k) {
			if(c[j].Name == a.Columns[k].Name) {
				i.push_back(k);
			}
		}
	}
	return i;
}

// Helper function for natural join
vector<_Column> combineComCol(_Relation a, _Relation b, vector<_Column> c) {
	vector<int> commonInda;
	vector<int> commonIndb;
	commonInda = getCommonInd(c, a);
	commonIndb = getCommonInd(c, b);
	for(int i=0; i<a.Columns[0].Rows.size(); ++i) {
		for(int j=0; j<b.Columns[0].Rows.size(); ++j) {
			bool okAdd = true;
			for(int k=0; k<c.size(); ++k) {
				if(a.Columns[commonInda[k]].Type == _Type::INT && b.Columns[commonIndb[k]].Type == _Type::INT) {
					int *p = (int*)a.Columns[commonInda[k]].Rows[i].Data;
					int *p2 = (int*)b.Columns[commonIndb[k]].Rows[j].Data;
					if(*p != *p2) {
						okAdd = false;
						break;
					}
				}
				else {
					string *p = (string*)a.Columns[commonInda[k]].Rows[i].Data;
					string *p2 = (string*)b.Columns[commonIndb[k]].Rows[j].Data;
					if(*p != *p2) {
						okAdd = false;
						break;
					}
				}
			}
			if(okAdd == true) {
				for(int z = 0; z<c.size(); z++) {
					c[z].Rows.push_back(a.Columns[commonInda[z]].Rows[i]);
				}
			}
		}
	}
	return c;
}

// Helper function for natural join
vector<_Column> getNonCommon(_Relation a, _Relation b) {
	vector<_Column> nonC;
	for(int i=0; i<a.Columns.size(); ++i) {
		bool okAdd = true;
		for(int j=0; j<b.Columns.size(); ++j) {
			if(a.Columns[i].Name == b.Columns[j].Name) {
				okAdd = false;
				break;
			}
		}
		if(okAdd == true) {
			nonC.push_back(_Column(a.Columns[i].Name, a.Columns[i].AutoIncrement, a.Columns[i].Type));
		}
	}
	return nonC;
}

// Acquires the non common column indexes
vector<int> getNonComInd(vector<_Column> temp, _Relation a) {
	vector<int> r;
	for(int i=0; i<a.Columns.size(); ++i) {
		bool okAdd = true;
		for(int j=0; j<temp.size(); ++j) {
			if(temp[j].Name == a.Columns[i].Name) {
				okAdd = false;
				break;
			}
		}
		if(okAdd == true) {
			r.push_back(i);
		}
	}
	return r;
}

// Adds the columns not in com to com that match the indexes of temps columns to the columns of com
vector<_Column> addCol(vector<_Column> com, vector<_Column> temp, _Relation a) {
	vector<int> commonIndA = getCommonInd(com, a);
	vector<int> noncomInd = getNonComInd(temp, a);
	for(int i=0; i<noncomInd.size(); ++ i) {
		com.push_back(a.Columns[noncomInd[i]]);
	}

	for(int i=0; i<temp[0].Rows.size(); ++i) {
		for(int j=0; j<a.Columns[0].Rows.size(); ++j) {
			bool toAdd = true;
			int k;
			for(k=0; k<temp.size(); ++k) {
				if(com[k].Name == a.Columns[commonIndA[k]].Name) {
					if(com[k].Type == _Type::INT && a.Columns[commonIndA[k]].Type == _Type::INT) {
						int* comData = (int*)com[k].Rows[i].Data;
						int* relAData = (int*)a.Columns[commonIndA[k]].Rows[j].Data;
						if(*comData != *relAData) {
							toAdd = false;
							break;
						}
					}
					else if(com[k].Type == _Type::VARCHAR && a.Columns[commonIndA[k]].Type == _Type::VARCHAR) {
						string* comData = (string*)com[k].Rows[i].Data;
						string* relAData = (string*)a.Columns[commonIndA[k]].Rows[j].Data;
						if(*comData != *relAData) {
							toAdd = false;
							break;
						}
					}
				}
			}
			if(toAdd == true) {
				for(int z = 0; z<noncomInd.size(); ++z) {
					bool tobreak = false;
					for(int x = 0; x<com.size(); ++x) {
						if(com[x].Name == a.Columns[noncomInd[i]].Name) {
							com[x].AddRow(a.Columns[noncomInd[i]].Rows[j]);
							tobreak = true;
						}
					}
					if(tobreak = true) { break; }
				}
			}
		}
	}
	return com;
}

// Helper function for natural join
vector<_Column> combineAll(vector<_Column> com, vector<_Column> nca, vector<_Column> ncb, _Relation a, _Relation b) {
	vector<int> commonIndA = getCommonInd(com, a);
	vector<int> commonIndB = getCommonInd(com, b);
	vector<_Column> temp;
	for(int i=0; i<com.size(); ++i) {
		temp.push_back(com[i]);
	}
	if(nca.size() != 0) {
		com = addCol(com, temp, a);
	}
	if(ncb.size() != 0) {
		com = addCol(com, temp, b);
	}
	return com;
}
// Natural Join of two Relations/Tables
_Relation* Database::naturalJoin(_Relation a, _Relation b) {
	vector<_Column>* commonC = new vector<_Column>();
	commonC = getcommonCols(a, b);
	if(commonC->size() == 0) {
		cerr << "The relations have no common Columns.\n";
	}
	vector<_Column> combComC;
	combComC = combineComCol(a, b, *(commonC));
	_Relation *newRelation = new _Relation("sudo");
	vector<_Column> nonCommona;
	vector<_Column> nonCommonb;
	nonCommona = getNonCommon(a, b);
	nonCommonb = getNonCommon(b, a);
	vector<_Column> temp;
	temp = combineAll(*(commonC), nonCommona, nonCommonb, a, b);
	for(int i=0; i<temp.size(); ++i) {
		newRelation->AddColumn(temp[i]);
	}
	return newRelation;
}

/*_Relation* Database::naturalJoin(_Relation a, _Relation b) {
	vector<_Column> aAttributes;
	vector<_Column> bAttributes;
	for (int q = 0; q < a.Columns.size(); ++q) {
		aAttributes.push_back(a.Columns[q]);
	}
	for (int q = 0; q < b.Columns.size(); ++q) {
		bAttributes.push_back(b.Columns[q]);
	}
	_Relation* tempRelation = crossProduct(a, b);
	_Relation combined("Combined");
	int i, j, k, l;
	for (i = 0; i < aAttributes.size(); i++) {
		for (j = 0; j < bAttributes.size(); j++) {
			if ((aAttributes[i].Type == bAttributes[j].Type)
				&& (aAttributes[i].Name.compare(bAttributes[j].Name))) {
				combined.AddColumn(aAttributes[i]);
				combined.AddColumn(bAttributes[j]);
			} // end if statement 
		} // end for loop with j
	} // end for loop with i
	vector<_Column> tAttributes;
	for (int q = 0; q < tempRelation->Columns.size(); ++q) {
		tAttributes.push_back(tempRelation->Columns[i]);
	}
	vector<vector<_Data>> tRows;
	vector<_Column> cAttributes;
	for (int q = 0; q < combined.Columns.size(); ++q) {
		cAttributes.push_back(combined.Columns[i]);
	}
	vector<vector<_Data>> cRows;
	vector<vector<_Data>> newRows;
	for (i = 0; i < tempRelation->Columns[0].Rows.size(); i++) {
		tRows.push_back(tempRelation->GetRow(i));
	} // end for loop with i
	if (combined.Columns.size() == 0) {
		cerr << "Error: combined columns is equal to 0\n";
		_Relation *fail = new _Relation("sudo");
		return fail;
	}
	else {
		for (j = 0; j < combined.Columns[0].Rows.size(); j++) {
			cRows.push_back(combined.GetRow(j));
		} // end for loop with j
	}
	for (i = 0; i < tAttributes.size(); i++) {
		for (j = 0; j < tRows.size(); j++) {
			for (k = 0; k < cAttributes.size(); k++) {
				for (l = 0; l < cRows.size(); l++) {
					_Data d = tempRelation->GetRow2(i, j);
					_Data d2 = combined.GetRow2(k, l);
					if (d.Data == d.Data) {
						newRows.push_back(tRows[j]);
					} // end if statemnt 
				} // end l for loop 
			} // end k for loop
		} // end j for loop
	} // end i for loop 
	for (i = 0; i < tempRelation->Columns.size(); i++) {
		tempRelation->Columns[i].Rows.erase(tempRelation->Columns[i].Rows.begin(), tempRelation->Columns[i].Rows.end());
	} // end i for loop 
	for (j = 0; j < newRows.size(); j++) {
		tempRelation->AddRow(newRows[j]);
	} // end j for loop 
	for (int q = 0; q<tempRelation->Columns.size(); ++q)
		tAttributes.push_back(tempRelation->Columns[q]);
	vector<string> commonNames;
	for (i = 0; i < cAttributes.size() / 2; i++) {
		commonNames.push_back(cAttributes[i].Name);
	} // end for loop 
	for (j = 0; j < tAttributes.size(); j++) {
		for (k = 0; k < commonNames.size(); k++) {
			if (tAttributes[j].Name.compare(commonNames[j])) {
				tempRelation->Columns.erase(tempRelation->Columns.begin() + j);
				break;
			} // end if statement 
		} // end k for loop
	} // end j for loop 
	_Relation* rel = new _Relation();
	memcpy(rel, &tempRelation, sizeof(_Relation));
	return rel;
} // end naturalJoin(Relation&, Relation&)*/