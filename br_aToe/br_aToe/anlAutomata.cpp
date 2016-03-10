#include <string>
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>

#include "binaryTree.h"
#include "biTree.h"
#include "tree.hh"
#include "tinyxml.h"
#include "tinystr.h"
#include "anlAutomata.h"
#include "stdlib.h" 
#include <stdio.h>

using namespace std;

//符号说明：
//label name -- <a> in comments in .xml
//guard -- x==8 ->[8];x<8 ->[0,8)
//concatenation -- "."means x=0 ; "~" means no update;
//name of equations -- X1,X2,X3-->according to the target_location name


extern map <string, int>  m_templateName_ID;
tree<string> tr;//tree of the locations                                  
tree<string>::iterator top, loc, one, it[100];//iterator of the tree of transitions

vector<string> source_location;
vector<string> target_location;

SyntaxNode rootNode;

int numLocation; //number of locations 

LocationNode m_location_node;
vector<LocationNode> m_location_tree;
vector<LocationNode> m_recLocation_tree;//the new order of the locations into the tree
//vector<string> m_init_expression;
//vector<RegularExpression> m_init_expression;
vector<WholeFormula> m_all_formulas;
//WholeFormula m_whole_formula;
vector<string> gGetClocks(TiXmlNode* pParent);//get all clocks informations
vector<string> getClockVector(string declaration);//cut the vectors of clocks
string int2str(int &i);//int to string 
string sGuard(string transition_guard);//Transfer the guard to an regular expression
string sUpdate(string transition_update);//Transfer the update to an regular expression signal like"." and "~"
string sExpression(string target_id);//Get the Name of the regular expression
int sID(string source_id);
string gGetAcceptLocation(TiXmlNode* pParent);//get all transition information into the struct m_location_tree 
string gFindInitID(TiXmlNode* pParent);//find the initial location
void gCreateTree(string location, tree<string>::iterator one);//build the tree of all transitions
void gInitialExpression(vector<LocationNode> m_recLocation_tree);//get the initial timed regular expression
void sQuasilinearEquations(WholeFormula m_whole_formula);//solve the quasilinear equations into simple style
void gSubstituteFormula(WholeFormula m_single_formula, vector<WholeFormula> m_all_formulas);//substitute one subformula into another formula
void gCombineFormula(vector<WholeFormula> m_all_formulas);
int gNumSourceID(string sourceID);
void gInitialFormula(vector<LocationNode> m_recLocation_tree);//build the binary tree of single formula
//SyntaxNode *CreateSingleTree(SyntaxNode * father, string order_string);


void sQuasilinearEquationsl(WholeFormula m_whole_formula);
int main(){
	TiXmlBase::SetCondenseWhiteSpace(false);
	TiXmlDocument doc("testTA2.xml");
	if (doc.LoadFile()){
		gGetClocks(&doc);
		cout<<"InitLocation:"<<gFindInitID(&doc)<<endl;
		string acceptLocation = gGetAcceptLocation(&doc);
		cout<<"acceptLocation:"<<acceptLocation<<endl;
		top = tr.begin();
		one = tr.insert(top, acceptLocation);
		gCreateTree(acceptLocation, one);
		gInitialFormula(m_recLocation_tree);
		gInitialExpression(m_recLocation_tree);
		//rootNode.father = NULL;
		//string expression = "X2={<c>}|{{<e>|<d>(2,5)}~<b>.X2}";
	    //CreateSingleTree(&rootNode, expression);
		//gCombineFormula(m_all_formulas);
		if (loc != tr.end()) {
			tree<string>::iterator sib2 = tr.begin(one);
			tree<string>::iterator end2 = tr.end(one);
			
			while (sib2 != end2) {
				for (int i = 0; i < tr.depth(sib2); ++i)
					cout << " ";
				cout << (*sib2) << endl;
				++sib2;
			}
		}	
	}
	doc.SaveFile();
	
	cout <<"location number:"<< numLocation << endl;
	system("pause");

	getchar();
	return 0;
}


