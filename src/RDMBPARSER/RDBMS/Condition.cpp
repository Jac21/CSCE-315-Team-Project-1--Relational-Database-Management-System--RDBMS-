#include "Condition.h"

/*--------------------------|
	Author: Jeffrey LeRoy   |
	Group: Team C           |
	RDBMS Project			|
	Last Edited 2/10/14		|
---------------------------*/

/* --- FUNCTIONS/METHODS -- */

// Evaluate for a Comparison
_Relation Comparison::evaluate(_Relation* r) {
	//construct new empty relation
	_Relation r2 = _Relation("sudo");
	for (int i = 0; i < r->Columns.size(); ++i)
		r2.AddColumn(_Column(r->Columns[i].Name, r->Columns[i].AutoIncrement, r->Columns[i].Type));

	// Fill Relation with rows that match the comparison
	switch (c) {
	case EQUALS:
		for (int i = 0; i < r->Columns.size(); ++i) {
			if (r->Columns[i].Name == attributeName) {
				for (int j = 0; j < r->Columns[i].Rows.size(); ++j) {
					if (r->Columns[i].Rows[j].Type == compD.Type && r->Columns[i].Rows[j].Data == compD.Data) {
						vector<_Data> dt;
						for (int k = 0; k < r->Columns.size(); ++k) {
							_Data d = r->Columns[k].Get_Data(j);
							dt.push_back(d);
						}
						r2.AddRow(dt);
					}
				}
			}
		}
		return r2;

	case NOTEQUALS:
		for (int i = 0; i < r->Columns.size(); ++i) {
			if (r->Columns[i].Name == attributeName) {
				for (int j = 0; j < r->Columns[i].Rows.size(); ++j) {
					if (r->Columns[i].Rows[j].Type == compD.Type && r->Columns[i].Rows[j].Data != compD.Data) {
						vector<_Data> dt;
						for (int k = 0; k < r->Columns.size(); ++k) {
							_Data d = r->Columns[k].Get_Data(j);
							dt.push_back(d);
						}
						r2.AddRow(dt);
					}
				}
			}
		}
		return r2;

	case LESSTHAN:
		for (int i = 0; i < r->Columns.size(); ++i) {
			if (r->Columns[i].Name == attributeName) {
				for (int j = 0; j < r->Columns[i].Rows.size(); ++j) {
					if (r->Columns[i].Rows[j].Type == compD.Type && r->Columns[i].Rows[j].Data < compD.Data) {
						vector<_Data> dt;
						for (int k = 0; k < r->Columns.size(); ++k) {
							_Data d = r->Columns[k].Get_Data(j);
							dt.push_back(d);
						}
						r2.AddRow(dt);
					}
				}
			}
		}
		return r2;

	case GREATERTHAN:
		for (int i = 0; i < r->Columns.size(); ++i) {
			if (r->Columns[i].Name == attributeName) {
				for (int j = 0; j < r->Columns[i].Rows.size(); ++j) {
					if (r->Columns[i].Rows[j].Type == compD.Type && r->Columns[i].Rows[j].Data > compD.Data) {
						vector<_Data> dt;
						for (int k = 0; k < r->Columns.size(); ++k) {
							_Data d = r->Columns[k].Get_Data(j);
							dt.push_back(d);
						}
						r2.AddRow(dt);
					}
				}
			}
		}
		return r2;

	case LESSTNANOREQUAL:
		for (int i = 0; i < r->Columns.size(); ++i) {
			if (r->Columns[i].Name == attributeName) {
				for (int j = 0; j < r->Columns[i].Rows.size(); ++j) {
					if (r->Columns[i].Rows[j].Type == compD.Type && r->Columns[i].Rows[j].Data <= compD.Data) {
						vector<_Data> dt;
						for (int k = 0; k < r->Columns.size(); ++k) {
							_Data d = r->Columns[k].Get_Data(j);
							dt.push_back(d);
						}
						r2.AddRow(dt);
					}
				}
			}
		}
		return r2;

	case GREATERTHANOREQUAL:
		for (int i = 0; i < r->Columns.size(); ++i) {
			if (r->Columns[i].Name == attributeName) {
				for (int j = 0; j < r->Columns[i].Rows.size(); ++j) {
					if (r->Columns[i].Rows[j].Type == compD.Type && r->Columns[i].Rows[j].Data >= compD.Data) {
						vector<_Data> dt;
						for (int k = 0; k < r->Columns.size(); ++k) {
							_Data d = r->Columns[k].Get_Data(j);
							dt.push_back(d);
						}
						r2.AddRow(dt);
					}
				}
			}
		}
		return r2;
	}
}

