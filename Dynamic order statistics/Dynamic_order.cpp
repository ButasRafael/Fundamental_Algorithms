#include <iostream>
#include "Profiler.h"
#include <cstdlib>
#include <ctime>
using namespace std;
Profiler prof("Count_Operations");

typedef struct Node {
	Node* left;
	Node* right;
	Node* parent;
	int value;
	int size;
}node;

node* divide(int left, int right, node *parent, Operation&op)
{
	if (left > right)
		return NULL;
	node *c = (node*)malloc(sizeof(node));
	int mid = (left + right) / 2;
	c->value = mid;
	c->size = right-left + 1;
	op.count();
	c->parent = parent;
	c->left = divide(left, mid - 1, c,op);
	c->right = divide(mid + 1, right, c,op);
	return c;
}

node* Build_Tree(int size, Operation&op)
{
	if (size < 0)
	{
		op.count();
		return NULL;
	}
	node* root = (node*)malloc(sizeof(node));
	int mid = size / 2;
	root->value = mid;
	root->parent = NULL;
	root->size = size + 1;
	op.count();
	root->left = divide(0, mid - 1, root,op);
	root->right = divide(mid + 1, size, root,op);
	return root;
}

node* OS_Select(node* root, int i, Operation&op)
{
	if (root == NULL)
		return NULL;
	int rank;
	if (root->left != NULL)
	{
		rank = root->left->size + 1;
		op.count();
	}
	else
	{
		rank = 1;
		op.count();
	}
	op.count();
	if (i == rank)
	{
		return root;
	}
	op.count();
	if (i < rank)
	{
		return OS_Select(root->left, i, op);
	}
	return OS_Select(root->right, i - rank, op);
}

void Size_Update(node* root, Operation& op)
{
	while (root != NULL)
	{
		root->size--;
		op.count();
		root = root->parent;
	}
}

void Transplant(node *&root,node* x, node* y)
{
	if (x == root)
	{
		root = y;
		if (y != NULL)
			y->parent = NULL;
	}
	else
	{
		if (x == x->parent->left)
			x->parent->left = y;
		else
			if (x == x->parent->right)
				x->parent->right = y;
		if (y != NULL)
			y->parent = x->parent;
	}

}
node* OS_Delete(node* &root,int i, Operation&op)
{
	node* ToDelete = OS_Select(root, i,op);
	if (ToDelete != NULL)
	{
		if (ToDelete->left == NULL)
		{
			Transplant(root, ToDelete, ToDelete->right);
			Size_Update(ToDelete, op);
			free(ToDelete);
		}
		else
			if (ToDelete->right == NULL)
			{
				Transplant(root, ToDelete, ToDelete->left);
				Size_Update(ToDelete, op);
				free(ToDelete);
			}
			else
			{
				node* successor = ToDelete->right;
				while (successor->left != NULL)
					successor = successor->left;
				if (successor->parent != ToDelete)
				{
					Transplant(root, successor, successor->right);
					successor->right = ToDelete->right;
					successor->right->parent = successor;
				}
				Transplant(root, ToDelete, successor);
				successor->left = ToDelete->left;
				successor->left->parent = successor;
				successor->size = ToDelete->size;
				Size_Update(successor, op);
				free(ToDelete);

			}
	}

	return root;

}

void perf()
{
	int increment = 100;
	int start_size = 100;
	int end_size = 10000;
	int number_exp = 5;
	int i, j, n;
	node* root = { NULL };
	node* search = { NULL };
	node* root_after_deletion = { NULL };
	for (int experiment = 0; experiment < number_exp; experiment++)
	{
		for (int size = start_size; size <= end_size; size += increment)
		{
			Operation op_build = prof.createOperation("Operations_Build", size);
			Operation op_select = prof.createOperation("Operations_Select", size);
			Operation op_delete = prof.createOperation("Operations_Delete", size);
			root=Build_Tree(size,op_build);
			n = size;
			for (int i = 0; i < size; i++)
			{
				j = rand() % n+1;
				search=OS_Select(root, j,op_select);
				root_after_deletion=OS_Delete(root, j,op_delete);
				n--;
			}
		}
	}
	prof.divideValues("Operations_Build", number_exp);
	prof.divideValues("Operations_Select", number_exp);
	prof.divideValues("Operations_Delete", number_exp);
	prof.createGroup("Total Operations", "Operations_Build", "Operations_Select", "Operations_Delete");
	prof.showReport();

}

void Pretty_Print(node* root, int spaces)
{
	if (root == NULL)
		return;
	if (root != NULL)
	{
		for (int i = 0; i < spaces; i++)
		{
			cout << " ";
		}
		cout << root->value << ": " << root->size << endl;
		Pretty_Print(root->left, spaces + 6);
		Pretty_Print(root->right, spaces + 6);
	}
}

void Build_Tree_Demo()
{
	int size = 20;
	Operation dummy = prof.createOperation("Operations_Dummy", size);
	node*root=Build_Tree(size,dummy);
	cout << "Initial Tree:" << endl << endl;
	Pretty_Print(root, 0);
	cout << endl << endl;


}

void OS_Select_Demo()
{
	int size = 20;
	Operation dummy = prof.createOperation("Operations_Dummy", size);
	int k = 5;
	node* root = Build_Tree(size,dummy);
	node* selectedNode = OS_Select(root, k,dummy);
	cout << "The " << k << "th smallest element is: " << selectedNode->value << endl << endl;


}

void OS_Delete_Demo()
{
	int size = 20;
	Operation dummy = prof.createOperation("Operations_Dummy", size);
	node* root = Build_Tree(size,dummy);
	int deleteIndex = 5;
	cout << "Deleting the " << deleteIndex << "th smallest element..." << endl << endl;
	root = OS_Delete(root, deleteIndex,dummy);
	cout << "Tree after deletion:" << endl;
	Pretty_Print(root, 0);

}

int main()
{
	Build_Tree_Demo();
	OS_Select_Demo();
	OS_Delete_Demo();
	//srand(time(NULL));
	//perf();
}