vector<string> gGetClocks(TiXmlNode* pParent){
	vector<string> clocks;
	if (!pParent)
	{
		return clocks;
	}
	pParent = pParent->FirstChild();
	while (pParent)
	{
		if (TiXmlNode::TINYXML_ELEMENT == pParent->Type() && "nta" == string(pParent->Value()))
		{
			pParent = pParent->FirstChild();
			break;
		}
		pParent = pParent->NextSibling();
	}
	TiXmlNode* pChild;
	pChild = pParent;
	while (pChild)
	{
		if (pChild->Type() == TiXmlNode::TINYXML_ELEMENT && "declaration" == string(pChild->Value()))
		{
			string declaration = pChild->FirstChild()->Value();
			clocks = getClockVector(declaration);
		}
		pChild = pChild->NextSibling();
		if (pChild->Type() == TiXmlNode::TINYXML_ELEMENT && "template" == string(pChild->Value()))
		{
			pChild = pChild->FirstChild();
			while (pChild)
			{
				if (pChild->Type() == TiXmlNode::TINYXML_ELEMENT && "declaration" == string(pChild->Value()))
				{
					string declaration = pChild->FirstChild()->Value();
					clocks = getClockVector(declaration);
				}
				pChild = pChild->NextSibling();
			}
		}
	}
	for (int i = 0; i <= clocks.size() - 1; i++)
		cout << clocks[i] << endl;
	return clocks;
}

//push the clock names into a vector
vector<string> getClockVector(string declaration){
	int i = 0, n = 0;
	string dclclock;
	string ll;
	vector<string> dclclocks;
	vector<string> clocks;
	while (i != declaration.size()){
		if (declaration.c_str()[i] != ';'){
			dclclock += declaration.c_str()[i];
		}
		else{
			dclclocks.push_back(dclclock);
		}
		i++;
	}
	while (n != dclclocks.size()){
		if (std::string::npos != dclclocks[n].find("clock"))
		{
			int pos = dclclocks[n].find("clock");
			int clockpos = pos + 5;
			for (clockpos; clockpos != dclclocks[n].size(); clockpos++)
			{
				if (dclclocks[n][clockpos] != ' ')
				{
					if (dclclocks[n][clockpos] != ',')
					{
						ll += dclclocks[n][clockpos];

					}
					else{
						clocks.push_back(ll);
						ll.clear();
					}
				}
			}
			clocks.push_back(ll);
		}
		n++;
	}
	return clocks;
}

string int2str(int &i)
{
	string s;
	stringstream ss(s);
	ss << i;

	return ss.str();
}

string sGuard(string transition_guard)
{
	int i = 0;
	if (transition_guard.size() == 0)
	{
		return "";
	}
	string retGuard;
	
	if (transition_guard.find('&') == -1)//format like: 1<=x<=6
	{
		if (transition_guard[0] >= '0'&& transition_guard[0] <= '9')
		{
			int clockvalue[2] = { 0, 0 };
			int j = 0;
			int k = 0;
			int nationsum = 0;
			int iPos = 0;
			while (j <= transition_guard.size())
			{
				if (transition_guard[j] >= '0'&& transition_guard[j] <= '9')
				{
					clockvalue[k] = (10 * clockvalue[k]) + transition_guard[j] - '0';
				}
				else
				{
					if (transition_guard[j] == '=')
					{
						nationsum++;
						iPos = j;
					}
					k = 1;
				}
				j++;
			}
			int leftvalue = clockvalue[0] <= clockvalue[1] ? clockvalue[0] : clockvalue[1];
			int rightvalue = clockvalue[0] > clockvalue[1] ? clockvalue[0] : clockvalue[1];
			if (nationsum == 2)
			{
				retGuard = "[" + int2str(leftvalue) + "," + int2str(rightvalue) + "]";
			}
			else if (nationsum == 0)
			{
				retGuard = "(" + int2str(leftvalue) + "," + int2str(rightvalue) + ")";
			}
			else if (iPos< transition_guard.size()/2-1)
			{
				retGuard = "[" + int2str(leftvalue) + "," + int2str(rightvalue) + ")";
			}
			else if (iPos>= transition_guard.size()/2-1)
			{
				retGuard = "(" + int2str(leftvalue) + "," + int2str(rightvalue) + "]";
			}
			return retGuard;
		}
		else
		{
			int iPos = transition_guard.find('=');
			while (iPos != -1)
			{
				int clockvalue = 0;
				string left_nation = ""; //false->close(  true->open[
				string right_nation = ""; //false->close,) true-> open]
				if (transition_guard.find('<') != -1)
				{
					iPos = transition_guard.find('<');
				}
				else if (transition_guard.find('>') != -1)
				{
					iPos = transition_guard.find('>');
				}
				while (0 != transition_guard.size())
				{
					if (transition_guard.c_str()[iPos] == '<')
					{

						left_nation = "[0,";
						iPos++;
						if (transition_guard.c_str()[iPos] == '=')
						{
							right_nation = "]";
							iPos++;
						}
						else right_nation = ")";
					}
					if (transition_guard.c_str()[iPos] == '>')
					{
						right_nation = ",+)";
						iPos++;
						if (transition_guard.c_str()[iPos] == '=')
						{
							left_nation = "[";
							iPos++;
						}
						else left_nation = "(";
					}
					if (transition_guard.c_str()[iPos] == '='&&transition_guard.c_str()[iPos++] == '=')
					{
						left_nation = "[";
						right_nation = "]";
						iPos++;
					}
					while (transition_guard.c_str()[iPos] == ' ')
					{
						iPos++;
					}
					while (transition_guard.c_str()[iPos] >= '0'&& transition_guard.c_str()[iPos] <= '9')
					{
						clockvalue = (10 * clockvalue) + transition_guard.c_str()[iPos] - '0';
						iPos++;
					}
					i++;
					retGuard = left_nation + int2str(clockvalue) + right_nation;
					return retGuard;
				}
			}
		}
	}
	else
	{
		int iPos = transition_guard.find('=');
		int leftvalue = 0;
		int rightvalue = 0;
		string left_nation = ""; //false->close(  true->open[
		string right_nation = ""; //false->close,) true-> open]
		while (transition_guard.size() != 0)
		{
			if (transition_guard.find('<') != -1)
		    {
		    	iPos = transition_guard.find('<');
		    }
			if (transition_guard.c_str()[iPos] == '<')
			{
				//left_nation = "[0,";
				iPos++;
				if (transition_guard.c_str()[iPos] == '=')
				{
					right_nation = "]";
					iPos++;
				}
				else right_nation = ")";
			}
			while (transition_guard.c_str()[iPos] == ' ')
			{
				iPos++;
			}
			while (transition_guard.c_str()[iPos] >= '0'&& transition_guard.c_str()[iPos] <= '9')
			{
				rightvalue = (10 * rightvalue) + transition_guard.c_str()[iPos] - '0';
				iPos++;
			}
	    	if (transition_guard.find('>') != -1)
		    {
		    	iPos = transition_guard.find('>');
	    	}
			if (transition_guard.c_str()[iPos] == '>')
			{
				//right_nation = ",+)";
				iPos++;
				if (transition_guard.c_str()[iPos] == '=')
				{
					left_nation = "[";
					iPos++;
				}
				else left_nation = "(";
			}
			while (transition_guard.c_str()[iPos] == ' ')
			{
				iPos++;
			}
			while (transition_guard.c_str()[iPos] >= '0'&& transition_guard.c_str()[iPos] <= '9')
			{
				leftvalue = (10 * leftvalue) + transition_guard.c_str()[iPos] - '0';
				iPos++;
			}
			retGuard = left_nation + int2str(leftvalue) +","+ int2str(rightvalue) + right_nation;
			return retGuard;
		}

	}
	//return retGuard;
}