// Creates two Relations from evaluationg the first
// ...two comparisons then using the second
// ...adds all the rows to the first then return the first
_Relation OR::evaluate(_Relation* r) {
	_Relation r1 = c1.evaluate(r);
	_Relation r2 = c2.evaluate(r);

	for (int i = 0; i < r2.Columns.size(); ++i) {
		vector<_Data> dt;
		for (int j = 0; j < r2.Columns.size(); ++j) {
			dt.push_back(r2.Columns[j].Get_Data(i));
		}
		r1.AddRow(dt);
	}

	return r1;
}

_Relation AND::evaluate(_Relation* r) {
	_Relation r1 = c1.evaluate(r);
	_Relation r2 = _Relation("sudo");
	for (int i = 0; i < r->Columns.size(); ++i)
		r2.AddColumn(_Column(r->Columns[i].Name, r->Columns[i].AutoIncrement, r->Columns[i].Type));

	for (int i = 0; i < r1.Columns.size(); ++i) {
		if (r1.Columns[i].Name == c2.attributeName) {
			for (int j = 0; j < r2.Columns[i].Rows.size(); ++i) {

				if (c2.c == Comparison::Comp::EQUALS) {
					if (r1.Columns[i].Rows[j].Type == c2.compD.Type && r1.Columns[i].Rows[j].Data == c2.compD.Data) {
						vector<_Data> dt;
						for (int k = 0; k < r1.Columns.size(); ++k) {
							_Data d = r->Columns[k].Get_Data(j);
							dt.push_back(d);
						}
						r2.AddRow(dt);
					}
				}

				if (c2.c == Comparison::Comp::NOTEQUALS) {
					if (r1.Columns[i].Rows[j].Type == c2.compD.Type && r1.Columns[i].Rows[j].Data != c2.compD.Data) {
						vector<_Data> dt;
						for (int k = 0; k < r1.Columns.size(); ++k) {
							_Data d = r->Columns[k].Get_Data(j);
							dt.push_back(d);
						}
						r2.AddRow(dt);
					}
				}

				if (c2.c == Comparison::Comp::LESSTHAN) {
					if (r1.Columns[i].Rows[j].Type == c2.compD.Type && r1.Columns[i].Rows[j].Data < c2.compD.Data) {
						vector<_Data> dt;
						for (int k = 0; k < r1.Columns.size(); ++k) {
							_Data d = r->Columns[k].Get_Data(j);
							dt.push_back(d);
						}
						r2.AddRow(dt);
					}
				}

				if (c2.c == Comparison::Comp::GREATERTHAN){
					if (r1.Columns[i].Rows[j].Type == c2.compD.Type && r1.Columns[i].Rows[j].Data > c2.compD.Data) {
						vector<_Data> dt;
						for (int k = 0; k < r1.Columns.size(); ++k) {
							_Data d = r->Columns[k].Get_Data(j);
							dt.push_back(d);
						}
						r2.AddRow(dt);
					}
				}

				if (c2.c == Comparison::Comp::LESSTNANOREQUAL) {
					if (r1.Columns[i].Rows[j].Type == c2.compD.Type && r1.Columns[i].Rows[j].Data <= c2.compD.Data) {
						vector<_Data> dt;
						for (int k = 0; k < r1.Columns.size(); ++k) {
							_Data d = r->Columns[k].Get_Data(j);
							dt.push_back(d);
						}
						r2.AddRow(dt);
					}
				}

				if (c2.c == Comparison::Comp::GREATERTHANOREQUAL) {
					if (r1.Columns[i].Rows[j].Type == c2.compD.Type && r1.Columns[i].Rows[j].Data >= c2.compD.Data) {
						vector<_Data> dt;
						for (int k = 0; k < r1.Columns.size(); ++k) {
							_Data d = r->Columns[k].Get_Data(j);
							dt.push_back(d);
						}
						r2.AddRow(dt);
					}
				}

			}
		}
	}

	return r2;
}
/* -- CONSTRUCTORS -- */
Comparison::Comparison(void) {}

Comparison::Comparison(String n, _Data t, Comp x)  {
	attributeName = n;
	compD = t;
	c = x;
}

AND::AND(void) {}

AND::AND(Comparison c1, Comparison c2) {
	c1 = c1;
	c2 = c2;
}


OR::OR(Comparison c1, Comparison c2) {
	c1 = c1;
	c2 = c2;
}





