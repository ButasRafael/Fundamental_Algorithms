#include <iostream>
#include "Profiler.h"
#include <cstdlib>
#include <ctime>
using namespace std;
Profiler prof("BST_Traversals");

typedef struct Node {
	int value;
	Node* left;
	Node* right;
}node;

node* divide(int left, int right)
{
	if (left > right)
		return NULL;
	node* c = (node*)malloc(sizeof(node));
	int mid = (left + right) / 2;
	c->value = mid;
	c->left = divide(left, mid - 1);
	c->right = divide(mid + 1, right);
	return c;
}

node* Build_Tree(int size)
{
	if (size < 0)
		return NULL;
	node* root = (node*)malloc(sizeof(node));
	int mid = size / 2;
	root->value = mid;
	root->left = divide(0, mid - 1);
	root->right = divide(mid + 1, size);
	return root;
}


void Print_Preorder(node* root,Operation &op)
{
	if (root == NULL)
		return;
	op.count();
	cout << root->value << " ";
	Print_Preorder(root->left,op);
	Print_Preorder(root->right,op);
}

void Print_Inorder(node* root, Operation& op)
{
	if (root == NULL)
		return;
	Print_Inorder(root->left,op);
	op.count();
	cout << root->value << " ";
	Print_Inorder(root->right,op);
}

void Print_Postorder(node* root, Operation& op)
{
	if (root == NULL)
		return;
	Print_Postorder(root->left,op);
	Print_Postorder(root->right,op);
	op.count();
	cout << root->value << " ";
}

void Iterative_Traversal(node* root, Operation& op)
{
	node* current, * pre;

	if (root == NULL)
		return;

	current = root;
	while (current != NULL) {

		if (current->left == NULL) 
		{
			op.count();
			cout << current->value << " ";
			current = current->right;
		}
		else 
		{

			pre = current->left;
			while (pre->right != NULL && pre->right != current)
				pre = pre->right;

			if (pre->right == NULL)
			{
				pre->right = current;
				current = current->left;
			}
			else 
			{
				pre->right = NULL;
				op.count();
				cout << current->value << " ";
				current = current->right;
			} 
		} 
	} 
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
	cout << root->value << endl;
		Pretty_Print(root->left, spaces + 6);
		Pretty_Print(root->right, spaces + 6);
	}
}

void Insertion_Sort(int* a, int size,int left,int right, Operation& comp, Operation& assig)
{
	int buff;
	int j;
	for (int i = left+1; i < right+1; i++)
	{
		assig.count();
		buff = a[i];
		j = i - 1;
		while (j >= left && a[j] > buff)
		{
			comp.count();
			assig.count();
			a[j + 1] = a[j];
			j--;

		}
		comp.count();
		assig.count();
		a[j + 1] = buff;
	}
}

int PivotIndex(int* a, int left, int right, Operation& comp, Operation& assig)
{
	assig.count();
	int pivot = a[right];
	int i = (left - 1);
	for (int j = left; j <= right - 1; j++)
	{
		comp.count();
		if (a[j] <= pivot)
		{
			i++;
			assig.count(3);
			swap(a[i], a[j]);
		}
	}
	assig.count(3);
	swap(a[i + 1], a[right]);
	return (i + 1);
}

void QuickSort(int* a, int size, int left, int right)
{
	Operation comp = prof.createOperation("Comparisons_QuickSort", size);
	Operation assig = prof.createOperation("Assignments_QuickSort", size);
	if (left < right)
	{
		int pivot = PivotIndex(a, left, right, comp, assig);
		QuickSort(a, size, left, pivot - 1);
		QuickSort(a, size, pivot + 1, right);
	}
}

void Hybrid_QuickSort(int* a, int size, int left, int right,int threshold)
{
	Operation comp = prof.createOperation("Comparisons_Hybrid_QuickSort", size);
	Operation assig = prof.createOperation("Assignments_Hybrid_QuickSort", size);
	if (left < right)
	{
		if (right - left + 1 < threshold)
		{
			Insertion_Sort(a, size, left, right,comp,assig);
		}
		else
		{
			int pivot = PivotIndex(a, left, right, comp, assig);
			Hybrid_QuickSort(a, size, left, pivot - 1,threshold);
			Hybrid_QuickSort(a, size, pivot + 1, right,threshold);
		}
	}
}

void Hybrid_QuickSort_parameters(int* a, int size, int left, int right, int threshold, Operation& comp, Operation& assig)
{
	if (left < right)
	{
		if (right - left + 1 < threshold)
		{
			Insertion_Sort(a, size, left, right, comp, assig);
		}
		else
		{
			int pivot = PivotIndex(a, left, right, comp, assig);
			Hybrid_QuickSort_parameters(a, size, left, pivot - 1, threshold,comp,assig);
			Hybrid_QuickSort_parameters(a, size, pivot + 1, right, threshold,comp,assig);
		}
	}
}

