/*

* Butas Rafael-Dorian
* Group 30424
 
* First, i want to talk about the implemented algorithms, and then i will interpret the charts.

* Sink or Heapify is a recursive function that interchanghes a parent with the biggest child if that child is also bigger that the parent.
* With other words, is "sinks" an element to its right place in a heap(where he is bigger than the childs).

* Swim is also a recursive function that interchanges a child with its parent if the child is bigger.
* With other words, it makes an element "swim" to its right place in a heap.

* Bottom up and Top Down are heap construction algorithms that use Sink and respectively Swim.
* For Bottom Up, as the name suggests we start with the first non leaf elements and splits the input array and makes sub-heaps(by calling heapify on every element) going up.
* For Top Down, we start with the second element(the root has no parent so we dont apply swim on it), and we call swim for every element.

* Heap Sort is a sorting algorithm that uses Bottom Up approach(just because its faster) to prepare the array.
* After that, we know that at the root we have the largest element in the array. We swap it with the last element(in the heap and the array) so we can remove it from the heap(not the array).
* We apply Heapify to restore the heap proprieties, now the second last element stands at the root. 
* We swap it at the end of the heap which is before the last element in the array and remove it again from the heap.
* In this process the first root which is the largest element goes to the end of the array, the second root which is the second largest goes before the largest element and so on.

* Interpreting the charts, we can clearly see that the Bottom Up approach is better in both cases(average and worst).
* On average case the difference regarding the operations is quite small but in the worst case it gets significantly bigger.
* Bottom Up algorithm has an average and worst case complexity of O(n) while Top Down algorithm has an average and worst case complexity of O(n*log(n)).
* While normally you think that both have O(n*log(n)), using the heap properties that the height is log(n) and the maximum number of nodes is n/2^(h+1)
* We can prove that the upper bound is O(2*n) which is O(n) by calculatin the sum of h/2^h(which is equal to 2).

*/

#include <iostream>
#include "Profiler.h"
using namespace std;
Profiler prof("Average_case");
enum CASES { Average,Worst };
void Sink(int* a,int size,int i,Operation &comp, Operation &assig)
{
	int l = 2 * i + 1;
	int r = 2 * i + 2;
	int largest;
	if (l<size && a[l]>a[i])
		largest = l;
	else
		largest = i;
	if (r<size && a[r]>a[largest])
		largest = r;
	if (l < size)
		comp.count();
	if (r < size)
		comp.count();
	if (largest != i)
	{
		assig.count(3);
		swap(a[i], a[largest]);
		Sink(a, size, largest,comp,assig);
	}


}
void Swim(int* a, int i, Operation &comp, Operation &assig)
{
	int p = (i-1) / 2;
	if (i > 0)
	{
		comp.count();
		if (a[p] < a[i])
		{
			assig.count(3);
			swap(a[p], a[i]);
			Swim(a, p, comp, assig);
		}
	}
}
void Bottom_Up(int* a, int size)
{
	Operation comp = prof.createOperation("Comparisons_Bottom_Up", size);
	Operation assig = prof.createOperation("Assignments_Bottom_Up", size);
	for (int i = size / 2 - 1; i >= 0; i--)
		Sink(a, size, i, comp, assig);
}
void Top_Down(int* a, int size)
{
	Operation comp = prof.createOperation("Comparisons_Top_Down", size);
	Operation assig = prof.createOperation("Assignments_Top_Down", size);
	for (int i = 1; i < size; i++)
		Swim(a, i, comp, assig);
	
}
void HeapSort(int* a, int size)
{
	Operation comp = prof.createOperation("Comparisons_HeapSort", size);
	Operation assig = prof.createOperation("Assignments_HeapSort", size);
	Bottom_Up(a, size);
	for (int i = size-1; i >=1; i--)
	{
		assig.count(3);
		swap(a[0], a[i]);
		Sink(a, i, 0,comp,assig);
	}

}
void demo_Bottom_up()
{
	int a[] = { 4,1,3,2,16,9,10,14,8,7};
	int size = 10;
	Bottom_Up(a, size);
	cout << "This is the max heap using Bottom Up Construction: ";
	for (int i = 0; i < size; i++)
		cout << a[i] << " ";
	cout << "\n";
}
void demo_Top_Down()
{
	int a[] = { 4,1,3,2,16,9,10,14,8,7 };
	int size = 10;
	Top_Down(a, size);
	cout << "This is the max heap using Top Down Construction: ";
	for (int i = 0; i < size; i++)
		cout << a[i] << " ";
	cout << "\n";
}
void demo_HeapSort()
{
	int a[] = { 4,1,3,2,16,9,10,14,8,7};
	int size = 10;
	HeapSort(a, size);
	cout << "This is the sorted array using Heap Sort: ";
	for (int i = 0; i < size; i++)
		cout << a[i] << " ";
	cout << "\n";
}

void perf(CASES cases)
{
	int a[10001];
	int a1[10001];
	int increment = 100;
	int start_size = 100;
	int end_size = 10000;
	int number_exp = 1;

	if (cases == Average)
	{
		number_exp = 5;
	}
	for (int experiment = 0; experiment < number_exp; experiment++)
	{
		for (int size = start_size; size <= end_size; size += increment)
		{
			if (cases == Average)
			{
				FillRandomArray(a, size, 10, 50000, false, UNSORTED);
				CopyArray(a1, a, size);

			}


			if (cases == Worst)
			{
				FillRandomArray(a, size, 10, 50000, false, ASCENDING);
				FillRandomArray(a1, size, 10, 50000, false, ASCENDING);

			}
			Bottom_Up(a, size);
			Top_Down(a1, size);

		}
	}
	prof.divideValues("Comparisons_Bottom_Up", number_exp);
	prof.divideValues("Assignments_Bottom_Up", number_exp);
	prof.addSeries("Operations_Bottom_Up", "Assignments_Bottom_Up", "Comparisons_Bottom_Up");

	prof.divideValues("Comparisons_Top_Down", number_exp);
	prof.divideValues("Assignments_Top_Down", number_exp);
	prof.addSeries("Operations_Top_Down", "Assignments_Top_Down", "Comparisons_Top_Down");

	prof.createGroup("Total Assignments", "Assignments_Bottom_Up", "Assignments_Top_Down");
	prof.createGroup("Total Comparisons", "Comparisons_Bottom_Up", "Comparisons_Top_Down");
	prof.createGroup("Total Operations", "Operations_Bottom_Up", "Operations_Top_Down");
}


	void perf_all()
	{
		perf(Average);
		prof.reset("Worst_case");
		perf(Worst);
		prof.showReport();
	}




int main()
{
	//demo_Bottom_up();
	//demo_Top_Down();
	//demo_HeapSort();
	perf_all();
	return 0;
}

