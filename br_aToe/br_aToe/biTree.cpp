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
	//�ڵ�Ԫ������Ϊ�ṹ��
	struct LinkNode
	{
		string data;
		LinkNode *left;
		LinkNode *right;
		LinkNode(const string& dat, LinkNode *l, LinkNode *r) :data(dat), left(l), right(r){}
	};

	LinkNode *head;//��ͷ�ڵ�

	//��ӽڵ�
	void AddTreeNode(LinkNode *node, LinkNode *newNode);
	//��ʾ��������
	void ShowCLR(LinkNode *root);
	//��ʾǰ������
	void ShowLCR(LinkNode *root);
	//��ʾ��������
	void ShowLRC(LinkNode *root);
	//�߶�
	int  Height(LinkNode *root);

public:
	//��ӽڵ�
	bool AddTreeNode(string  data);
	//��ʾ��������
	bool ShowCLR();
	//��ʾǰ������
	bool ShowLCR();
	//��ʾ��������
	bool ShowLRC();
	//ǰ������
	bool Floor();
	//��Сֵ
	bool Min(int** minValue);
	//���ֵ
	bool Max(int** maxValue);
	//�Ƿ��ǿ���
	bool Empty();
	//�߶�
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
//��ӽڵ�
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

//��ӽڵ�
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

//�������
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

//�������
bool biTree::ShowCLR()
{
	if (Empty())
	{
		return false;
	}
	ShowCLR(head->left);

	return true;
}


//ǰ�����
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

//ǰ�����
bool biTree::ShowLCR()
{
	if (Empty())
	{
		return false;
	}
	ShowLCR(head->left);

	return true;
}

//�������
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

//�������
bool biTree::ShowLRC()
{
	if (Empty())
	{
		return false;
	}
	ShowLRC(head->left);

	return true;
}


//�ж����Ƿ�Ϊ��
bool biTree::Empty()
{
	return head->left == NULL;
}

//�ö���ʵ�ֶ������������
//1:��Ӹ��ڵ�
//2:��ӡ���ڵ������,��Ӹ��ڵ���ӽڵ㣬�������ڵ�
//3:ѭ���ڶ���
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

//���������нϴ��һ��
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

//�ݹ���������ĸ߶�
//�������ĸ߶Ⱦ������������������нϴ�һ�Ŷ������ĸ߶�
int biTree::Height(LinkNode *root)
{
	if (root == NULL)
	{
		return 0;
	}
	return 1 + max(Height(root->left), Height(root->right));
}

//��ָ��ָ���ָ����ܶ������ĸ߶�
void biTree::Height(int** height)
{
	**height = Height(head->left);
}