void QuickSort_parameters(int* a, int size, int left, int right, Operation& comp, Operation& assig)
{
	if (left < right)
	{
		int pivot = PivotIndex(a, left, right, comp, assig);
		QuickSort_parameters(a, size, left, pivot - 1,comp,assig);
		QuickSort_parameters(a, size, pivot + 1, right,comp,assig);
	}
}

void Insertion_Sort_NO_OP(int* a, int size, int left, int right)
{
	int buff;
	int j;
	for (int i = left + 1; i < right + 1; i++)
	{
		buff = a[i];
		j = i - 1;
		while (j >= left && a[j] > buff)
		{
			a[j + 1] = a[j];
			j--;

		}
		a[j + 1] = buff;
	}
}

int PivotIndex_NO_OP(int* a, int left, int right)
{
	int pivot = a[right];
	int i = (left - 1);
	for (int j = left; j <= right - 1; j++)
	{
		if (a[j] <= pivot)
		{
			i++;
			swap(a[i], a[j]);
		}
	}
	swap(a[i + 1], a[right]);
	return (i + 1);
}

void QuickSort_NO_OP(int* a, int size, int left, int right)
{
	if (left < right)
	{
		int pivot = PivotIndex_NO_OP(a, left, right);
		QuickSort_NO_OP(a, size, left, pivot - 1);
		QuickSort_NO_OP(a, size, pivot + 1, right);
	}
}

void Hybrid_QuickSort_NO_OP(int* a, int size, int left, int right,int threshold)
{
	if (left < right)
	{
		if (right - left + 1 < threshold)
		{
			Insertion_Sort_NO_OP(a, size, left, right);
		}
		else
		{
			int pivot = PivotIndex_NO_OP(a, left, right);
			Hybrid_QuickSort_NO_OP(a, size, left, pivot - 1,threshold);
			Hybrid_QuickSort_NO_OP(a, size, pivot + 1, right,threshold);
		}
	}
}

void QuickSort_threshold(int* a, int size, int left, int right)
{
	Operation comp = prof.createOperation("Comparisons_QuickSort_Threshold", size);
	Operation assig = prof.createOperation("Assignments_QuickSort_Threshold", size);
	if (left < right)
	{
		int pivot = PivotIndex(a, left, right, comp, assig);
		QuickSort_threshold(a, size, left, pivot - 1);
		QuickSort_threshold(a, size, pivot + 1, right); 
	}
}

void Insertion_Sort_threshold(int* a, int size, int left, int right)
{
	int buff;
	int j;
	Operation comp = prof.createOperation("Comparisons_InsertionSort_Threshold", size);
	Operation assig = prof.createOperation("Assignments_InsertionSort_Threshold", size);
	for (int i = left + 1; i < right + 1; i++)
	{
		
		assig.count();
		buff = a[i];
		j = i - 1;
		while (j >= left && a[j] > buff)
		{
			comp.count();
			assig.count();
			a[j + 1] = a[j];
			j--;

		}
		comp.count();
		assig.count();
		a[j + 1] = buff;
	}
}

void demo_Traversals()
{
	int size = 20;
	Operation dummy = prof.createOperation("Operations_Demo", size);
	node* root = Build_Tree(size);
	cout << "Initial Tree:" << endl << endl;
	Pretty_Print(root, 0);
	cout << endl << endl;
	cout << "PreOrder Traversal:" << endl << endl;
	Print_Preorder(root, dummy);
	cout << endl << endl;
	cout << "InOrder Traversal:" << endl << endl;
	Print_Inorder(root, dummy);
	cout << endl << endl;
	cout << "PostOrder Traversal:" << endl << endl;
	Print_Postorder(root, dummy);
	cout << endl << endl;
	cout << "Iterative Traversal:" << endl << endl;
	Iterative_Traversal(root, dummy);
	cout << endl << endl;
}
void demo_Insertion_Sort()
{
	int a[] = { 25,24,33,39,3,18,19,31,23,49,45,16,1,29,40,22,15,20,24,4,13,34,67,45,39,0,89,69,42,47,46,49,100 };
	int size = 33;
	Operation comp = prof.createOperation("Comparisons_Demo", size);
	Operation assig = prof.createOperation("Assignments_Demo", size);
	Insertion_Sort(a, size, 0, size - 1,comp,assig);
	cout << "This is the sorted array using Iterative Insertion Sort: ";
	for (int i = 0; i < size; i++)
		cout << a[i] << " ";
	cout << endl;
}

void demo_QuickSort()
{
	int a[] = { 25,24,33,39,3,18,19,31,23,49,45,16,1,29,40,22,15,20,24,4,13,34,67,45,39,0,89,69,42,47,46,49,100 };
	int size = 33;
	QuickSort(a, size, 0, size - 1);
	cout << "This is the sorted array using Quick Sort: ";
	for (int i = 0; i < size; i++)
		cout << a[i] << " ";
	cout << endl;
}

