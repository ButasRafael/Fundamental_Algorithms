/*
* First, I will talk about the algorithms in this program and then about their efficiency interpreting the charts.
* 
* Sink or Heapify is a recursive function that interchanghes a parent with the biggest child if that child is also bigger that the parent.
* With other words, it "sinks" an element to its right place in a heap(where he is bigger than the childs).

* Heap Sort is a sorting algorithm that uses Bottom Up approach(just because its faster) to prepare the array.
* After that, we know that at the root we have the largest element in the array. We swap it with the last element(in the heap and the array) so we can remove it from the heap(not the array).
* We apply Heapify to restore the heap proprieties, now the second last element stands at the root. 
* We swap it at the end of the heap which is before the last element in the array and remove it again from the heap.
* In this process the first root which is the largest element goes to the end of the array, the second root which is the second largest goes before the largest element and so on.
* Heap sort is not stable because we can have to equal elements, and in the heap the first one in the array is also the first one to become root and will be placed after the second one.

* Quick Sort is a sorting algorithm that uses a pivot to partition the element.The pivot, at the beginning of the partition is the last element in the array.
* The partitioning function brings the smaller than pivot element to the left and the larger to the right, putting the pivot in its final place in the array.
* Then, the quick sort recur in the left part and in the right part choosing different pivots, using a divide and conquer method to put all the elements in their place.
* Quick sort is not stable because during partitoning it swaps non adjacent elements without considering if we have other elements that are equal to the ones we are swapping.

* Insertion sort is a sorting algorithm in which the array is splitted into two parts(the left sorted part, and the right unsorted or unknown part).
* Insertion sort compares the current element(the first element in the unknown part) with the elements from the sorted part.
* In this comparing process, the elements that are greater then our current element are right shifted to make space for the current element.
* Insertion sort is a stable algorithm(if an element from unknown part is equal to an element from the sorted part it will be placed after him, the order doesnt change).

* Random Quick Sort does everything like quick sort but the pivot is chosen randomly in each recursive call to have a smaller chance of choosing a bad pivot and ending up in the worst case.

* Quick select is a recursive divide and conquer algorithm in which we find the k smallest element in an unsorted array.
* It does the partitioning like quick sort but instead of going recursively in the left and the right it compares the pivot index with k to know in which direction to recur(only one).
* If k is smaller you go search the k smallest element in the left, if k is larger you go search the (k-all the elements in the left including the pivot) smallest element in the right.
* Also randomized_select chooses the pivot randomly to have a smaller chance of ending up in a worst case(which is O(n^2)).
* The average complexity is O(n).

* Median quick select is working exactly like quick select but it uses the median of the medians approach to find the best pivot avoiding worst cases.
* The median of the medians is an algorithm which splits the array in arrays of 5, finds the median of all of them putting them in a new array and then, recursivly, does it again for the median array, until you have only one median left.
* That median is guaranteed to be a good pivot(sometimes the best).You can use this median quick select to create a quick sort that has no worst case n^2 but has worst case O(n*log(n)),
  but its not efficent in practice because best and average cases are less efficent.
 
* Now, lets interpret the charts.
 
* We can see that on the average case both quick sort and heap sort have n*log(n) complexities but quick sort is slightly better.Also the random quick sort does a few more assignments to choose a random element and put it to the end
* But you have smaller chances ending up in worst case so maybe it's worth it.
 
* At heap sort, best case occurs when all the elements are equal(no sink need to be done) and it's complexity in O(n), and at quick sort, best case occurs when in each recursive call the pivot is the mean-value element in the sub-array and its compleexity is n*log(n).
* So heap sort is way better then quick sort in the best case.
 
* At heap sort, worst case occurs when all the elements are distinct(you sink deeper), and it's compelxity is n*log(n), and at quick sort, worst case occurs when the pivot is the smallest or the largest value at each recursive call, and its complexity is n^2.
* So heap sort is waay better on the worst case. Using median quick sort you get worst case complexity n*log(n), but still not better than heap sort.
 
* Regarding insertion sort, in all cases the iterative version and the recursive version have the same number of operations.(average and worst cases have n^2 and best case is linear).
* The execution time is almost the same, the recursive version is slightly slower then the iterative version.
*/

