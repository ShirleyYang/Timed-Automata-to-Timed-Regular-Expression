#include <string>
#include <vector>
#include <map>

using namespace std;

//DS of locations in automata
class LocationNode
{
public:
	//the struct to save all information of transitions
	int id;
	bool isVisit;
	string source_id;
	string target_id;
	string guard;
	string update;
	string comments;
	string expression;
	LocationNode(){};
};

class SyntaxNode
{
public:
	string x_Symbol;// symbol of the node, including k, &, |, .,~
	string x_formula;
	bool x_bNode;
	string x_subID;

	SyntaxNode* leftchild;
	SyntaxNode* rightchild;
	SyntaxNode* father;
};

class RegularExpression
{
public:
	int x_id;
	bool x_simple;
	string  m_regular_expression;
};

class SubFormula
{
public:
	string x_edge;
	string x_concatenation;
	string x_operation;
	string x_left;
	string x_right;
	bool x_exchange;
};

class WholeFormula
{
public:
	int x_id;
	string x_Name;
	vector<SubFormula> x_subFormula;
	bool x_read;
};