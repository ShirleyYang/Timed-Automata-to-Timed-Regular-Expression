#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include "stdlib.h" 
#include <stdio.h>
using namespace std;

struct nodeTree {
	char symbol;//'|', '.', '~', 'e'means edges, and 'x' means x formula
	string data;
	string rootName;
	struct nodeTree* left;
	struct nodeTree* right;
};

static int lookup(struct  nodeTree* node, string target) {
	// 1. Base case == empty tree 
	// in that case, the target is not found so return false 
	if (node == NULL) {
		return(false);
	}
	else {
		// 2. see if found here 
		if (target == node->data) return(true);
		else {
			// 3. otherwise recur down the correct subtree 
			if (target <  node->data) return(lookup(node->left, target));
			else return(lookup(node->right, target));
		}
	}
}

/*
Helper function that allocates a new node
with the given data and NULL left and right
pointers.
*/
struct  nodeTree* newNode(string data, char symbol) {
	struct  nodeTree* node = new(struct nodeTree);    // "new" is like "malloc" 
	node->data = data;
	node->left = NULL;
	node->right = NULL;


	return(node);
}


/*
Give a binary search tree and a number, inserts a new node
with the given number in the correct place in the tree.
Returns the new root pointer which the caller should
then use (the standard trick to avoid using reference
parameters).
*/
bool flag = true;
struct  nodeTree* insert(struct nodeTree* node, string data, char symbol) {
	// 1. If the tree is empty, return a new, single node 
	if (node == NULL) {
		return(newNode(data,symbol));
	}
	else {
		// 2. Otherwise, recur down the tree 
		if (flag == true) {
			node->left = insert(node->left, data, symbol);
			flag = false;
		}
		else {
			node->right = insert(node->right, data, symbol);
			flag = true;
		}

		return(node); // return the (unchanged) node pointer 
	}
}

int maxDepth(struct  nodeTree* node) {
	if (node == NULL) {
		return(0);
	}
	else {
		// compute the depth of each subtree 
		int lDepth = maxDepth(node->left);
		int rDepth = maxDepth(node->right);
		// use the larger one 
		if (lDepth > rDepth) return(lDepth + 1);
		else return(rDepth + 1);
	}
}

/*
Given a non-empty binary search tree,
return the minimum data value found in that tree.
Note that the entire tree does not need to be searched.
*/
string minValue(struct  nodeTree* node) {
	struct  nodeTree* current = node;
	// loop down to find the leftmost leaf 
	while (current->left != NULL) {
		current = current->left;
	}

	return(current->data);
}

/*
Given a binary search tree, print out
its data elements in increasing
sorted order.
*/
void printTree(struct  nodeTree* node) {
	if (node == NULL) return;
	printTree(node->left);
	cout<<node->data<<"";
	printTree(node->right);
}

void printPreOrder(struct nodeTree* node){
	if (node == NULL) return;
	cout << node->data;
	printPreOrder(node->left);
	printPreOrder(node->right);
}

void printInOrder(struct nodeTree* node){
	if (node == NULL) return;
	cout << "(";
	printPreOrder(node->left);
	cout << node->data;
	printPreOrder(node->right);
	cout << ")";
}

/*
Given a binary tree, print its
nodes according to the "bottom-up"
postorder traversal.
*/
void printPostorder(struct  nodeTree* node) {
	if (node == NULL) return;
	// first recur on both subtrees 
	printTree(node->left);
	printTree(node->right);

	// then deal with the node 
	cout << node->data << "";
}

/*
Change a tree so that the roles of the
left and right pointers are swapped at every node.
So the tree...
4
/ \
2   5
/ \
1   3

is changed to...
4
/ \
5   2
/ \
3   1
*/
void mirror(struct  nodeTree* node) {
	if (node == NULL) {
		return;
	}
	else {
		struct  nodeTree* temp;

		// do the subtrees 
		mirror(node->left);
		mirror(node->right);

		// swap the pointers in this node 
		temp = node->left;
		node->left = node->right;
		node->right = temp;
	}
}

void 