string sUpdate(string transition_update)
{
	string m_concatanation;
	if (transition_update.size()!=0)
	{
		m_concatanation = ".";
	}
	else m_concatanation = "~";
	return m_concatanation;
}

string sExpression(string target_id)
{
	string m_expression;
	if (target_id.size()==0)
	{
		return "";
	}
	int iPos = target_id.find("d");
	int expression_ID =0;
	while (0 != target_id.size())
	{
		iPos++;
		while (target_id.c_str()[iPos] == ' ')
		{
			iPos++;
		}
		while (target_id.c_str()[iPos] >= '0'&& target_id.c_str()[iPos] <= '9')
		{
			expression_ID = (10 * expression_ID) + target_id.c_str()[iPos] - '0';
			iPos++;
		}
		if (expression_ID !=0)
		{
			m_expression = "X" + int2str(expression_ID);
		}
		else
		{
			m_expression = "";
		}
		return m_expression;
	}	
}

int sID(string source_id){
	int m_ID = 0;
	if (source_id.size() == 0) return 0;
	int iPos = source_id.find("d");
	while (0 != source_id.size())
	{
		iPos++;
		while (source_id.c_str()[iPos] == ' ')
		{
			iPos++;
		}
		while (source_id.c_str()[iPos] >= '0' && source_id.c_str()[iPos] <='9')
		{
			m_ID = (10 * m_ID) + source_id.c_str()[iPos] - '0';
			iPos++;
		}
		return m_ID;
	}
}