void demo_Hybrid_QuickSort()
{
	int a[] = { 25,24,33,39,3,18,19,31,23,49,45,16,1,29,40,22,15,20,24,4,13,34,67,45,39,0,89,69,42,47,46,49,100 };
	int size = 33;
	Hybrid_QuickSort(a, size, 0, size - 1,17);
	cout << "This is the sorted array using Hybrid Quick Sort: ";
	for (int i = 0; i < size; i++)
		cout << a[i] << " ";
	cout << endl;
}


void perf_traversals()
{
	int a[10001];
	int increment = 100;
	int start_size = 100;
	int end_size = 10000;
	int number_exp = 5;
	node* root = { NULL };
	for (int experiment = 0; experiment < number_exp; experiment++)
	{
		for (int size = start_size; size <= end_size; size += increment)
		{
			Operation op_preorder = prof.createOperation("Operations_PreOrder_Traversal", size);
			Operation op_inorder = prof.createOperation("Operations_InOrder_Traversal", size);
			Operation op_postorder = prof.createOperation("Operations_PostOrder_Traversal", size);
			Operation op_iterative = prof.createOperation("Operations_Iterative_Traversal", size);
			root = Build_Tree(size);
			Print_Preorder(root, op_preorder);
			Print_Inorder(root, op_inorder);
			Print_Postorder(root, op_postorder);
			Iterative_Traversal(root, op_iterative);
			cout << size << " ";
		}
	}
	prof.divideValues("Operations_PreOrder_Traversal", number_exp);
	prof.divideValues("Operations_InOrder_Traversal", number_exp);
	prof.divideValues("Operations_PostOrder_Traversal", number_exp);
	prof.divideValues("Operations_Iterative_Traversal", number_exp);
	prof.createGroup("Total Operations", "Operations_PreOrder_Traversal", "Operations_InOrder_Traversal", "Operations_PostOrder_Traversal", "Operations_Iterative_Traversal");
}

void perf_find_threshold()
{
	int a[10001];
	int a1[10001];
	int size;
	int count = 0;
	int increment = 1;
	int start_size = 0;
	int end_size = 50;
	int number_exp = 10000;
	for (int experiment = 0; experiment < number_exp; experiment++)
	{
		count++;
		for (size = start_size; size <= end_size; size += increment)
		{
			FillRandomArray(a, size, 10, 50000, false, UNSORTED);
			CopyArray(a1, a, size);
			QuickSort_threshold(a, size, 0, size - 1);
			Insertion_Sort_threshold(a1, size, 0, size - 1);
		
		}
		cout << count << " ";
	}
	prof.divideValues("Comparisons_QuickSort_Threshold", number_exp);
	prof.divideValues("Assignments_QuickSort_Threshold", number_exp);
	prof.addSeries("Operations_QuickSort_Threshold", "Assignments_QuickSort_Threshold", "Comparisons_QuickSort_Threshold");

	prof.divideValues("Comparisons_InsertionSort_Threshold", number_exp);
	prof.divideValues("Assignments_InsertionSort_Threshold", number_exp);
	prof.addSeries("Operations_InsertionSort_Threshold", "Assignments_InsertionSort_Threshold", "Comparisons_InsertionSort_Threshold");

	prof.createGroup("Total Comparisons", "Comparisons_InsertionSort_Threshold", "Comparisons_QuickSort_Threshold");
	prof.createGroup("Total Assignments", "Assignments_InsertionSort_Threshold", "Assignments_QuickSort_Threshold");
	prof.createGroup("Total Operations", "Operations_InsertionSort_Threshold", "Operations_QuickSort_Threshold");
	
}
void perf_OP()
{
	int a[10001];
	int a1[10001];
	int increment = 100;
	int start_size = 100;
	int end_size = 10000;
	int number_exp = 5;
	for (int experiment = 0; experiment < number_exp; experiment++)
	{
		for (int size = start_size; size <= end_size; size += increment)
		{
			FillRandomArray(a, size, 10, 50000, false, UNSORTED);
			CopyArray(a1, a, size);
			QuickSort(a, size, 0, size - 1);
			Hybrid_QuickSort(a1, size, 0, size - 1,17);
			cout << size << " ";
		}
	}

	prof.divideValues("Comparisons_QuickSort", number_exp);
	prof.divideValues("Assignments_QuickSort", number_exp);
	prof.addSeries("Operations_QuickSort", "Assignments_QuickSort", "Comparisons_QuickSort");

	prof.divideValues("Comparisons_Hybrid_QuickSort", number_exp);
	prof.divideValues("Assignments_Hybrid_QuickSort", number_exp);
	prof.addSeries("Operations_Hybrid_QuickSort", "Assignments_Hybrid_QuickSort", "Comparisons_Hybrid_QuickSort");

	prof.createGroup("Total Assignments", "Assignments_QuickSort", "Assignments_Hybrid_QuickSort");
	prof.createGroup("Total Comparisons", "Comparisons_QuickSort", "Comparisons_Hybrid_QuickSort");
	prof.createGroup("Total Operations", "Operations_QuickSort", "Operations_Hybrid_QuickSort");

}
void perf_time()
{
	int a[10001];
	int a1[10001];
	int number_tests = 1000;
	int start_size = 100;
	int end_size = 10000;
	int increment = 100;
	for (int size = start_size; size <= end_size; size += increment)
	{
		prof.startTimer("Time_QuickSort", size);
		for (int tests = 0; tests < number_tests; tests++)
		{
			FillRandomArray(a, size, 10, 50000, false, UNSORTED);
			QuickSort_NO_OP(a, size, 0, size - 1);
		}
		prof.stopTimer("Time_QuickSort", size);
		prof.startTimer("Time_Hybrid_QuickSort", size);
		for (int tests = 0; tests < number_tests; tests++)
		{
			FillRandomArray(a, size, 10, 50000, false, UNSORTED);
			Hybrid_QuickSort_NO_OP(a, size, 0, size - 1, 17);
		}	
		prof.stopTimer("Time_Hybrid_QuickSort", size);

		cout << size << " ";
	}
	prof.createGroup("Total Time", "Time_QuickSort", "Time_Hybrid_QuickSort");
}

