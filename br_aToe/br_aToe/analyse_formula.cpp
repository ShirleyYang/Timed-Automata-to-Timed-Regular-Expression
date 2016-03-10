#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>

#include "binaryTree.h"
#include "tree.hh"
#include "tinyxml.h"
#include "tinystr.h"
#include "anlAutomata.h"
#include "stdlib.h" 
#include <stdio.h>
using namespace std;

/*
SyntaxNode root_Node; //the root of the tree of formula
extern tree<string> tr;//tree of the locations                                  
extern tree<string>::iterator top, loc, one, it[100];//iterator of the tree of transitions
extern vector<LocationNode> m_recLocation_tree;//the new order of the locations into the tree
extern string int2str(int &i);//int to string 
extern string sGuard(string transition_guard);//Transfer the guard to an regular expression
extern string sUpdate(string transition_update);//Transfer the update to an regular expression signal like"." and "~"
extern string sExpression(string target_id);//Get the Name of the regular expression
extern int sID(string source_id);

void createBiTree(SyntaxNode* father);
SyntaxNode* gInitialFormula(SyntaxNode* father, string sub_string);
SyntaxNode* pre_Formula(SyntaxNode* father, string sub_string);
string m_formula_string;
SyntaxNode* root;

void mains()
{
	LocationNode LN1, LN2;
	LN1.source_id = "id1";
	LN1.target_id = "id1";
	LN1.guard = "x == 5";
	LN1.comments = "b";
	LN1.update = "x = 0";
	LN2.source_id = "id1";
	LN2.target_id = "id0";
	LN2.guard = "x <= 10";
	LN2.comments = "a";
	m_recLocation_tree.push_back(LN1);
	m_recLocation_tree.push_back(LN2);

	createBiTree(root);
	
}

void createBiTree(SyntaxNode* father)
{
	if ( father == NULL)
	{
	}
	vector<LocationNode>::iterator iter = m_recLocation_tree.begin();
	int i = 0;
	string left_string;
	string right_string;
	for (iter; iter != m_recLocation_tree.end(); ++iter)
	{
		if ( iter != m_recLocation_tree.end()-1)
	    {
			father = NULL;
	    }
		else
		{
			father = new SyntaxNode;
			if (i == 0)
			{
				father->x_subID = iter->id;
			}
			if (i == 1)
			{
				father->x_Symbol = "||";
			}
			i = 1;
			if ((iter != m_recLocation_tree.end() - 1) && (iter->source_id == (iter + 1)->source_id) && (iter->target_id == (iter + 1)->target_id) && (iter->update == (iter + 1)->update))
			{
				(iter + 1)->isVisit = false;
				father->x_Symbol = "||";
				father->leftchild->x_Symbol = "";
				father->rightchild->x_Symbol = "";
				left_string = "{<" + iter->comments + ">" + sGuard(iter->guard);
				right_string = "<" + (iter + 1)->comments + ">" + sGuard((iter + 1)->guard) + "}";
			}
			else
			{
				father->leftchild->x_formula += "<" + iter->comments + ">" + sGuard(iter->guard);
			}
			//createBiTree(father->leftchild);
			//createBiTree(father->rightchild);
		}
	}
 	
}
SyntaxNode* pre_Formula(SyntaxNode* father, string sub_string)
{
	cout << sub_string << endl;
	int  i = 0;
	bool number_Flag=false;
	SyntaxNode* new_Node = new SyntaxNode;

	new_Node->father = father;

	new_Node->x_formula.clear();

	string left_string;
	string right_string;
	int bracket = 0;

	while (sub_string.c_str()[i] !='\0')
	{
	}
	return father;
}

SyntaxNode* gInitialFormula(SyntaxNode* father, string sub_string)
{
	tree<string>::breadth_first_iterator sib = tr.begin_breadth_first();
	int id = 0;
	while (sib != tr.end(loc))
	{
		cout << *sib << endl;
		vector<LocationNode>::iterator iter = m_recLocation_tree.begin();
		int i = 0;
		string r_expression;
		cout << sub_string << endl;
		bool number_Flag = false;
		SyntaxNode* new_Node = new SyntaxNode;

		new_Node->father = father;

		new_Node->x_formula.clear();

		string left_string;
		string right_string;
		for (iter; iter != m_recLocation_tree.end(); ++iter)
		{
			if ((*sib) == iter->source_id && iter->isVisit == true)
			{
				iter->isVisit = false;
				//SubFormula m_sub_formula;
				if (i == 0)
				{
					new_Node->x_subID = iter->id;
					r_expression += sExpression(iter->source_id) + "=";;

				}
				if (i == 1)
				{
					r_expression += "&";
					new_Node->x_Symbol = "&";
					
				}
				i = 1;
				if ((iter != m_recLocation_tree.end() - 1) && (iter->source_id == (iter + 1)->source_id) && (iter->target_id == (iter + 1)->target_id) && (iter->update == (iter + 1)->update))
				{
					(iter + 1)->isVisit = false;
					r_expression += "{<" + iter->comments + ">" + sGuard(iter->guard) + "&" + "<" + (iter + 1)->comments + ">" + sGuard((iter + 1)->guard) + "}";
					new_Node->x_Symbol = "&";
					new_Node->leftchild->x_Symbol = "";
					new_Node->rightchild->x_Symbol = "";
					left_string = "{<" + iter->comments + ">" + sGuard(iter->guard);
					right_string = "<" + (iter + 1)->comments + ">" + sGuard((iter + 1)->guard) + "}";
				}
				else
				{
					r_expression += "<" + iter->comments + ">" + sGuard(iter->guard);
					new_Node->leftchild->x_formula += "<" + iter->comments + ">" + sGuard(iter->guard);
				}
				new_Node = new_Node->rightchild;
				string tmp = "";
				if (iter->target_id != "id0")
				{
					//r_expression += sUpdate(iter->update);
					tmp += sUpdate(iter->update);
					new_Node->x_Symbol = sUpdate(iter->update);
				}
				r_expression += sExpression(iter->target_id);
				tmp += sExpression(iter->target_id);
				new_Node->x_formula = sExpression(iter->target_id);
				
			}
		}
		i = 0;
		++sib;
		if (r_expression != "") {
			//m_single_expression.m_regular_expression=r_expression;
			//m_single_expression.x_id = id++;
			//m_single_expression.x_simple = false;
			//m_init_expression.push_back(m_single_expression);
		}
	}
	//vector<RegularExpression>::iterator it2 = m_init_expression.begin();
	//for (it2; it2 != m_init_expression.end(); ++it2)
	//cout <<it2->x_id<<"---"<< it2->m_regular_expression << endl;
	return father;
}
*/