string gGetAcceptLocation(TiXmlNode* pParent)
{
	string acceptLocation;
	int locationID = 0;
	if (!pParent)
	{
		return acceptLocation;
	}
	pParent = pParent->FirstChild();
	while (pParent)
	{
		if (TiXmlNode::TINYXML_ELEMENT == pParent->Type() && "nta" == string(pParent->Value()))
		{
			pParent = pParent->FirstChild()->NextSibling();
			break;
		}
		pParent = pParent->NextSibling();
	}
	TiXmlNode* pChild;
	pChild = pParent->FirstChild();
	while (pChild)
	{
		if (pChild->Type() == TiXmlNode::TINYXML_ELEMENT && "location" == string(pChild->Value()))
		{
			if ("location" == string(pChild->Value()))
			{
				numLocation++;
			}
		}
		if (pChild->Type() == TiXmlNode::TINYXML_ELEMENT &&  "transition" == string(pChild->Value()))
		{
			TiXmlNode* pGrandson = pChild->FirstChild();
			while (pGrandson)
			{
				TiXmlAttribute* pAttrib = pGrandson->ToElement()->FirstAttribute();
				while (pAttrib)
				{
					// assumption: target is the next to the source.
					if ("ref" == string(pAttrib->Name()) && "source" == string(pGrandson->Value()))
					{
						m_location_node.source_id = string(pAttrib->Value());
						m_location_node.id = locationID++;
						m_location_node.isVisit = false;
						source_location.push_back(string(pAttrib->Value()));
					}
					if ("ref" == string(pAttrib->Name()) && "target" == string(pGrandson->Value()))
					{

						m_location_node.target_id = string(pAttrib->Value());
					    target_location.push_back(string(pAttrib->Value()));
					}
					
					if ("kind" == string(pAttrib->Name()) && "label" == string(pGrandson->Value()))
					{
						
						if ("guard" == string(pAttrib->Value()))
						{
							m_location_node.guard = string(pGrandson->FirstChild()->Value());
							pAttrib = pAttrib->Next();
							if ("assignment" != string(pAttrib->Value()) && "comments" == string(pAttrib->Value()))
							{
								m_location_node.update = "";
							}
							if ("assignment" != string(pAttrib->Value()) && "comments" != string(pAttrib->Value()))
							{
								m_location_node.update = "";
								m_location_node.comments = "";
							}	
						}
						
						if ("assignment" == string(pAttrib->Value()))
						{
							m_location_node.update=(string(pGrandson->FirstChild()->Value()));
							if ("comments" != string(pAttrib->Next()->Value()))
							{
								m_location_node.comments="";
							} 
							
						}
						if ("comments" == string(pAttrib->Value()))
						{
							m_location_node.comments =string( pGrandson->FirstChild()->Value());
						}
					}
					pAttrib = pAttrib->Next();
				}

				pGrandson = pGrandson->NextSibling();
			}
			m_location_tree.push_back(m_location_node);
			m_location_node.update = "";
			m_location_node.guard = "";
			m_location_node.comments = "";
		}
		pChild = pChild->NextSibling();
	}

	vector<LocationNode>::iterator iter = m_location_tree.begin();
	for (iter; iter != m_location_tree.end(); ++iter)
		cout <<iter->id<<" "<<iter->isVisit<<" "<< iter->source_id << " " << iter->target_id <<" "<<iter->guard<<" "<<iter->update<<" "<<iter->comments<< endl;
	
	for (int i = 0; i <=target_location.size() - 1; i++)
	{
		//find the acceptLocation:1.target_id but not source_id   2.source_id = target_id    3.any location in target locations 
		vector<string>::iterator result = find(source_location.begin(), source_location.end(), target_location[i]);
		if (result == source_location.end()){ //找到非source的target location
			acceptLocation = target_location[i];
		}
	}
	if (acceptLocation != "")
	{
		return acceptLocation;
	}
	else
	{
		for(int i = 0; i <= source_location.size() - 1; i++)
		{
			for (int j = 0; i <= target_location.size() - 1; j++)
			{
				if (source_location[i] == target_location[j])
				{
					acceptLocation = source_location[i];
				}
			}
		}
		return acceptLocation;
	}
	/*
	vector<string>::iterator source_ids;
	sort(source_location.begin(), source_location.end());
	source_ids = unique(source_location.begin(), source_location.end());
	if (source_ids != source_location.end()){
		source_location.erase(source_ids, source_location.end());
	}
	for (int i = 0; i <= source_location.size() - 1; i++)
		cout << source_location[i] << " ";
		*/
}

string gFindInitID(TiXmlNode* pParent)
{
	string initLocation;
	if (!pParent)
	{
		return initLocation;
	}
	pParent = pParent->FirstChild();
	while (pParent)
	{
		if (TiXmlNode::TINYXML_ELEMENT == pParent->Type() && "nta" == string(pParent->Value()))
		{
			pParent = pParent->FirstChild()->NextSibling();
			break;
		}
		pParent = pParent->NextSibling();
	}
	TiXmlNode* pChild = pParent->FirstChild();
	while (pChild)
	{
		if (pChild->Type() == TiXmlNode::TINYXML_ELEMENT && "init" == string(pChild->Value()))
		{
			TiXmlAttribute* pAttrib = pChild->ToElement()->FirstAttribute();
			while (pAttrib)
			{
				if ("ref" == string(pAttrib->Name()))
				{
					initLocation = pAttrib->Value();
					pChild = pChild->NextSibling();	// move to transition
					break;	// finish the init process
				}
				pAttrib = pAttrib->Next();
			}
			
		}
		pChild = pChild->NextSibling();
	}
	return initLocation;
}

