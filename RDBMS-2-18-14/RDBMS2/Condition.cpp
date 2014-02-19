#include "Condition.h"

/*--------------------------|
	Author: Jeffrey LeRoy   |
	Group: Team C           |
	RDBMS Project			|
	Last Edited 2/10/14		|
---------------------------*/

/* --- FUNCTIONS/METHODS -- */

// Evaluate for a Comparison
_Relation* Comparison::evaluate(_Relation* r) {
	//construct new empty relation
	_Relation *r2 = new _Relation("sudo");
	for (int i = 0; i < r->Columns.size(); ++i)
		r2->AddColumn(_Column(r->Columns[i].Name, r->Columns[i].AutoIncrement, r->Columns[i].Type));
	// Fill Relation with rows that match the comparison
	switch (c) {
	case EQUALS:
		int i;
		for (i = 0; i < r->Columns.size(); ++i) {
			if (r->Columns[i].Name == attributeName) {
				break;
			}
		}
		for (int j = 0; j < r->Columns[i].Rows.size(); ++j) {
			vector<_Data> dt;
			if (compD.Type == _Type::INT && r->Columns[i].Rows[j].Type == _Type::INT) {
				int* p = (int*)compD.Data;
				int* p2 = (int*)r->Columns[i].Rows[j].Data;
				if (*p == *p2) {
					for (int k = 0; k < r->Columns.size(); ++k) {
						dt.push_back(r->Columns[k].Rows[j]);
					}
					r2->AddRow(dt);
				}
			}
			else {
				string* p = (string*)compD.Data;
				string* p2 = (string*)r->Columns[i].Rows[j].Data;
				if (*p == *p2) {
					for (int k = 0; k < r->Columns.size(); ++k) {
						dt.push_back(r->Columns[k].Rows[j]);
					}
					r2->AddRow(dt);
				}
			}
		}
		return r2;

	case NOTEQUALS:
		int z;
		for (z = 0; z < r->Columns.size(); ++z) {
			if (r->Columns[z].Name == attributeName) {
				break;
			}
		}
		for (int j = 0; j < r->Columns[z].Rows.size(); ++j) {
			vector<_Data> dt;
			if (compD.Type == _Type::INT && r->Columns[z].Rows[j].Type == _Type::INT) {
				int* p = (int*)compD.Data;
				int* p2 = (int*)r->Columns[z].Rows[j].Data;
				if (*p != *p2) {
					for (int k = 0; k < r->Columns.size(); ++k) {
						dt.push_back(r->Columns[k].Rows[j]);
					}
					r2->AddRow(dt);
				}
			}
			else {
				string* p = (string*)compD.Data;
				string* p2 = (string*)r->Columns[z].Rows[j].Data;
				if (*p != *p2) {
					for (int k = 0; k < r->Columns.size(); ++k) {
						dt.push_back(r->Columns[k].Rows[j]);
					}
					r2->AddRow(dt);
				}
			}
		}
		return r2;

	case LESSTHAN:
		int x;
		for (x = 0; x < r->Columns.size(); ++x) {
			if (r->Columns[x].Name == attributeName) {
				break;
			}
		}
		for (int j = 0; j < r->Columns[x].Rows.size(); ++j) {
			vector<_Data> dt;
			if (compD.Type == _Type::INT && r->Columns[x].Rows[j].Type == _Type::INT) {
				int* p = (int*)compD.Data;
				int* p2 = (int*)r->Columns[x].Rows[j].Data;
				if (*p > *p2) {
					for (int k = 0; k < r->Columns.size(); ++k) {
						dt.push_back(r->Columns[k].Rows[j]);
					}
					r2->AddRow(dt);
				}
			}
			else {
				string* p = (string*)compD.Data;
				string* p2 = (string*)r->Columns[x].Rows[j].Data;
				if (*p > *p2) {
					for (int k = 0; k < r->Columns.size(); ++k) {
						dt.push_back(r->Columns[k].Rows[j]);
					}
					r2->AddRow(dt);
				}
			}
		}
		return r2;

	case GREATERTHAN:
		int c;
		for (c = 0; c < r->Columns.size(); ++c) {
			if (r->Columns[c].Name == attributeName) {
				break;
			}
		}
		for (int j = 0; j < r->Columns[c].Rows.size(); ++j) {
			vector<_Data> dt;
			if (compD.Type == _Type::INT && r->Columns[c].Rows[j].Type == _Type::INT) {
				int* p = (int*)compD.Data;
				int* p2 = (int*)r->Columns[c].Rows[j].Data;
				if (*p < *p2) {
					for (int k = 0; k < r->Columns.size(); ++k) {
						dt.push_back(r->Columns[k].Rows[j]);
					}
					r2->AddRow(dt);
				}
			}
			else {
				string* p = (string*)compD.Data;
				string* p2 = (string*)r->Columns[c].Rows[j].Data;
				if (*p < *p2) {
					for (int k = 0; k < r->Columns.size(); ++k) {
						dt.push_back(r->Columns[k].Rows[j]);
					}
					r2->AddRow(dt);
				}
			}
		}
		return r2;

	case LESSTNANOREQUAL:
		int v;
		for (v = 0; v < r->Columns.size(); ++v) {
			if (r->Columns[v].Name == attributeName) {
				break;
			}
		}
		for (int j = 0; j < r->Columns[v].Rows.size(); ++j) {
			vector<_Data> dt;
			if (compD.Type == _Type::INT && r->Columns[v].Rows[j].Type == _Type::INT) {
				int* p = (int*)compD.Data;
				int* p2 = (int*)r->Columns[v].Rows[j].Data;
				if (*p >= *p2) {
					for (int k = 0; k < r->Columns.size(); ++k) {
						dt.push_back(r->Columns[k].Rows[j]);
					}
					r2->AddRow(dt);
				}
			}
			else {
				string* p = (string*)compD.Data;
				string* p2 = (string*)r->Columns[v].Rows[j].Data;
				if (*p >= *p2) {
					for (int k = 0; k < r->Columns.size(); ++k) {
						dt.push_back(r->Columns[k].Rows[j]);
					}
					r2->AddRow(dt);
				}
			}
		}
		return r2;

	case GREATERTHANOREQUAL:
		int b;
		for (b = 0; b < r->Columns.size(); ++b) {
			if (r->Columns[b].Name == attributeName) {
				break;
			}
		}
		for (int j = 0; j < r->Columns[b].Rows.size(); ++j) {
			vector<_Data> dt;
			if (compD.Type == _Type::INT && r->Columns[b].Rows[j].Type == _Type::INT) {
				int* p = (int*)compD.Data;
				int* p2 = (int*)r->Columns[b].Rows[j].Data;
				if (*p <= *p2) {
					for (int k = 0; k < r->Columns.size(); ++k) {
						dt.push_back(r->Columns[k].Rows[j]);
					}
					r2->AddRow(dt);
				}
			}
			else {
				string* p = (string*)compD.Data;
				string* p2 = (string*)r->Columns[b].Rows[j].Data;
				if (*p <= *p2) {
					for (int k = 0; k < r->Columns.size(); ++k) {
						dt.push_back(r->Columns[k].Rows[j]);
					}
					r2->AddRow(dt);
				}
			}
		}
		return r2;
	}
}

