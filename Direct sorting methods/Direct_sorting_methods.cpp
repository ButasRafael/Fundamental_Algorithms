/*
* First, i want to talk a bit about the sorting algorithms in this program and then i will talk about their efficency interpreting the charts.

* The first one is the insertion sort, in which the array is splitted into two parts(the left sorted part, and the right unsorted or unknown part).
* Insertion sort compares the current element(the first element in the unknown part) with the elements from the sorted part.
* In this comparing process, the elements that are greater then our current element are right shifted to make space for the current element.
* Insertion sort is a stable algorithm(if an element from unknown part is equal to an element from the sorted part it will be placed after him, the order doesnt change)

* The second one is selection sort, in which the array is also splitted into two parts(the left sorted part, and the right unsorted or unknown part).
* Selection sort finds the index of the smallest element in the unsorted part and swaps it with the first element in this part.
* At selection sort, you consider the first element from the unsorted part as the minimum, and if some smaller value is found, you swap them.
* Selection sort is not a stable algorithm(for example ins the array 7 7 1 the first seven wil be swapped with the 1, changing the order).

* The third one is the bubble sort, in which you swap the adjacent elements if they are in the wrong order.
* Bubble sort, after first iteration, moves the largest element at the end of the array, after second iteration, it moves the second largest element before the largest element, and so on.
* Bubble sort is a stable algorithm(if two numbers are equal, they will becom adjacent eventually and the algorithm will carry further the one from the right).

* The fourth and the last one, is the Binary Insertion sort, which works like the insertion sort, but with a small diference.
* The diference is the comparing process(the search for the current element's place is done binary)
* This means that once we found the index, we need to shift to the right all the elements starting with the last element of the array all the way to the element from the found index.
* Stability reamins in place.

* On the average case, using the same random array, and performing the experiment five times, we can see that the bubble sort is the worst algorithm with more than tripled operations.
* Insertion and Selection sort are quite similar in this case(only a small number of operations are separating these two and it fluctuates from one to another).
* The Binary Insertion sort is clearly the best on the average case(because of the reduced comparisons from O(n) to O(logn)),having a complexity of n*log(n) so logarithmic, when all the other have n^2 on average case(quadratic complexity).
 
* All the algorithms need an already sorted array for the best case(they make almost no assignments).
* So, on the best case we can see that altough selection sort does no assignments, it does as many comparisons as in the average case and it ends up with a quadratic complexitiy like on the average case (n^2) , which makes it the worts algorithm on the best case.
* Insertion and Bubble sort are the best on best case(linear complexity), they have the same number of comparions(n-1), but bubble sort is a bit better because it has no assignments(he makes no swaps) and the insetion sort still has 2(n-1) assignments.
* Also, the Binary Insetion sort is not that good on the best case because it does way many comparisons then the insertion sort(but it still has linear complexity)
 
* On the worst case, all algorithms except selection sort need a reversed sorted array, and the selection sort needs a sorted array but with the first element being also the greatest(because it will traverse the whole array step by step).
* So, on the worst case the number of assignments and comparisons for the insertion sort aproximately doubles(compared to average case), making it almost the worst algorithm, after bubble sort which has the assignments doubled and having a bit more comparisons then in the average case.
* Selection Sort is the best algorithm for the worst case, having the same amount of operations like in the average case(+20 maximum).
* The Binary Insertion Sort is still a great choice for the worst case, but it has slighty more operations then selection sort.
* 
* All algorithms have quadratic complexity on the worst case, but selection sort has linear assignments complexity and binary insertion sort has linear comparisons complexity.
* All algorithms have quadratic complexity on average case,except the binary insertion sort which has logarithmic complexity.
* All algorithms have linear complexity on the best case, except selection sort which has quadratic complexity.
* 
* 


*/



#include <iostream>
#include "Profiler.h"
using namespace std;
Profiler prof("Average_case");
enum CASES { Average, Best, Worst };