void gCreateTree(string location, tree<string>::iterator one)//put the locations into a tree
{
	int iterNum = 0;
	for (int j = 0; j <= m_location_tree.size() - 1; j++)
	{
		for (int i = 0; i <= m_location_tree.size() - 1; i++)
		{
			if (m_location_tree[i].isVisit != true)
			{
				if (m_location_tree[i].target_id == location)
				{
					m_location_tree[i].isVisit = true;
					it[iterNum++] = tr.append_child(one, m_location_tree[i].source_id);
					m_recLocation_tree.push_back(m_location_tree[i]);
				}
			}
		}
		location = m_recLocation_tree[j].source_id;
		one = it[j];
	}
	
	vector<LocationNode>::iterator iter =m_recLocation_tree.begin();
	for (iter; iter != m_recLocation_tree.end(); ++iter)
		cout << iter->id << " " << iter->isVisit << " " << iter->source_id << " " << iter->target_id << " " << iter->guard << " " << iter->update << " " << iter->comments << endl;
		
}

int gNumSourceID(string sourceID)
{
	int numSourceID = 0;
	vector<LocationNode>::iterator iter = m_recLocation_tree.begin();
	for (iter; iter != m_recLocation_tree.end(); ++iter)
	{
		if (iter->source_id == sourceID)
		{
			numSourceID++;
		}
	}
	return numSourceID;
}

