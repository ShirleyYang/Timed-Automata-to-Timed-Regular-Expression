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
//二叉树的建立
template <class T>
void createBinaryTree(TreeNode<T> *&root, T nodeValue)  //传递指针的引用
{
	TreeNode<T>* p = root;
	if (nodeValue == "")
	{
		root = NULL;
	}
	else
	{
		root = new TreeNode<T>();            //构造一个节点
		root->data = nodeValue;
		createBinaryTree(root->Lchild, root->Lchild->data);    //递归构造左子树
		createBinaryTree(root->Rchild, root->Rchild->data);    //递归构造右子树
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
//二叉树的先序遍历
template <class T>
void preOrder(TreeNode<T> * & p) //传递指针的引用
{
	if (p)
	{
		cout << p->data << " ";
		preOrder(p->Lchild);
		preOrder(p->Rchild);
	}
}

//**************************************************************************************
//二叉树的中序遍历
template <class T>
void inOrder(TreeNode<T> * & p) //传递指针的引用
{

	if (p)
	{
		inOrder(p->Lchild);
		cout << p->data << " ";
		inOrder(p->Rchild);
	}
}

//**************************************************************************************
//二叉树的后序遍历
template <class T>
void postOrder(TreeNode<T> *& p) //传递指针的引用
{
	if (p)
	{
		postOrder(p->Lchild);
		postOrder(p->Rchild);
		cout << p->data << " ";
	}
}

//*************************************************************************************
//统计二叉树中结点的个数
template<class T>
int countNode(TreeNode<T> * & p) //传递指针的引用
{
	if (p == NULL) return 0;
	return 1 + countNode(p->Lchild) + countNode(p->Rchild);
}

//***********************************************************************************
//求二叉树的深度
template<class T>
int depth(TreeNode<T> *& p) //传递指针的引用
{
	if (p == NULL)
		return -1;
	int h1 = depth(p->Lchild);
	int h2 = depth(p->Rchild);
	if (h1>h2)return (h1 + 1);
	return h2 + 1;
}

//***********************************************************************************
//二叉树的消毁操作
//容易混淆的错误声明：void destroy(TreeNode<T>* p) 这种声明会创建一个局部的临时对象来保存传递的指针
//虽然2个指针都执行同一块堆空间，delete局部指针 也会删除二叉树结构所占用的堆内存
//但是全局传递的那个指针将会是垃圾指针，会产生不可预料的错误
//void destroy(TreeNode<T> *& p) 此函数的参数为全局指针的一个别名，代表全局指针rootNode本身
//  这样p = NULL;能达到置空指针的左右
//可选的方案是在调用完destroy方法之后，在主函数中执行rootNode = NULL操作
template<class T>
void destroy(TreeNode<T> *& p)  //传递指针的引用,消毁函数，用来消毁二叉树中的各个结点
{
	if (p)
	{
		//错误 return之后 没有执行delete p
		//return destroy(p->Lchild);
		//return destroy(p->Rchild);

		destroy(p->Lchild);
		destroy(p->Rchild);

		//delete只能释放由用户通过new方式在堆中申请的内存，
		//是通过变量声明的方式由系统所声明的栈内存不能使用delete删除

		//delete和free函数一样，不修改它参数对应指针指向的内容，也不修改指针本身，
		//只是在堆内存管理结构中将指针指向的内容标记为可被重新分配
		delete p;

		//堆上内存释放 栈上指针并不销毁
		//此时p指向的地址未知,此时执行*p = ? 操作会导致不可预料的错误
		//但是可以重新赋值p = &x;
		//最好delete之后把P置空
		p = NULL;

	}
}

//********************************************************************************