// Creates two Relations from evaluationg the first
// ...two comparisons then using the second
// ...adds all the rows to the first then return the first
_Relation* OR::evaluate(_Relation* r) {
	_Relation r1 = *(c1.evaluate(r));
	_Relation r2 = *(c2.evaluate(r));
	_Relation *r3 = new _Relation("sudo");
	for (int i = 0; i < r->Columns.size(); ++i)
		r3->AddColumn(_Column(r->Columns[i].Name, r->Columns[i].AutoIncrement, r->Columns[i].Type));
	for(int i=0; i<r1.Columns[0].Rows.size(); ++i) {
		vector<_Data> dt;
		for(int j=0; j<r1.Columns.size(); ++j) {
			dt.push_back(r1.Columns[j].Rows[i]);
		}
		r3->AddRow(dt);
	}
	for(int i=0; i<r2.Columns[0].Rows.size(); ++i) {
		vector<_Data> dt;
		for(int j=0; j<r2.Columns.size(); ++j) {
			dt.push_back(r2.Columns[j].Rows[i]);
		}
		r3->AddRow(dt);
	}
	return r3;
}

// AND condition
_Relation* AND::evaluate(_Relation* r) {
	_Relation r1 = *c1.evaluate(r);
	_Relation *r2 = new _Relation("sudo");
	for (int i = 0; i < r->Columns.size(); ++i)
		r2->AddColumn(_Column(r->Columns[i].Name, r->Columns[i].AutoIncrement, r->Columns[i].Type));

	for (int i = 0; i < r1.Columns.size(); ++i) {
		if (r1.Columns[i].Name == c2.attributeName) {
			for (int j = 0; j < r2->Columns[i].Rows.size(); ++i) {
				if (c2.c == Comparison::Comp::EQUALS) {
					if (r1.Columns[i].Rows[j].Type == c2.compD.Type && r1.Columns[i].Rows[j].Data == c2.compD.Data) {
						vector<_Data> dt;
						for (int k = 0; k < r1.Columns.size(); ++k) {
							_Data d = r->Columns[k].Get_Data(j);
							dt.push_back(d);
						}
						r2->AddRow(dt);
					}
				}

				if (c2.c == Comparison::Comp::NOTEQUALS) {
					if (r1.Columns[i].Rows[j].Type == c2.compD.Type && r1.Columns[i].Rows[j].Data != c2.compD.Data) {
						vector<_Data> dt;
						for (int k = 0; k < r1.Columns.size(); ++k) {
							_Data d = r->Columns[k].Get_Data(j);
							dt.push_back(d);
						}
						r2->AddRow(dt);
					}
				}

				if (c2.c == Comparison::Comp::LESSTHAN) {
					if (r1.Columns[i].Rows[j].Type == c2.compD.Type && r1.Columns[i].Rows[j].Data < c2.compD.Data) {
						vector<_Data> dt;
						for (int k = 0; k < r1.Columns.size(); ++k) {
							_Data d = r->Columns[k].Get_Data(j);
							dt.push_back(d);
						}
						r2->AddRow(dt);
					}
				}

				if (c2.c == Comparison::Comp::GREATERTHAN){
					if (r1.Columns[i].Rows[j].Type == c2.compD.Type && r1.Columns[i].Rows[j].Data > c2.compD.Data) {
						vector<_Data> dt;
						for (int k = 0; k < r1.Columns.size(); ++k) {
							_Data d = r->Columns[k].Get_Data(j);
							dt.push_back(d);
						}
						r2->AddRow(dt);
					}
				}

				if (c2.c == Comparison::Comp::LESSTNANOREQUAL) {
					if (r1.Columns[i].Rows[j].Type == c2.compD.Type && r1.Columns[i].Rows[j].Data <= c2.compD.Data) {
						vector<_Data> dt;
						for (int k = 0; k < r1.Columns.size(); ++k) {
							_Data d = r->Columns[k].Get_Data(j);
							dt.push_back(d);
						}
						r2->AddRow(dt);
					}
				}

				if (c2.c == Comparison::Comp::GREATERTHANOREQUAL) {
					if (r1.Columns[i].Rows[j].Type == c2.compD.Type && r1.Columns[i].Rows[j].Data >= c2.compD.Data) {
						vector<_Data> dt;
						for (int k = 0; k < r1.Columns.size(); ++k) {
							_Data d = r->Columns[k].Get_Data(j);
							dt.push_back(d);
						}
						r2->AddRow(dt);
					}
				}

			}
		}
	}

	return r2;
}
/* -- CONSTRUCTORS -- */
Comparison::Comparison(void) {}

Comparison::Comparison(string n, _Data t, Comp x)  {
	attributeName = n;
	compD = t;
	c = x;
}

AND::AND(void) {}

AND::AND(Comparison comp, Comparison comp2) {
	c1.attributeName =  comp.attributeName;
	c1.c =  comp.c;
	c1.compD = comp.compD;
	c2.attributeName = comp2.attributeName;
	c2.c = comp2.c;
	c2.compD = comp2.compD;
}


OR::OR(Comparison comp, Comparison comp2) {
	c1.attributeName = comp.attributeName;
	c1.c = comp.c;
	c1.compD = comp.compD;
	c2.attributeName = comp2.attributeName;
	c2.c = comp2.c;
	c2.compD = comp2.compD;
}