void gInitialFormula(vector<LocationNode> m_recLocation_tree)
{
	tree<string>::breadth_first_iterator sib = tr.begin_breadth_first();
	int id = 0;
	vector<nodeTree> formulaTree;
	vector<string> sibTree;
	while (sib != tr.end(loc))
	{
		vector<string>::iterator it = find(sibTree.begin(), sibTree.end(), (*sib));
		if (it == sibTree.end())
		{
			sibTree.push_back(*sib);
			cout << "location:" << *sib << endl;
			int numSourceID = gNumSourceID(*sib);
			vector<LocationNode>::iterator iter = m_recLocation_tree.begin();
			struct   nodeTree* root = NULL;
			for (iter; iter != m_recLocation_tree.end(); ++iter)
			{
				if ((*sib) == iter->source_id && iter->isVisit == true)
				{
					iter->isVisit = false;
					//string nodeValue = "<" + iter->comments + ">" + sGuard(iter->guard);
					//root = insert(root, nodeValue);
					if (numSourceID == 1)
					{
						string nodeValue = "<" + iter->comments + ">" + sGuard(iter->guard);
						if (iter->target_id != "id0")
						{
							string rightValue = sExpression(iter->target_id);
							root = insert(root, ".", '.');
							if (numSourceID == gNumSourceID(*sib))
							{
								root->rootName = sExpression(iter->source_id);
							}
							root = insert(root, nodeValue, 'e');
							root = insert(root, rightValue, 'x');
						}
						else
						{
							root = insert(root, nodeValue, 'e');
							if (numSourceID == gNumSourceID(*sib))
							{
								root->rootName = sExpression(iter->source_id);
							}
						}
					}
					else
					{
						if ((iter != m_recLocation_tree.end() - 1) && (iter->source_id == (iter + 1)->source_id) && (iter->target_id == (iter + 1)->target_id) && (iter->update == (iter + 1)->update))
						{
							(iter + 1)->isVisit = false;
							if (iter->target_id != "id0")
							{
								string rightValue = sExpression(iter->target_id);
								root = insert(root, ".", '.');
								root = insert(root, "|", '|');
								root = insert(root, rightValue, 'x');
								string nodeValue = "<" + iter->comments + ">" + sGuard(iter->guard);
								string leftValue = "<" + (iter + 1)->comments + ">" + sGuard((iter + 1)->guard);
								root = insert(root, nodeValue, 'e');
								root = insert(root, leftValue, 'e');
							}
							else
							{
								root = insert(root, "|", '|');
								string nodeValue = "<" + iter->comments + ">" + sGuard(iter->guard);
								string leftValue = "<" + (iter + 1)->comments + ">" + sGuard((iter + 1)->guard);
								root = insert(root, nodeValue, 'e');
								root = insert(root, leftValue, 'e');
							}
						}
						else{
							string nodeValue = "<" + iter->comments + ">" + sGuard(iter->guard);
							root = insert(root, "|", '|');
							if (numSourceID == gNumSourceID(*sib))
							{
								root->rootName = sExpression(iter->source_id);
							}
							if (iter->target_id != "id0")
							{
								string rightValue = sExpression(iter->target_id);
								root = insert(root, ".", '.');
								root = insert(root, nodeValue, 'e');
								root = insert(root, rightValue, 'x');
								//root->left = newNode(nodeValue, 'e');
								//root->right = newNode(rightValue, 'x');
							}
							else
							{
								root = insert(root, nodeValue, 'e');
							}
						}
						numSourceID--;
					}
					
				}
				
			}
			printInOrder(root);
		}
		++sib;
	}
}
void gInitialExpression(vector<LocationNode> m_recLocation_tree)
{ 
	tree<string>::breadth_first_iterator sib = tr.begin_breadth_first();
	int id = 0;
	//RegularExpression m_single_expression;
	WholeFormula m_whole_formula;
	//string r_expression;
	vector<string> sibTree;
	while (sib != tr.end(loc)) 
	{
		vector<string>::iterator it = find(sibTree.begin(), sibTree.end(), (*sib));
		if (it == sibTree.end())
		{
			sibTree.push_back(*sib);
			cout << "location:" << *sib << endl;
		}
			vector<LocationNode>::iterator iter = m_recLocation_tree.begin();
			int i = 0;
			string r_expression;
			SubFormula m_sub_formula;
			for (iter; iter != m_recLocation_tree.end(); ++iter)
			{
				if ((*sib) == iter->source_id && iter->isVisit == true)
				{
					iter->isVisit = false;
					m_sub_formula.x_edge = "";
					//SubFormula m_sub_formula;
					if (i == 0)
					{
						m_whole_formula.x_Name = sExpression(iter->source_id);
						m_whole_formula.x_id = sID(iter->source_id);
						r_expression += sExpression(iter->source_id) + "=";
						m_sub_formula.x_operation = "={";
					}
					if (i == 1)
					{
						r_expression += "|";
						m_sub_formula.x_operation = "|{";
					}
					i = 1;
					if ((iter != m_recLocation_tree.end() - 1) && (iter->source_id == (iter + 1)->source_id) && (iter->target_id == (iter + 1)->target_id) && (iter->update == (iter + 1)->update))
					{
						(iter + 1)->isVisit = false;
						r_expression += "{<" + iter->comments + ">" + sGuard(iter->guard) + "|" + "<" + (iter + 1)->comments + ">" + sGuard((iter + 1)->guard) + "}";
						m_sub_formula.x_edge = "{<" + iter->comments + ">" + sGuard(iter->guard) + "|" + "<" + (iter + 1)->comments + ">" + sGuard((iter + 1)->guard) + "}";
						//i = 1;
					}
					else
					{
						r_expression += "<" + iter->comments + ">" + sGuard(iter->guard);
						m_sub_formula.x_edge = "<" + iter->comments + ">" + sGuard(iter->guard);
					}
					string tmp = "";
					if (iter->target_id != "id0")
					{
						r_expression += sUpdate(iter->update);
						tmp += sUpdate(iter->update);
					}
					r_expression += sExpression(iter->target_id);
					tmp += sExpression(iter->target_id);
					m_sub_formula.x_concatenation = tmp;
					m_sub_formula.x_exchange = false;
					m_whole_formula.x_subFormula.push_back(m_sub_formula);
				}
			}
			i = 0;
		
		++sib;	
		if (r_expression != "") {
			//m_single_expression.m_regular_expression=r_expression;
			//m_single_expression.x_id = id++;
			//m_single_expression.x_simple = false;
			//m_init_expression.push_back(m_single_expression);
			sQuasilinearEquations(m_whole_formula);
			m_all_formulas.push_back(m_whole_formula);
			m_whole_formula.x_subFormula.clear();
			rootNode.father = NULL;
			//CreateSingleTree(&rootNode, r_expression);
		}
	}
	//vector<RegularExpression>::iterator it2 = m_init_expression.begin();
	//for (it2; it2 != m_init_expression.end(); ++it2)
	//cout <<it2->x_id<<"---"<< it2->m_regular_expression << endl;
	vector<WholeFormula>::iterator it = m_all_formulas.begin();
	for (it; it != m_all_formulas.end(); ++it){
		cout << "Single Formulas:" <<it->x_id<< it->x_Name;
		for (int i = 0; i <= it->x_subFormula.size() - 1;i++)
		{
			cout <<it->x_subFormula[i].x_operation<< it->x_subFormula[i].x_edge<<it->x_subFormula[i].x_concatenation<<"}" ;
		}
		cout << endl;
	}
	gSubstituteFormula(m_all_formulas[0], m_all_formulas);

}

