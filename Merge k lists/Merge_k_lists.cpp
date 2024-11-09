/*
* First, i will talk about the algorithms and then i will interpret the charts.
* 
*  Heapify is a recursive function that interchanghes a parent with the samllest child if that child is also smaller that the parent.
* With other words, it "sinks" an element to its right place in a heap(where he is smaller than the childs).
* 
* For Build MinHeap, we start with the first non leaf elements and splits the input array and makes sub-heaps (by calling heapify on every element) going up.
* 
* The function Generate_Lists generates k random sorted lists of total lenght n, with random lenghts for each lists(but their sum is n).
* I used an random sorted array with first element 0, last element n and size k+1.
* The size of each list is the difference between two consecutive elements(since the array is random the sizes will be random).
* Because first element in 0 and last n, the sum of all random list sizes will be n as required.
* Then, we fill the lists with sorted random elements till they get to the random size.
* 
* The function merge lists merge that k random sorted list into one big sorted list.
* We call build min heap first to have the list with the smallest first element at the root.
* We put it into the final lists, we remove it(then the second element from that list will be at root).
* We call heapify to have again the list with the second smallest element at root. We put it into the final list,remove it and so on.
* If a list at the root at some point is empty, we swap it with the last list, remove it from the heap and decrease the size of the heap.
* Then we call heapify and repeat the operations above.
* 
* Now, lets talk about the charts.
* 
* The complexity of merging k lists with n total elements is n*log(k) because insertion and deletion from a list takes log(k) and we do it for all the n elements.
* We can see in the first chart, that if k is bigger, the algorithm is worse because log(k) wil be greater.
* From the second chart, where n is a constant(10.000) we can see the logarithmic function log(k).





*/


#include <iostream>
#include <iterator>
#include "Profiler.h"
#include <list>
using namespace std;
Profiler prof("Average_case");

void showlist(list<int> g)
{
	list<int>::iterator it;
	for (it = g.begin(); it != g.end(); ++it)
		cout << *it << " ";
}

void Heapify(list <int> *a, int size, int i,Operation &op)
{
	int l = 2 * i + 1;
	int r = 2 * i + 2;
	int smallest;
	if (l < size && a[l].front() < a[i].front())
		smallest = l;
	else
		smallest = i;
	if (r < size && a[r].front() < a[smallest].front())
		smallest = r;
	if (l < size)
		op.count();
	if (r < size)
		op.count();
	if (smallest != i)
	{
		op.count(3);
		swap(a[i], a[smallest]);
		Heapify(a, size, smallest,op);
	}
}
void build_minheap(list <int> *a, int k, Operation& op)
{
	for (int i = k / 2; i >= 0; i--)
	{
		Heapify(a, k, i,op);
	}
}

void mergelist (list <int> *a,int size, list <int> final, int k,Operation &op)
{
	build_minheap(a, k,op);
	while (k > 0)
	{
		op.count(3);
		int el = a[0].front();
		final.push_back(el);
		a[0].pop_front();
		if (a[0].empty())
		{
			op.count(3);
			swap(a[0], a[k - 1]);
			k--;
		}
			Heapify(a, k, 0,op);
		
	}
	showlist(final);
	
}
list <int>*Generate_Lists(int size,int k)
{
	list <int> *a = new list <int>[k];
	int* list_size = (int*)malloc(k * sizeof(int));
	int* arr = (int*)malloc(size * sizeof(int));
	FillRandomArray(arr, k, 1, size-1, true, ASCENDING);
	arr[0] = 0;
	arr[k] = size;
	for (int i = 0; i < k; i++)
	{
		list_size[i] = arr[i + 1] - arr[i];
	}
	for (int i = 0; i < k; i++)
	{
		FillRandomArray(arr, list_size[i], 10, 50000, false, ASCENDING);
		for(int j=0;j<list_size[i];j++)
		{
			a[i].push_back(arr[j]);
		}

	}
	return a;
}
void perf_size()
{
	int increment = 100;
	int start_size = 101;
	int end_size = 10001;
	int number_exp = 5;
	int k1 = 5, k2 = 10, k3 = 100;
	list <int>* a, final;
	int size;

	for (int experiment = 0; experiment < number_exp; experiment++)
	{
		for (size = start_size; size <= end_size; size += increment)
		{
			Operation op1 = prof.createOperation("Operations_k1", size);
			Operation op2 = prof.createOperation("Operations_k2", size);
			Operation op3 = prof.createOperation("Operations_k3", size);
			a=Generate_Lists(size, k1);
			mergelist(a, size, final, k1,op1);
			a=Generate_Lists(size, k2);
			mergelist(a, size, final, k2, op2);
			a=Generate_Lists(size, k3);
			mergelist(a, size, final, k3, op3);
		}
		cout<<size << " ";
	}
	prof.divideValues("Operations_k1", number_exp);
	prof.divideValues("Operations_k2", number_exp);
	prof.divideValues("Operations_k3", number_exp);
	prof.createGroup("Total Operations","Operations_k1", "Operations_k2", "Operations_k3");
	prof.showReport();
}
void perf_k()
{
	int increment = 10;
	int start_k = 10;
	int end_k = 500;
	int number_exp = 5;
	list <int>* a, final;
	int k;
	int size = 10000;

	for (int experiment = 0; experiment < number_exp; experiment++)
	{
		for (k = start_k; k <= end_k; k += increment)
		{
			Operation op = prof.createOperation("Operations", k);
			a = Generate_Lists(size, k);
			mergelist(a, size, final, k, op);
		}
		cout << k << " ";
	}
	prof.divideValues("Operations", number_exp);
	prof.showReport();
}


void demo_Generate_Lists()
{
	int size = 50, k = 10;
	list <int>* a = Generate_Lists(size, k);
	cout << endl;
	cout << "               RANDOM GENERATED LISTS          ";
	cout << endl << endl;
	for (int i = 0; i < k; i++)
	{
		cout << "List " << i << " with length " << a[i].size() << ": ";
		showlist(a[i]);
		cout << endl;
	}

	
}
void demo_build_minheap()
{
	
	int size = 50, k = 10;
	Operation op = prof.createOperation("Operations_Bottom_Up", size);
	list <int>* a = Generate_Lists(size, k);
	cout << endl;
	cout << "              INITIAL RANDOM GENERATED LISTS(HEAP)          ";
	cout << endl << endl;
	for (int i = 0; i < k; i++)
	{
		cout << "List " << i << " with length " << a[i].size() << ": ";
		showlist(a[i]);
		cout << endl;
	}
	cout << endl << endl << endl;
	cout << "              HEAP LISTS          ";
	cout << endl << endl;
	build_minheap(a, k,op);
	for (int i = 0; i < k; i++)
	{
		cout << "List " << i << " with length " << a[i].size() << ": ";
		showlist(a[i]);
		cout << endl;
	}

}
void demo_merge_lists()
{
	int size = 50, k = 10;
	Operation op = prof.createOperation("Operations_Merge_Lists", size);
	list <int>* a = Generate_Lists(size, k), final;
	cout << endl;
	cout << "              INITIAL RANDOM GENERATED LISTS(MERGE)          ";
	cout << endl << endl;
	for (int i = 0; i < k; i++)
	{
		cout << "List " << i << " with length " << a[i].size() << ": ";
		showlist(a[i]);
		cout << endl;
	}
	cout << endl << endl << endl;
	cout << "              MERGED LIST         ";
	cout << endl << endl;
	mergelist(a, size,final, k,op);

}



int main()
{
	demo_Generate_Lists();
	demo_build_minheap();
	demo_merge_lists();
	/*perf_size();
	perf_k();*/
	return 0;


}



