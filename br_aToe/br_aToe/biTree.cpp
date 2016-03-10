#include "stdlib.h"
//#include "stdafx.h"
#include <stdio.h>
#include<iostream>
#include <string>
using namespace std;
#include "biTree.h";
#include <queue>


#include <stdio.h>

class biTree
{
private:
	//节点元素类型为结构体
	struct LinkNode
	{
		string data;
		LinkNode *left;
		LinkNode *right;
		LinkNode(const string& dat, LinkNode *l, LinkNode *r) :data(dat), left(l), right(r){}
	};

	LinkNode *head;//表头节点

	//添加节点
	void AddTreeNode(LinkNode *node, LinkNode *newNode);
	//显示中序排列
	void ShowCLR(LinkNode *root);
	//显示前序排列
	void ShowLCR(LinkNode *root);
	//显示右序排列
	void ShowLRC(LinkNode *root);
	//高度
	int  Height(LinkNode *root);

public:
	//添加节点
	bool AddTreeNode(string  data);
	//显示中序排列
	bool ShowCLR();
	//显示前序排列
	bool ShowLCR();
	//显示右序排列
	bool ShowLRC();
	//前序排列
	bool Floor();
	//最小值
	bool Min(int** minValue);
	//最大值
	bool Max(int** maxValue);
	//是否是空树
	bool Empty();
	//高度
	void Height(int** height);
	/*
	~biTree()
	{
	delete[] head;
	}

	biTree()
	{
	head = new LinkNode("o", NULL, NULL);
	}
	*/
};
//添加节点
void biTree::AddTreeNode(LinkNode *node, LinkNode *newNode)
{
	if (node->data > newNode->data)
	{
		if (node->left == NULL)
		{
			node->left = newNode;
		}
		else{
			AddTreeNode(node->left, newNode);
		}

	}
	else if (node->data < newNode->data)
	{
		if (node->right == NULL)
		{
			node->right = newNode;
		}
		else{
			AddTreeNode(node->right, newNode);
		}
	}

}

//添加节点
bool biTree::AddTreeNode(string data)
{
	LinkNode *node = new LinkNode(data, NULL, NULL);
	if (head->left == NULL)
	{
		head->left = node;
	}
	AddTreeNode(head->left, node);

	return true;
}

//中序遍历
void biTree::ShowCLR(LinkNode *root)
{
	if (root != NULL){
		cout <<"CLR Order:" <<root->data << "";
	}

	if (root->left != NULL)
	{
		ShowCLR(root->left);
	}

	if (root->right != NULL)
	{
		ShowCLR(root->right);
	}
}

//中序遍历
bool biTree::ShowCLR()
{
	if (Empty())
	{
		return false;
	}
	ShowCLR(head->left);

	return true;
}


//前序遍历
void biTree::ShowLCR(LinkNode *root)
{
	if (root->left != NULL)
	{
		ShowLCR(root->left);
	}

	if (root != NULL){
		cout << root->data << "  ";
	}

	if (root->right != NULL)
	{
		ShowLCR(root->right);
	}
}

//前序遍历
bool biTree::ShowLCR()
{
	if (Empty())
	{
		return false;
	}
	ShowLCR(head->left);

	return true;
}

//后序遍历
void biTree::ShowLRC(LinkNode *root)
{
	if (root->left != NULL)
	{
		ShowLRC(root->left);
	}

	if (root->right != NULL)
	{
		ShowLRC(root->right);
	}

	if (root != NULL){
		cout << root->data << "  ";
	}
}

//后序遍历
bool biTree::ShowLRC()
{
	if (Empty())
	{
		return false;
	}
	ShowLRC(head->left);

	return true;
}


//判断树是否为空
bool biTree::Empty()
{
	return head->left == NULL;
}

//用队列实现二叉树层序遍历
//1:添加根节点
//2:打印根节点的数据,添加根节点的子节点，弹出根节点
//3:循环第二步
bool biTree::Floor()
{
	queue<LinkNode*> q;
	LinkNode* cur = head->left;
	q.push(head->left);
	while (!q.empty())
	{
		cur = q.front();
		cout << cur->data << "  ";

		if (cur->left != NULL){
			q.push(cur->left);
		}
		if (cur->right != NULL)
		{
			q.push(cur->right);
		}
		q.pop();
	}
	return true;

}

//求两个数中较大的一个
int max(int a, int b)
{
	if (a > b)
	{
		return a;
	}
	else
	{
		return b;
	}
}

//递归求二叉树的高度
//二叉树的高度就是左子树和右子树中较大一颗二叉树的高度
int biTree::Height(LinkNode *root)
{
	if (root == NULL)
	{
		return 0;
	}
	return 1 + max(Height(root->left), Height(root->right));
}

//用指向指针的指针接受二叉树的高度
void biTree::Height(int** height)
{
	**height = Height(head->left);
}
