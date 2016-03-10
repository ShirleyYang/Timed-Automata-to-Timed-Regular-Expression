#include <string>
#include <vector>
#include <map>
#include <algorithm>
#include <iterator>
#include "stdlib.h" 
#include <stdio.h>
using namespace std;


template<class T>       
struct TreeNode
{
	T data; 
	T symbol;
	TreeNode <T> *Lchild, *Rchild;

	/*TreeNode(T nodeValue = T(),TreeNode<T> *leftNode = NULL,TreeNode<T> *rightNode = NULL )
	:data(nodeValue),Lchild(leftNode),Rchild(rightNode){} */
};

//**************************************************************************************
//�������Ľ���
template <class T>
void createBinaryTree(TreeNode<T> *&root, T nodeValue)  //����ָ�������
{
	TreeNode<T>* p = root;
	if (nodeValue == "")
	{
		root = NULL;
	}
	else
	{
		root = new TreeNode<T>();            //����һ���ڵ�
		root->data = nodeValue;
		createBinaryTree(root->Lchild, root->Lchild->data);    //�ݹ鹹��������
		createBinaryTree(root->Rchild, root->Rchild->data);    //�ݹ鹹��������
	}
}


template <class T>
void insert(T data)
{
	root = insert(root, data);
}

template <class T>
TreeNode<T> insert(TreeNode<T> *&root, T nodeValue)
{
	
}
//************************************************************************************
//���������������
template <class T>
void preOrder(TreeNode<T> * & p) //����ָ�������
{
	if (p)
	{
		cout << p->data << " ";
		preOrder(p->Lchild);
		preOrder(p->Rchild);
	}
}

//**************************************************************************************
//���������������
template <class T>
void inOrder(TreeNode<T> * & p) //����ָ�������
{

	if (p)
	{
		inOrder(p->Lchild);
		cout << p->data << " ";
		inOrder(p->Rchild);
	}
}

//**************************************************************************************
//�������ĺ������
template <class T>
void postOrder(TreeNode<T> *& p) //����ָ�������
{
	if (p)
	{
		postOrder(p->Lchild);
		postOrder(p->Rchild);
		cout << p->data << " ";
	}
}

//*************************************************************************************
//ͳ�ƶ������н��ĸ���
template<class T>
int countNode(TreeNode<T> * & p) //����ָ�������
{
	if (p == NULL) return 0;
	return 1 + countNode(p->Lchild) + countNode(p->Rchild);
}

//***********************************************************************************
//������������
template<class T>
int depth(TreeNode<T> *& p) //����ָ�������
{
	if (p == NULL)
		return -1;
	int h1 = depth(p->Lchild);
	int h2 = depth(p->Rchild);
	if (h1>h2)return (h1 + 1);
	return h2 + 1;
}

//***********************************************************************************
//�����������ٲ���
//���׻����Ĵ���������void destroy(TreeNode<T>* p) ���������ᴴ��һ���ֲ�����ʱ���������洫�ݵ�ָ��
//��Ȼ2��ָ�붼ִ��ͬһ��ѿռ䣬delete�ֲ�ָ�� Ҳ��ɾ���������ṹ��ռ�õĶ��ڴ�
//����ȫ�ִ��ݵ��Ǹ�ָ�뽫��������ָ�룬���������Ԥ�ϵĴ���
//void destroy(TreeNode<T> *& p) �˺����Ĳ���Ϊȫ��ָ���һ������������ȫ��ָ��rootNode����
//  ����p = NULL;�ܴﵽ�ÿ�ָ�������
//��ѡ�ķ������ڵ�����destroy����֮������������ִ��rootNode = NULL����
template<class T>
void destroy(TreeNode<T> *& p)  //����ָ�������,���ٺ������������ٶ������еĸ������
{
	if (p)
	{
		//���� return֮�� û��ִ��delete p
		//return destroy(p->Lchild);
		//return destroy(p->Rchild);

		destroy(p->Lchild);
		destroy(p->Rchild);

		//deleteֻ���ͷ����û�ͨ��new��ʽ�ڶ���������ڴ棬
		//��ͨ�����������ķ�ʽ��ϵͳ��������ջ�ڴ治��ʹ��deleteɾ��

		//delete��free����һ�������޸���������Ӧָ��ָ������ݣ�Ҳ���޸�ָ�뱾��
		//ֻ���ڶ��ڴ����ṹ�н�ָ��ָ������ݱ��Ϊ�ɱ����·���
		delete p;

		//�����ڴ��ͷ� ջ��ָ�벢������
		//��ʱpָ��ĵ�ַδ֪,��ʱִ��*p = ? �����ᵼ�²���Ԥ�ϵĴ���
		//���ǿ������¸�ֵp = &x;
		//���delete֮���P�ÿ�
		p = NULL;

	}
}

//********************************************************************************