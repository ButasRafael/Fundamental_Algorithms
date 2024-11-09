/*

* First, i will talk about the algorithms and what are they doing.
* In the first represenation,the parent representation, we have an array that represents the parent vector(for each index, the value represents the parent of that index).
* We are required to print this array as a tree using pretty print.
 
* For the second representaion, we need to transform this parent representation into a multi-way representation.
* To do this, from the initial array, i created a vector of nodes, in which each node has a value,a vector with all of his childs and the sizw of the child vector.
* Initially, the node array contained only the indexes, size of the child array being 0.
* Then, using one traversal of the original array, we add the current index to the childs vector of the current value and we increase the size of the child vector.
* If the current value is  -1 it means that we arrived at the root, and we remember it and do nothing because at the root we will have the entire tree using this method.
* Finally, we do a pretty print starting from the root, in which we print the root value, and we call the pretty print function recursively for the current elements child vector(which contains all the elements).
 
* For the third representation, we need to transform the multi-way representation into a binary representation.
* To do this, we create another class in wich each node contains the value and two pointers, one to the first child(the left one) and one to the right sibling.
* First, strating from the root, in each call we transform the current element from the second representation class to the third representation class.
* Then, if it has childrens(we check in the second repr class), we go as left as we can and for each node we store the left children in the left pointer.
* When we are done going to the left(we reached the leaf), thanks to recursion, a pointer will be created to walk through each sibling of the current node starting from the end to the beginning and a call will be made with the current element being each sibling.
* In this way, the first childs are created going as deep in the left as we can, and then we come back up recursively, creating their siblings (and the siblings childs and siblings if any).

* Finally ,we do another pretty print starting from the root, in which we print the root value, and we call the pretty print function recursively for the current elements first child and right sibling.

* Now lets talk about the space and time efficiency of the two transformations.

* In the first transformation, we traverse the initial array once and we create for each node of node array, the child vector so the time complexity is O(n).
* Also, in this transformation, we used aditional memory to store the nodes and to store the child vector for each node which has different sizes so the space complexity is n times a constant representing the medium number of childs for each node, so still O(n).

* In the second transformation, maximum recursion calls is n in case of a skewed tree, which means the timpe complexity is also O(n).
* Also, we used less aditional memory in this transformation, we just transformed each current node from one represenation to another and we stored only references, so space complexity is O(n) but less then in the first transformation.
 
*/

#include <iostream>
using namespace std;

typedef struct R2 {
	int value;
	R2** child;
	int size;
}r2;

typedef struct R3 {
	int value;
	R3* left;
	R3* right;
}r3;

r2** Create_Node_Array(r2**node,int size)
{
	for (int i = 1; i < size; i++)
	{
		r2* temp = (r2*)malloc(sizeof(r2));
		temp->value = i;
		temp->child = (r2**)malloc(sizeof(r2));
		temp->size = 0;
		node[i] = temp;
	}
	return node;
}

void Add_Child_T2(int* a, int size, int i, r2** node)
{
	node[a[i]]->child[node[a[i]]->size] = node[i];
	node[a[i]]->size++;
}

r3* Add_Node_T3(int value)
{
	r3* temp = (r3*)malloc(sizeof(r3));
	temp->value = value;
	temp->left = NULL;
	temp->right = NULL;
	return temp;
}

r2* T_12(int* a, int size)
{
	
	r2* node[10] = { NULL };
	r2* root = NULL;
	Create_Node_Array(node, size);
	for (int i = 1; i < size; i++)
	{
		if (a[i] != -1)
			Add_Child_T2(a, size, i, node);
		else
			root = node[i];
	}
		return root;

}
	 
r3* T_23(r2* current_r2)
{
	r3* current_r3 = Add_Node_T3(current_r2->value);
	if (current_r2->size > 0)
	{
		current_r3->left = T_23(current_r2->child[0]);
		r3* ptr = current_r3->left;
		for (int k = 1; k < current_r2->size; k++)
		{
			ptr->right = T_23(current_r2->child[k]);
			ptr = ptr->right;
		}
	}
	return current_r3;
}

void PP1(int* a, int size, int parent, int spaces)
{
	for (int i = 1; i <= size; i++)
	{

		if (a[i] == parent)
		{
			for (int j = 0; j < spaces; j++)
			{
				cout << "  ";
			}
			cout << i << endl;
			PP1(a, size, i, spaces + 2);
		}


	}
}

void PP2(r2* root,int spaces)
{

	if (root == NULL)
		return;
	for (int i = 0; i < spaces; i++)
	{
		cout << "  ";
	}
	cout << root->value << endl;
	for (int i = 0; i < root->size; i++)
	{
		PP2(root->child[i], spaces + 2);
	}

}

void PP3(r3* root, int spaces)
{
	if (root == NULL)
		return;
	if (root != NULL)
	{
		for (int i = 0; i < spaces; i++)
		{
			cout << "  ";
		}
		cout << root->value << endl;
		PP3(root->left, spaces + 2);
		PP3(root->right, spaces);
	}
}

int main()
{
	int a[] = {0,2,7,5,2,7,7,-1,5,2};
	int size = sizeof(a) / sizeof(a[0]);
	cout << endl << endl;
	cout << "Pretty print for first represenation:";
	cout << endl << endl;
	PP1(a, size, -1, 0);
	cout << endl << endl;
	r2* root2 = T_12(a, size);
	cout << "Pretty print for second represenation:";
	cout << endl << endl;
	PP2(root2, 0);
	cout << endl << endl;
	r3* root3 = T_23(root2);
	cout << "Pretty print for third represenation:";
	cout << endl << endl;
	PP3(root3, 0);
}