void sQuasilinearEquations(WholeFormula m_whole_formula){
	for (int i = 0; i <= m_whole_formula.x_subFormula.size() - 1; i++)
	{
		string subConcatenation = m_whole_formula.x_subFormula[i].x_concatenation;
		if (subConcatenation != "")
		{
			string operation = "";
			operation += subConcatenation[0];
			subConcatenation = subConcatenation.substr(1, subConcatenation.size());
			if (subConcatenation == m_whole_formula.x_Name)
			{
				if (operation == "~")
				{
					operation = "~*";
				}
				if (operation == ".")
				{
					operation = "*";
				}
				m_whole_formula.x_subFormula[i].x_exchange = true;
				m_whole_formula.x_subFormula[i].x_edge += operation;
				m_whole_formula.x_subFormula[i].x_concatenation = m_whole_formula.x_subFormula[i].x_concatenation[0];
				cout << "testa:" << m_whole_formula.x_subFormula[i].x_edge << m_whole_formula.x_subFormula[i].x_concatenation << m_whole_formula.x_subFormula[i - 1].x_edge << endl;
				if (i != 0 && !m_whole_formula.x_subFormula[i - 1].x_exchange)
				{
					m_whole_formula.x_subFormula[i].x_operation = m_whole_formula.x_subFormula[i - 1].x_operation;
					m_whole_formula.x_subFormula[i - 1].x_operation = "";
					SubFormula tmp = m_whole_formula.x_subFormula[i - 1];
					m_whole_formula.x_subFormula[i - 1] = m_whole_formula.x_subFormula[i];
					m_whole_formula.x_subFormula[i] = tmp;
				}
			}
		}
		m_whole_formula.x_subFormula[i].x_exchange = false;
	}
}
/*void sQuasilinearEquationsl(WholeFormula m_whole_formula)
{
	vector<WholeFormula>::iterator it = m_all_formulas.begin();
	for (it; it != m_all_formulas.end(); ++it)
	{
		for (int i = 0; i <= it->x_subFormula.size() - 1; i++)
		{
			string subConcatenation = it->x_subFormula[i].x_concatenation;
			if (subConcatenation != "")
		    {
				string operation = "";
				operation += subConcatenation[0];
				subConcatenation = subConcatenation.substr(1,subConcatenation.size());
				if (subConcatenation == it->x_Name)
				{
					cout << "success" << endl;
					if (operation =="~")
					{
						operation = "~*";
					}
					if (operation ==".")
					{
						operation = "*";
					}
					it->x_subFormula[i].x_exchange = true;
					it->x_subFormula[i].x_edge += operation;
					it->x_subFormula[i].x_concatenation = it->x_subFormula[i].x_concatenation[0];
					cout <<"testa:"<< it->x_subFormula[i].x_edge <<it->x_subFormula[i].x_concatenation <<it->x_subFormula[i-1].x_edge<< endl;
					if (i != 0 && !it->x_subFormula[i-1].x_exchange)
					{
						it->x_subFormula[i].x_operation = it->x_subFormula[i - 1].x_operation;
						it->x_subFormula[i - 1].x_operation = "";
						SubFormula tmp = it->x_subFormula[i - 1];
						it->x_subFormula[i - 1] = it->x_subFormula[i];
						it->x_subFormula[i] = tmp;
					}
				}
		    }
			//cout << "Changed Formula:" << it->x_Name << it->x_subFormula[i].x_operation << it->x_subFormula[i].x_edge << it->x_subFormula[i].x_concatenation;
		}
		//cout << "Changed Formula:" << it->x_Name << it->x_subFormula[0].x_operation << it->x_subFormula[0].x_edge << it->x_subFormula[0].x_concatenation << it->x_subFormula[1].x_operation<< it->x_subFormula[1].x_edge << it->x_subFormula[1].x_concatenation << endl;
		//cout << endl;
	}
	
}
*/
void gSubstituteFormula(WholeFormula m_single_formula, vector<WholeFormula> m_all_formulas)
{
	//vector<WholeFormula>::iterator it = m_all_formulas.begin();
	//vector<SubFormula>::iterator go = m_single_formula.x_subFormula.begin();
	for (int i = 0; i <= m_single_formula.x_subFormula.size() - 1; i++)
	{
		string subConcatenation =m_single_formula.x_subFormula[i].x_concatenation;
		if (subConcatenation != "")
		{
			subConcatenation = subConcatenation.substr(1, subConcatenation.size());
		}
		if (subConcatenation != "")
		{
			vector<WholeFormula>::iterator it = m_all_formulas.begin();
			for (it; it != m_all_formulas.end(); ++it)
			{
				if (subConcatenation == it->x_Name)
				{
					for (int j = 0; j <= it->x_subFormula.size()-1 ; j++)
					{
						cout << it->x_subFormula[j].x_operation << it->x_subFormula[j].x_edge << it->x_subFormula[j].x_concatenation << "}";
						//m_single_formula.x_subFormula.erase(m_single_formula.x_subFormula.begin() + i);
						m_single_formula.x_subFormula[i].x_concatenation = m_single_formula.x_subFormula[i].x_concatenation[0];
						if (it->x_subFormula[j].x_operation=="={")
						{
							it->x_subFormula[j].x_operation = "";
						}
						m_single_formula.x_subFormula.insert(m_single_formula.x_subFormula.begin()+i+1,it->x_subFormula[j]);
						//m_single_formula.x_subFormula[i].x_operation =it->x_subFormula[j].x_operation;
						i++;
						//m_single_formula.x_subFormula[i+1] = it->x_subFormula[j];
					}
				}
			}
		}
	}
	//sQuasilinearEquations(m_single_formula);
	cout << "Changed Formulas:" << m_single_formula.x_Name;
	//cout << m_single_formula.x_subFormula.size();
	for (int l = 0; l != m_single_formula.x_subFormula.size(); l++){
		cout << m_single_formula.x_subFormula[l].x_operation << m_single_formula.x_subFormula[l].x_edge << m_single_formula.x_subFormula[l].x_concatenation;
	}
	cout <<"}"<< endl;
	
}