#include <iostream>
#include <cstdlib>
#include <time.h>
#include "Profiler.h"
using namespace std;
Profiler prof("Average_case");
enum CASES { Average, Worst, Best };

void Sink(int* a, int size, int i, Operation& comp, Operation& assig)
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
		Sink(a, size, largest, comp, assig);
	}
}

void HeapSort(int* a, int size)
{
	Operation comp = prof.createOperation("Comparisons_HeapSort", size);
	Operation assig = prof.createOperation("Assignments_HeapSort", size);
	for (int i = size / 2 - 1; i >= 0; i--)
		Sink(a, size, i, comp, assig);
	for (int i = size - 1; i >= 1; i--)
	{
		assig.count(3);
		swap(a[0], a[i]);
		Sink(a, i, 0, comp, assig);
	}
}

void Insertion_sort_Iterative(int* a, int size)
{
	Operation comp = prof.createOperation("Comparisons_InsertionSort_Iterative", size);
	Operation assig = prof.createOperation("Assignments_InsertionSort_Iterative", size);
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

void Insertion_sort_Iterative_NO_OP(int* a, int size)
{
	int buff;
	int j;
	for (int i = 1; i < size; i++)
	{
		buff = a[i];
		j = i - 1;
		while (j >= 0 && a[j] > buff)
		{
			a[j + 1] = a[j];
			j--;

		}
		a[j + 1] = buff;
	}
}

void Insertion_sort_Recursive(int* a, int size,Operation& comp, Operation& assig)
{
	if (size <= 1)
		return;
	Insertion_sort_Recursive(a, size - 1, comp, assig);
	assig.count();
	int buff = a[size - 1];
	int j = size - 2;
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

void Insertion_sort_Recursive_NO_OP(int* a, int size)
{
	if (size <= 1)
		return;
	Insertion_sort_Recursive_NO_OP(a, size - 1);
	int buff = a[size - 1];
	int j = size - 2;
	while (j >= 0 && a[j] > buff)
	{
		a[j + 1] = a[j];
		j--;
	}
	a[j + 1] = buff;
}

void CountOP_Insertion_sort_Recursive(int *a,int size)
{
	Operation comp = prof.createOperation("Comparisons_InsertionSort_Recursive", size);
	Operation assig = prof.createOperation("Assignments_InsertionSort_Recursive", size);
	Insertion_sort_Recursive(a, size, comp, assig);
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

int PivotIndex_BestCase(int* a, int left, int right, Operation& comp, Operation& assig)
{
	swap(a[(right + left - 1) / 2], a[right]);
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

void QuickSortBestCase(int* a, int size, int left, int right)
{
	Operation comp = prof.createOperation("Comparisons_QuickSortBestCase", size);
	Operation assig = prof.createOperation("Assignments_QuickSortBestCase", size);
	if (left < right)
	{
		int pivot = PivotIndex_BestCase(a, left, right, comp, assig);
		QuickSortBestCase(a, size, left, pivot - 1);
		QuickSortBestCase(a, size, pivot + 1, right);
	}
}

int PickRandomPivot(int* a, int left, int right, Operation& comp, Operation& assig)
{
	srand(time(NULL));
	int random = left + rand() % (right - left + 1);
	assig.count(3);
	swap(a[random], a[right]);
	return PivotIndex(a, left, right, comp, assig);
}

void QuickSort_Random_Pivot(int* a, int size, int left, int right)
{
	Operation comp = prof.createOperation("Comparisons_QuickSort_Random", size);
	Operation assig = prof.createOperation("Assignments_QuickSort_Random", size);
	if (left < right)
	{
		int pivot = PickRandomPivot(a, left, right, comp, assig);
		QuickSort_Random_Pivot(a, size, left, pivot - 1);
		QuickSort_Random_Pivot(a, size, pivot + 1, right);
	}
}

int Median(int* a, int size)
{
	sort(a, a + size);
	return a[size / 2];
}

int Partition_FindPivot(int* a, int left, int right,int k, Operation& comp, Operation& assig)
{
	int i;
	for (i = left; i < right; i++)
	{
		comp.count();
		if (a[i] == k)
			break;
	}
	assig.count(3);
	swap(a[i], a[right]);
	 i = (left - 1);
	for (int j = left; j <= right-1; j++)
	{
		comp.count();
		if (a[j] <= k)
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

int Median_Quick_Select(int* a,int size,int left, int right, int k)
{
	Operation comp = prof.createOperation("Comparisons_Median_QuickSelect", size);
	Operation assig = prof.createOperation("Assignments_Median_QuickSelect", size);

	if (left == right)
		return a[left];
	int n = right - left + 1, i;
	int* median = new int[(n + 4) / 5];

	for (i = 0; i < n / 5; i++)
	{
		median[i] = Median(a + left + i * 5, 5);
	}
	if (i * 5 < n)
	{
		median[i] = Median(a + left + i * 5, n % 5);
		i++;	
	}
	int final_median;
	if (i == 1)
		final_median = median[i - 1];
	else
		final_median = Median_Quick_Select(median, size, 0, i - 1, i / 2);
	int pivot = Partition_FindPivot(a, left, right, final_median, comp, assig);
	delete[] median;
	if (k - 1 == pivot - left)
		return a[pivot];
	if (k - 1 < pivot - left)
		return Median_Quick_Select(a, size,left, pivot - 1, k);
	else
		return Median_Quick_Select(a, size,pivot + 1, right, k - pivot + left - 1);
}

void Median_QuickSort(int* a, int size, int left, int right)
{
	Operation comp = prof.createOperation("Comparisons_Median_QuickSort", size);
	Operation assig = prof.createOperation("Assignments_Median_QuickSort", size);
	if (left < right)
	{
		int n = right - left + 1;
		int median = Median_Quick_Select(a,size,left, right, n / 2);
		int pivot = Partition_FindPivot(a, left, right, median, comp, assig);
		Median_QuickSort(a, size, left, pivot - 1);
		Median_QuickSort(a, size, pivot + 1, right);
	}
}

int QuickSelect_Random_Pivot(int* a, int size, int left, int right, int k)
{
	Operation comp = prof.createOperation("Comparisons_Randomized_Select", size);
	Operation assig = prof.createOperation("Assignments_Randomized_Select", size);
	if (left == right)
		return a[left];
	int pivot = PickRandomPivot(a, left, right, comp, assig);
	if (k - 1 == pivot - left)
		return a[pivot];
	if (k - 1 < pivot - left)
		return QuickSelect_Random_Pivot(a, size, left, pivot - 1, k);
	else
		return QuickSelect_Random_Pivot(a, size, pivot + 1, right, k - pivot + left - 1);
}


void Original_Array()
{
	int a[] = { 25,24,33,39,3,18,19,31,23,49,45,16,1,29,40,22,15,20,24,4,13,34 };
	int size = 22;
	cout << "This is the original array: ";
	for (int i = 0; i < size; i++)
		cout << a[i] << " ";
	cout << "\n";
}
void demo_HeapSort()
{
	int a[] = { 25,24,33,39,3,18,19,31,23,49,45,16,1,29,40,22,15,20,24,4,13,34 };
	int size = 22;
	HeapSort(a, size);
	cout << "This is the sorted array using Heap Sort: ";
	for (int i = 0; i < size; i++)
		cout << a[i] << " ";
	cout << "\n";
}

void demo_QuickSort()
{
	int a[] = { 25,24,33,39,3,18,19,31,23,49,45,16,1,29,40,22,15,20,24,4,13,34 };
	int size = 22;
	QuickSort(a, size,0,size-1);
	cout << "This is the sorted array using Quick Sort: ";
	for (int i = 0; i < size; i++)
		cout << a[i] << " ";
	cout << "\n";
}

void demo_Insertion_sort_Iterative()
{
	int a[] = { 25,24,33,39,3,18,19,31,23,49,45,16,1,29,40,22,15,20,24,4,13,34 };
	int size = 22;
	Insertion_sort_Iterative(a, size);
	cout << "This is the sorted array using Iterative Insertion Sort: ";
	for (int i = 0; i < size; i++)
		cout << a[i] << " ";
	cout << "\n";
}

void demo_Insertion_sort_Recursive()
{
	int a[] = { 25,24,33,39,3,18,19,31,23,49,45,16,1,29,40,22,15,20,24,4,13,34 };
	int size = 22;
	CountOP_Insertion_sort_Recursive(a, size);
	cout << "This is the sorted array using Recursive Insertion Sort: ";
	for (int i = 0; i < size; i++)
		cout << a[i] << " ";
	cout << "\n";
}

void demo_QuickSort_Random_Pivot()
{
	int a[] = { 25,24,33,39,3,18,19,31,23,49,45,16,1,29,40,22,15,20,24,4,13,34 };
	int size = 22;
	QuickSort_Random_Pivot(a, size, 0, size - 1);
	cout << "This is the sorted array using Quick Sort with Random Pivot: ";
	for (int i = 0; i < size; i++)
		cout << a[i] << " ";
	cout << "\n";
}

void demo_Median_QuickSort()
{
	int a[] = { 25,24,33,39,3,18,19,31,23,49,45,16,1,29,40,22,15,20,24,4,13,34 };
	int size = 22;
	Median_QuickSort(a, size, 0, size - 1);
	cout << "This is the sorted array using Median Quick Sort: ";
	for (int i = 0; i < size; i++)
		cout << a[i] << " ";
	cout << "\n";
}

void demo_QuickSelect_Random_Pivot()
{
	int a[] = { 25,24,33,39,3,18,19,31,23,49,45,16,1,29,40,22,15,20,24,4,13,34 };
	int size = 22;
	int k = 2;
	cout << "This is the ";
	cout << k;
	if (k > 3)
		cout << "th smallest element in the array using Quick Select With Random Pivot: ";
	if (k == 3)
		cout << "rd smallest element in the array using Quick Select With Random Pivot: ";
	if (k == 2)
		cout << "nd smallest element in the array using Quick Select With Random Pivot: ";
	if (k == 1)
		cout << "st smallest element in the array using Quick Select With Random Pivot: ";
	cout << QuickSelect_Random_Pivot(a, size, 0, size - 1, k);
	cout << "\n";
}

void demo_Median_Quick_Select()
{
	int a[] = { 25,24,33,39,3,18,19,31,23,49,45,16,1,29,40,22,15,20,24,4,13,34 };
	int size = 22;
	int k = 6;
	cout << "This is the ";
	cout << k;
	if (k > 3)
		cout << "th smallest element in the array using Median Quick Select: ";
	if (k == 3)
		cout << "rd smallest element in the array using Median Quick Select: ";
	if (k == 2)
		cout << "nd smallest element in the array using Median Quick Select: ";
	if (k == 1)
		cout << "st smallest element in the array using Median Quick Select: ";
	cout << Median_Quick_Select(a, size, 0, size - 1, k);
	cout << "\n";
}

void perfQuickHeap(CASES cases)
{
	int a[10001];
	int a1[10001];
	int a2[10001];
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
				HeapSort(a, size);
				QuickSort(a1, size, 0, size - 1);
				QuickSort_Random_Pivot(a2, size, 0, size - 1);
			}

			if (cases == Worst)
			{
				FillRandomArray(a, size, 10, 50000, true, UNSORTED);
				FillRandomArray(a1, size, 10, 50000, false, DESCENDING);
				HeapSort(a, size);
				QuickSort(a1, size, 0, size - 1);
			}
			if (cases == Best)
			{

				FillRandomArray(a, size, 25000, 25001, false, UNSORTED);
				for (int i = 0; i < size; i++)
				{
					if (a[i] == 25001)
						a[i] = 25000;
				}
				FillRandomArray(a1, size, 10, 50000, false, ASCENDING);
				HeapSort(a, size);
				QuickSortBestCase(a1, size, 0, size - 1);

			}
			cout << size << " ";

		}
	}
	if (cases == Average)
	{
		prof.divideValues("Comparisons_HeapSort", number_exp);
		prof.divideValues("Assignments_HeapSort", number_exp);
		prof.addSeries("Operations_HeapSort", "Assignments_HeapSort", "Comparisons_HeapSort");

		prof.divideValues("Comparisons_QuickSort", number_exp);
		prof.divideValues("Assignments_QuickSort", number_exp);
		prof.addSeries("Operations_QuickSort", "Assignments_QuickSort", "Comparisons_QuickSort");

		prof.divideValues("Comparisons_QuickSort_Random", number_exp);
		prof.divideValues("Assignments_QuickSort_Random", number_exp);
		prof.addSeries("Operations_QuickSort_Random", "Assignments_QuickSort_Random", "Comparisons_QuickSort_Random");

		prof.createGroup("Total Assignments", "Assignments_HeapSort", "Assignments_QuickSort", "Assignments_QuickSort_Random");
		prof.createGroup("Total Comparisons", "Comparisons_HeapSort", "Comparisons_QuickSort", "Comparisons_QuickSort_Random");
		prof.createGroup("Total Operations", "Operations_HeapSort", "Operations_QuickSort", "Operations_QuickSort_Random");

	}

	if (cases == Worst)
	{
		prof.divideValues("Comparisons_HeapSort", number_exp);
		prof.divideValues("Assignments_HeapSort", number_exp);
		prof.addSeries("Operations_HeapSort", "Assignments_HeapSort", "Comparisons_HeapSort");

		prof.divideValues("Comparisons_QuickSort", number_exp);
		prof.divideValues("Assignments_QuickSort", number_exp);
		prof.addSeries("Operations_QuickSort", "Assignments_QuickSort", "Comparisons_QuickSort");

		prof.createGroup("Total Assignments", "Assignments_HeapSort", "Assignments_QuickSort");
		prof.createGroup("Total Comparisons", "Comparisons_HeapSort", "Comparisons_QuickSort");
		prof.createGroup("Total Operations", "Operations_HeapSort", "Operations_QuickSort");
	}

	if (cases == Best)
	{
		prof.divideValues("Comparisons_HeapSort", number_exp);
		prof.divideValues("Assignments_HeapSort", number_exp);
		prof.addSeries("Operations_HeapSort", "Assignments_HeapSort", "Comparisons_HeapSort");

		prof.divideValues("Comparisons_QuickSortBestCase", number_exp);
		prof.divideValues("Assignments_QuickSortBestCase", number_exp);
		prof.addSeries("Operations_QuickSortBestCase", "Assignments_QuickSortBestCase", "Comparisons_QuickSortBestCase");

		prof.createGroup("Total Assignments", "Assignments_HeapSort", "Assignments_QuickSortBestCase");
		prof.createGroup("Total Comparisons", "Comparisons_HeapSort", "Comparisons_QuickSortBestCase");
		prof.createGroup("Total Operations", "Operations_HeapSort", "Operations_QuickSortBestCase");
	}

}

void perfInsertion(CASES cases)
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
				FillRandomArray(a, size, 10, 50000, false, DESCENDING);
				FillRandomArray(a1, size, 10, 50000, false, DESCENDING);
			}
			if (cases == Best)
			{
				FillRandomArray(a, size, 10, 50000, false, ASCENDING);
				FillRandomArray(a1, size, 10, 50000, false, ASCENDING);

			}
			Insertion_sort_Iterative(a, size);
			CountOP_Insertion_sort_Recursive(a1, size);
			cout << size << " ";

		}
	}

	prof.divideValues("Comparisons_InsertionSort_Iterative", number_exp);
	prof.divideValues("Assignments_InsertionSort_Iterative", number_exp);
	prof.addSeries("Operations_InsertionSort_Iterative", "Assignments_InsertionSort_Iterative", "Comparisons_InsertionSort_Iterative");

	prof.divideValues("Comparisons_InsertionSort_Recursive", number_exp);
	prof.divideValues("Assignments_InsertionSort_Recursive", number_exp);
	prof.addSeries("Operations_InsertionSort_Recursive", "Assignments_InsertionSort_Recursive", "Comparisons_InsertionSort_Recursive");

	prof.createGroup("Total Assignments Insertion Sort Iterative/Recursive", "Assignments_InsertionSort_Iterative", "Assignments_InsertionSort_Recursive");
	prof.createGroup("Total Comparisons Insertion Sort Iterative/Recursive", "Comparisons_InsertionSort_Iterative", "Comparisons_InsertionSort_Recursive");
	prof.createGroup("Total Operations Insertion Sort Iterative/Recursive", "Operations_InsertionSort_Iterative", "Operations_InsertionSort_Recursive");

}