void perf_time_threshold() {
	int a[10001];
	int a1[10001];
	int nr_tests = 1000;
	int start_size = 5;
	int end_size = 50;
	int increment = 1;
	
	for (int i = start_size; i <= end_size; i+=increment) 
	{
		prof.startTimer("Hybrid_QuickSort", i);
		for (int test = 0; test < nr_tests; test++) {
			FillRandomArray(a, 10000, 10, 50000);
			Hybrid_QuickSort_NO_OP(a, 10000, 0, 10000-1,i);	
		}
		prof.stopTimer("Hybrid_QuickSort", i);

		prof.startTimer("QuickSort", i);
		for (int test = 0; test < nr_tests; test++) {
			FillRandomArray(a, 10000, 10, 50000);
			QuickSort_NO_OP(a, 10000, 0, 10000 - 1);
		}
		prof.stopTimer("QuickSort", i);
		cout << i << " ";
	}

	prof.createGroup("Time Comparison", "Hybrid_QuickSort", "QuickSort");
}

void perf_OP_threshold() {
	int array[10001];
	int copy[10001];
	int nr_exp = 100;

	for (int exp = 1; exp <= nr_exp; exp++) {
		printf("%d ", exp);
		for (int i = 5; i < 50; i++) {
			printf("%d ", exp);
			FillRandomArray(array, 10000, 10, 50000, 0, UNSORTED);
			Operation hybridComp = prof.createOperation("hybridQS_comp", i);
			Operation hybridAssign = prof.createOperation("hybridQS_assign", i);
			Operation comp = prof.createOperation("QS_comp", i);
			Operation assign = prof.createOperation("QS_assign", i);

			CopyArray(copy, array, 10000);
			Hybrid_QuickSort_parameters(copy, 10000, 0, 10000 - 1,i, hybridComp, hybridAssign);
			QuickSort_parameters(array, 10000, 0, 10000 - 1,comp, assign);
		}
	}

	prof.divideValues("hybridQS_comp", nr_exp);
	prof.divideValues("hybridQS_assign", nr_exp);
	prof.divideValues("QS_comp", nr_exp);
	prof.divideValues("QS_assign", nr_exp);

	prof.addSeries("Hybrid_QS", "hybridQS_comp", "hybridQS_assign");
	prof.addSeries("QS", "QS_comp", "QS_assign");

	prof.createGroup("Assignments", "hybridQS_assign", "QS_assign");
	prof.createGroup("Comparisons", "hybridQS_comp", "QS_comp");
	prof.createGroup("Total_Operation", "Hybrid_QS", "QS");

	prof.showReport();
}






void perf_all()
{
	perf_traversals();
	prof.reset("Find_Threshold_Insertion-Quick");
	perf_find_threshold();
	prof.reset("Find_Threshold_Time");
	perf_time_threshold();
	prof.reset("Find_Threshold_OP");
	perf_OP_threshold();
	prof.reset("QuickSort_OP");
	perf_OP();
	prof.reset("QuickSort_Time");
	perf_time();
	prof.showReport();
}
int main()
{
	demo_Traversals();
	demo_Insertion_Sort();
	demo_QuickSort();
	demo_Hybrid_QuickSort();
	//perf_all();

	return 0;
}