void gCombineFormula(vector<WholeFormula> m_all_formulas)
{
	vector<WholeFormula>::iterator it = m_all_formulas.begin();
	
	for (it; it != m_all_formulas.end(); it++)
	{
		vector<WholeFormula>::iterator it2 = m_all_formulas.begin();
		for (it2; it2 != m_all_formulas.end(); it2++)
		{
			for (int i = 0; i <= it->x_subFormula.size() - 1; i++)
			{
				string subConcatenation = it->x_subFormula[i].x_concatenation;
				if (subConcatenation != "")
				{
					string operation = "";
					operation += subConcatenation[0];
					subConcatenation = subConcatenation.substr(1, subConcatenation.size());
					cout << "find it" << endl;
					if (subConcatenation == it2->x_Name)
					{
						//for (int j = 0; j <= it2->x_subFormula.size(); j++)
							//it->x_subFormula.insert(it->x_subFormula.begin() + i, it2->x_subFormula[j]);
							//cout << it->x_subFormula[i].x_operation << it->x_subFormula[i].x_edge << it->x_subFormula[i].x_concatenation << endl;
					}
				}
			}
		}
	}
}

SyntaxNode *CreateSingleTree(SyntaxNode * father, string order_string)
{
	cout << "order string:"<<order_string << endl;
	int i = 0;
	bool formulaFlag = false;
	SyntaxNode * newNode = new SyntaxNode;
	newNode->father = father;
	newNode->x_formula = "";

	int bracket = 0;
	string leftChildString, rightChildString;

	while (order_string.c_str()[i] != '\0')
	{
		if (order_string.c_str()[i] >= '0'	&&	order_string.c_str()[i] <= '9')
		{
			//property_Value = property_Value * 10 + order_string.c_str()[i] - '0';
			i++;
		}

		else if (order_string.c_str()[i] == '{')
		{
			formulaFlag = true;
			bracket++;
			i++;
		}

		else if (order_string.c_str()[i] == '}')
		{
			formulaFlag = true;
			bracket--;
			i++;
		}

		else if (order_string.c_str()[i] == '~'	&&	 bracket == 0)
		{
			newNode->x_Symbol = '~';
			cout << "testSuccess" << endl;
			if (order_string.c_str()[i - 1] == '}')
			{
				leftChildString = order_string.substr(1, i - 2);

			}
			else
			{
				leftChildString = order_string.substr(0, i);
			}

			if (order_string.c_str()[i + 1] == '{')
			{
				rightChildString = order_string.substr(i + 2, order_string.length() - i - 3);
			}
			else
			{

				rightChildString = order_string.substr(i + 1, order_string.length() - i - 1);
			}


			newNode->leftchild = CreateSingleTree(father, leftChildString);
			newNode->rightchild = CreateSingleTree(father, rightChildString);
			cout << "LeftChild:" << leftChildString << ";RightChild:" << rightChildString << endl;
			formulaFlag = true;
			if (newNode == NULL)
			{
				break;
			}
			if (newNode->rightchild == NULL)
			{
				break;
			}
			i++;
		}
		else i++;
	}
    if (formulaFlag == false)
		 {
			 newNode->x_Symbol = 'k';
			 newNode->leftchild = NULL;
			 newNode->rightchild = NULL;

		 }
	return newNode;

}