void perfTime(CASES cases)
{
	int a[10001];
	int a1[10001];
	int number_tests = 1000;
	int start_size = 100;
	int end_size = 10000;
	int increment = 100;

	for (int size = start_size; size <= end_size; size += increment)
	{

		for (int tests = 0; tests < number_tests; tests++)
		{
			if (cases == Average)
			{
				FillRandomArray(a, size, 10, 50000, false, UNSORTED);
				CopyArray(a1, a, size);
				prof.startTimer("Time_Iterative", size);
				Insertion_sort_Iterative_NO_OP(a, size);
				prof.stopTimer("Time_Iterative", size);
				CopyArray(a, a1, size);
				prof.startTimer("Time_Recursive", size);
				Insertion_sort_Recursive_NO_OP(a, size);
				prof.stopTimer("Time_Recursive", size);
			}
			if (cases == Worst)
			{
				FillRandomArray(a, size, 10, 50000, false, DESCENDING);
				CopyArray(a1, a, size);
				prof.startTimer("Time_Iterative", size);
				Insertion_sort_Iterative_NO_OP(a, size);
				prof.stopTimer("Time_Iterative", size);
				CopyArray(a, a1, size);
				prof.startTimer("Time_Recursive", size);
				Insertion_sort_Recursive_NO_OP(a, size);
				prof.stopTimer("Time_Recursive", size);
			}
			if (cases == Best)
			{
				FillRandomArray(a, size, 10, 50000, false, ASCENDING);
				CopyArray(a1, a, size);
				prof.startTimer("Time_Iterative", size);
				Insertion_sort_Iterative_NO_OP(a, size);
				prof.stopTimer("Time_Iterative", size);
				CopyArray(a, a1, size);
				prof.startTimer("Time_Recursive", size);
				Insertion_sort_Recursive_NO_OP(a, size);
				prof.stopTimer("Time_Recursive", size);
			}

		}
		cout << size << " ";
	}
	prof.createGroup("Total Time", "Time_Iterative", "Time_Recursive");
}

void perf_all_QuickHeap()
{
	perfQuickHeap(Average);
	prof.reset("Worst_case");
	perfQuickHeap(Worst);
	prof.reset("Best_case");
	perfQuickHeap(Best);
	prof.showReport();
}

void perf_all_Insertion()
{
	perfInsertion(Average);
	prof.reset("Worst_case");
	perfInsertion(Worst);
	prof.reset("Best_case");
	perfInsertion(Best);
	prof.showReport();
}
void perfTime_all()
{
	perfTime(Average);
	prof.reset("Worst_case");
	perfTime(Worst);
	prof.reset("Best_case");
	perfTime(Best);
	prof.showReport();
}

int main()
{
	Original_Array();
	demo_HeapSort();
	demo_Insertion_sort_Iterative();
	demo_Insertion_sort_Recursive();
	demo_QuickSort();
	demo_QuickSort_Random_Pivot();
	demo_Median_QuickSort();
	demo_QuickSelect_Random_Pivot();
	demo_Median_Quick_Select();
	//perf_all_QuickHeap();
	//perf_all_Insertion();
	//perfTime_all();
	
	return 0;
}