void Insertion_sort(int* a, int size)
{
	Operation comp = prof.createOperation("Comparisons_Insertion", size);
	Operation assig = prof.createOperation("Assignments_Insertion", size);
	int buff;
	int j;
	for (int i = 1; i < size; i++)
	{
		assig.count();
		buff = a[i];
		j = i - 1;
		while (j >= 0 && a[j] > buff)
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
void Selection_sort(int* a, int size)
{
	int min;
	Operation comp = prof.createOperation("Comparisons_Selection",size);
	Operation assig = prof.createOperation("Assignments_Selection", size);
	for (int i = 0; i < size - 1; i++)
	{
		min = i;
		for (int j = i + 1; j < size; j++)
		{
			comp.count();
			if (a[j] < a[min])
			{
				min = j;
				
			}
			

		}
		if (min != i)
		{
			assig.count(3);
			swap(a[i], a[min]);
		}
	}


}
void Bubble_sort(int* a, int size)
{
	bool sorted;
	Operation comp = prof.createOperation("Comparisons_Bubble", size);
	Operation assig = prof.createOperation("Assignments_Bubble", size);
	do
	{
		sorted = true;
		for (int i = 0; i < size - 1; i++)
		{
			comp.count();
			if (a[i] > a[i + 1])
			{
				assig.count(3);
				swap(a[i], a[i + 1]);
				sorted = false;
			}

		}
	} while (!sorted);
	
}
int BinarySearch(int* a, int value, int low, int high,Operation &comp)
{
	if (high <= low)
	{
		comp.count();
		if (a[low] < value)
			return low + 1;
		else
			return low;
	}
	int mid = (low + high) / 2;
	comp.count();
	if (value == a[mid])
		return mid + 1;
	comp.count();
	if (value > a[mid])
		return BinarySearch(a, value, mid + 1, high,comp);
	return BinarySearch(a, value, low, mid - 1,comp);

}
void Insertion_sort_binary(int* a, int size)
{
	Operation comp = prof.createOperation("Comparisons_Insertion_Binary", size);
	Operation assig = prof.createOperation("Assignments_Insertion_Binary", size);
	int i, j, buff, index;
	for (i = 1; i < size; i++)
	{
		assig.count();
		buff = a[i];
		j = i - 1;
		index = BinarySearch(a, buff, 0, j, comp);
		while (j >= index)
		{
			assig.count();
			a[j + 1] = a[j];
			j--;
		}
		assig.count();
		a[j + 1] = buff;

	}
}

void demo_insertion() 
{
	int a[] = {7,5,9,2,1,7,6,3,8};
	int size = 9;
	Insertion_sort(a, size);


	cout<< "This is the sorted array using insertion sort: ";
	for (int i = 0; i < size; i++)
		cout << a[i] << " ";
	cout << "\n";
}
void demo_selection()
{
	int a[] = { 7,5,9,2,1,7,6,3,8 };
	int size = 9;
	Selection_sort(a, size);


	cout << "This is the sorted array using selection sort: ";
	for (int i = 0; i < size; i++)
		cout << a[i] << " ";
	cout << "\n";
}
void demo_bubble()
{
	int a[] = { 7,5,9,2,1,7,6,3,8 };
	int size = 9;
	Bubble_sort(a, size);


	cout << "This is the sorted array uisng bubble sort: ";
	for (int i = 0; i < size; i++)
		cout << a[i] << " ";
	cout << "\n";
	
}
void demo_binary_insertion()
{
	int a[] = { 7,5,9,2,1,7,6,3,8 };
	int size = 9;
	Insertion_sort_binary(a, size);

	cout << "This is the sorted array using insertion binary sort: ";
	for (int i = 0; i < size; i++)
		cout << a[i] << " ";

}
void perf(CASES cases)
{
	int a[10001];
	int a1[10001];
	int a2[10001];
	int a3[10001];
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
				CopyArray(a2, a, size);
				CopyArray(a3, a, size);
			}

			if (cases == Best)
			{

				FillRandomArray(a, size, 10, 50000, false, ASCENDING);
				FillRandomArray(a1, size, 10, 50000, false, ASCENDING);
				FillRandomArray(a2, size, 10, 50000, false, ASCENDING);
				FillRandomArray(a3, size, 10, 50000, false, ASCENDING);

			}

			if (cases == Worst)
			{
				FillRandomArray(a, size, 10, 50000, false, DESCENDING);
				FillRandomArray(a1, size, 10, 50000, false, ASCENDING);
				a1[0] = 50001;
				FillRandomArray(a2, size, 10, 50000, false, DESCENDING);
				FillRandomArray(a3, size, 10, 50000, false, DESCENDING);
			
			}
			Insertion_sort(a, size);
			Selection_sort(a1, size);
			Bubble_sort(a2, size);
			Insertion_sort_binary(a3, size);
		}
	}
	prof.divideValues("Comparisons_Insertion", number_exp);
	prof.divideValues("Assignments_Insertion", number_exp);
	prof.addSeries("Operations_Insertion", "Assignments_Insertion", "Comparisons_Insertion");

	prof.divideValues("Comparisons_Selection", number_exp);
	prof.divideValues("Assignments_Selection", number_exp);
	prof.addSeries("Operations_Selection", "Assignments_Selection", "Comparisons_Selection");

	prof.divideValues("Comparisons_Bubble", number_exp);
	prof.divideValues("Assignments_Bubble", number_exp);
	prof.addSeries("Operations_Bubble", "Assignments_Bubble", "Comparisons_Bubble");

	prof.divideValues("Comparisons_Insertion_Binary", number_exp);
	prof.divideValues("Assignments_Insertion_Binary", number_exp);
	prof.addSeries("Operations_Insertion_Binary", "Assignments_Insertion_Binary", "Comparisons_Insertion_Binary");

	prof.createGroup("Total Assignments", "Assignments_Insertion", "Assignments_Selection", "Assignments_Bubble", "Assignments_Insertion_Binary");
	prof.createGroup("Total Comparisons", "Comparisons_Insertion", "Comparisons_Selection", "Comparisons_Bubble", "Comparisons_Insertion_Binary");
	prof.createGroup("Total Operations", "Operations_Insertion", "Operations_Selection", "Operations_Bubble", "Operations_Insertion_Binary");
}
void perf_all() {
	perf(Average);
	prof.reset("Worst_case");
	perf(Worst);
	prof.reset("Best_case");
	perf(Best);
	prof.showReport();
}


int main()
{
	demo_insertion();
	demo_selection();
	demo_bubble();
	demo_binary_insertion();
	//perf_all();
	return 0;
}



