/*
* So first, i will talk about the algorithms and then about their efficency
* I work with a hash table structure(each key contains a value, a name and a boolean factor that says if its still in the table or it was deleted)
* We cant just erase an element when we want to delete it because it will mess our search and insert functions(i explain why below)

* To create the hash table we need a hash function(i used a quadratic one with prime numbers to avoid as much as i can the collisions), total number of slots is also a prime number.
* We have 3 big operations on this hash table: insert,search and delete.
 
* The insert function checks if a key is empty or was deleted, and if so, it puts the wanted element in that spot.The spot calculation is done using the quadratic probing.
* If the key is not empty, we redo the probing with different indexes to go to another spot and check(because there can be collisons and our element can be further in the tabel).
 
* The search function is done using the same quadratic probing, but if we find a null key then we stop because our element shouldve been found by then on that path that leaded to the null key.

* The delete function uses aditional memory to store if a key was deleted or not.
* If we just erased it from the table, at the search operation for example we can found that null-erased element and we stop, but we actually need to continue because that was not null from the beginning.
* If we swaped it with the last and remove it, we escape from this issue, but we create another one: we mess up the element places that were done using the quadratic probing and damage the insert and search functions again.

* For the efficency part, i used fill factors to fill a specific amount of the table, and searched 1500 elements that were found in the table and 1500 that were not.
* Then i printed how many accessings in the hash table we need to find the 1500 found/not found elements for each fill factor.
* As expected, if the fill factor is bigger, the outputs will also be bigger because we have more elements to search and less elements where the function stops because its a null key.
* Also, the found elements require less accessings than the not found ones(obviously) because, for example, many of the found elements are found in O(1) (no collision happened)
* In the last part, i took a fill factor of 0.99 and deleted elements from the table until i got a fill factor of 0.8.
* Although i have know 0,8 non null elements, the search operations for found and not found is working the same as it did for the 0.99 fill factor.
* The reason for that is beacuse the deleted elements are considered filled in the search function(to keep searching).
* We should not stop at that elements because they don't tell us that the collisions ended and we can stop searching.

* To create 1500 found elements that will be search, i used uniform selecting: i splitted the elements into 1500 almost equal buckets and took a random element from each bucket.
* Example:if size=8005, first 505 buckets will have 6 elements, and the rest of 995 will have 5 elements.
* I did this because as i said before, elements from the beginning of the table had no collisions so the number of accessings to find them is one.
* On the other hand, elements from the end of the table had experienced a possible large number of collisions and its harder to find them.

* Finally, when i deleted the elements i also did that uniformly and randomly because that deleted elements will not be the ones we search(i did make sure of that).
* If this is true then if we delete from the beginning only we will not search that elements (that are easy to find), and will damage our results.
* So, if we want a good and correct average, we take the searched elements uniformly and randomly, we delete elements uniformly and randomly, and perform more than one test(i did 5).

  The expected number of accessings for the found elements is maximum (1/f) * ln(1/(1-f)) where f is the filling factor.
  The expected number of accessings for the not found elements is maximum 1/(1-f) where f is the filling factor.

*/


#include <iostream>
#include <cstdlib>
#include <ctime>
#include "Profiler.h"
#include <iomanip>
#define MAX 10007
using namespace std;
Profiler prof("Average_case");
enum CASES {Fill_0_8,Fill_0_85,Fill_0_9,Fill_0_95,Fill_0_99};

typedef struct Entry {
	int id;
	bool deleted;
	char name[30];
} entryT;

entryT* T[MAX] = { NULL };
entryT* e = { NULL };

int quadratic_hash(int id,int i, int N)
{
	int index = (id + (3 * i) + (5 * i * i)) % N;
	return index;
}

void insert(entryT** T, entryT* e,int N)
{
	int i = 0, index;
	do
	{
		index = quadratic_hash(e->id, i,N);
		if (T[index] == NULL||T[index]->deleted==true)
		{
			T[index] = (entryT*)malloc(sizeof(entryT));
			T[index]->id = e->id;
			T[index]->deleted = false;
			strcpy_s(T[index]->name, e->name);
			return;
		}
		i++;
	} while (i < N);

}

 entryT** Create_HashTable(entryT** T,entryT *e,int n,int *value,int N)
{
	 
	for (int i = 0; i < n; i++)
	{
		e=(entryT*)malloc(sizeof(entryT));
		e->id = value[i];
		e->deleted = false;
		strcpy_s(e->name, "Test");
		insert(T, e, N);
	}
	return T;
}

int search(entryT** T, int id,int N,int &count)
{
	int i = 0, index;
	count = 0;
	do
	{
		count++;
		index = quadratic_hash(id, i, N);
		if (T[index] == NULL)
			return -1;
		if (T[index]->id == id)
			return index;
		i++;
		
	} while (i < N);
	return -1;
}

void deleted(entryT** T, int id, int N,int &count)
{
		int index = search(T, id, N,count);
		if (index != -1)
		{
			T[index]->deleted = true;
		}
}

void demo() 
{
	int N = 11;
	int count = 0;
	entryT** demoT = new entryT * [N];
	 e = { NULL };
	int* value = new int[N];
	float alpha[6] = {0.8,0.85,0.9,0.95,0.99,1};
	int index, n = int(N * alpha[5]);
	FillRandomArray(value, N, 10, 500, true, UNSORTED);

	for (int i = 0; i < N; i++)
	{
		demoT[i] = { NULL };
	}

	cout << endl;
	cout << "     ARRAY VALUES     ";
	cout << endl << endl;

	for (int i = 0; i < N; i++)
		cout << value[i] << " ";
	cout << endl << endl;

	Create_HashTable(demoT, e,n, value, N);

	cout <<endl;
	cout << "     INITIAL HASH TABLE      ";
	cout << endl << endl;

	for (int i = 0; i < N; i++) 
	{
		cout << "Index " << i << ": ";
		if (demoT[i] != NULL)
			cout << demoT[i]->id << " " << demoT[i]->name << boolalpha << " " << demoT[i]->deleted;
		else
			cout << "-" << " " << "-" << " " << "-";
		cout << endl;
	}

	cout << endl << endl << endl;
	cout << "     SEARCH OPERATION RESULTS     ";
	cout << endl << endl;

	for (int i = 0; i < N; i++)
	{
		index = search(demoT, value[i], N,count);
		if (index != -1)
		{
			cout << "Element " << value[i] << " found at position " << index;
			cout << endl;
		}
		else
		{
			cout << "Element " << value[i] << " not found at in the hash table ";
			cout << endl;
		}
	}

	cout << endl << endl << endl;

	deleted(demoT, value[2], N,count);
	deleted(demoT, value[3], N,count);

	cout << "     HASH TABLE AFTER WE DELETED 2 ELEMENTS      ";
	cout << endl << endl;

	for (int i = 0; i < N; i++)
	{
		cout << "Index " << i << ": ";
		if (demoT[i] != NULL)
			cout << demoT[i]->id << " " << demoT[i]->name << boolalpha << " " << demoT[i]->deleted;
		else
			cout << "-" << " " << "-" << " " << "-";
		cout << endl;
	}

	cout << endl << endl << endl;

	e = (entryT*)malloc(sizeof(entryT));
	e->deleted = false;
	e->id = 13;
	strcpy_s(e->name, "Rafa");
	insert(demoT, e, N);

	cout << "     HASH TABLE AFTER WE INSERT ONE DISTINCTIVE ELEMENT     ";
	cout << endl << endl;

	for (int i = 0; i < N; i++)
	{
		cout << "Index " << i << ": ";
		if (demoT[i] != NULL)
			cout << demoT[i]->id << " " << demoT[i]->name << boolalpha << " " << demoT[i]->deleted;
		else
			cout << "-" << " " << "-" << " " << "-";
		cout << endl;
	}
}

void Create_Found_Uniform_Selection(int *F,int *a,int n, int buckets)
{
	int bucket_size = n / buckets;
	int remainder = n - bucket_size * (buckets - 1);
	int current, random, i = 0;
	
	
	while (remainder > bucket_size)
	{

		current = i * (bucket_size + 1);
		random = rand() % (bucket_size + 1) + current;
		remainder--;
		F[i] = a[random];
		i++;
	}

	int i2 = (bucket_size + 1) * i / bucket_size;

	while (i < buckets)
	{
		current = i2 * bucket_size;
		random = rand() % bucket_size + current;
		F[i] = a[random];
		i++;
		i2++;
	}


}

void Create_Not_Found(int* NF, int* a, int start, int end)
{
	int i2 = 0;
	for (int i = start; i < end; i++)
	{
		NF[i2] = a[i];
		i2++;
	}
}

void Create_New_Array(int* new_a, int* a, int size)
{
	int i2 = 0;
	for (int i = 0; i < size; i++)
	{
		if (a[i] != 0)
		{
			new_a[i2] = a[i];
			i2++;
		}
	}
}

void Uniform_Deletion(int *a,int n, int buckets)
{
	int bucket_size = n / buckets;
	int remainder = n - bucket_size * (buckets - 1);
	int current, random, i = 0, count = 0;

	while (remainder > bucket_size)
	{

		current = i * (bucket_size + 1);
		random = rand() % (bucket_size + 1) + current;;
		remainder--;
		deleted(T, a[random], MAX, count);
		a[random] = 0;
		i++;
	}

	int i2 = (bucket_size + 1) * i / bucket_size;

	while (i < buckets)
	{
		current = i2 * bucket_size;
		random = rand() % bucket_size + current;
		deleted(T, a[random], MAX, count);
		a[random] = 0;
		i++;
		i2++;
	}
}

void perf_alpha(CASES cases)
{
	int n = 0;
	float alpha = 0;
	if (cases == Fill_0_8)
	{
		alpha = 0.8;
		n = int(MAX * alpha);
	}
	if (cases == Fill_0_85)
	{
		alpha = 0.85;
		n = int(MAX * alpha);
	}
	if (cases == Fill_0_9)
	{
		alpha = 0.9;
		n = int(MAX * alpha);
	}
	if (cases == Fill_0_95)
	{
		alpha = 0.95;
		n = int(MAX * alpha);
	}
	if (cases == Fill_0_99)
	{
		alpha = 0.99;
		n = int(MAX * alpha);
	}


	int buckets = 1500;
	int array_size = n + buckets;

	int* a = new int[array_size + 1];
	FillRandomArray(a, array_size, 1, 1000000000, true, UNSORTED);

	int* F = new int[buckets + 1];
	int* NF = new int[buckets + 1];

	Create_Found_Uniform_Selection(F, a, n, buckets);
	Create_Not_Found(NF, a, n, array_size);
	
	float AvgF, AvgNF, TotF = 0, TotNF = 0;
	int MaxF = -1, MaxNF = -1;
	int number_exp = 5, count = 0;

	e = { NULL };
	for (int i = 0; i < MAX; i++)
	{
		T[i] = { NULL };
	}
	Create_HashTable(T, e, n, a, MAX);

	for (int experiment = 0; experiment < number_exp; experiment++)
	{
		for (int i = 0; i < buckets; i++)
		{
			search(T, F[i], MAX,count);
			TotF = TotF + count;
			if (count > MaxF)
				MaxF = count;
			search(T, NF[i], MAX,count);
			TotNF = TotNF + count;
			if (count > MaxNF)
			{
				MaxNF = count;
			}
		}
	}

	AvgF = TotF / (buckets * number_exp);
	AvgNF = TotNF / (buckets * number_exp);

	cout << setw(20) << alpha;
	cout << setw(20) << AvgF;
	cout << setw(20) << MaxF;
	cout << setw(20) << AvgNF;
	cout << setw(20) << MaxNF;
	cout << endl;
	
	delete[] NF;
	delete[] F;
	delete[] a;
}

void perf_delete()
{
	float alpha = 0.99;
	float wanted_alpha = 0.8;
	int n = int(MAX * alpha);
	int buckets = 1500;
	int delete_buckets = int((alpha - wanted_alpha) * MAX);
	int array_size = n + buckets;
	int* a = new int[array_size + 1];
	FillRandomArray(a, array_size, 1, 1000000000, true, UNSORTED);

	e = { NULL };
	for (int i = 0; i < MAX; i++)
	{
		T[i] = { NULL };
	}

	Create_HashTable(T, e, n, a, MAX);

	Uniform_Deletion(a, n, delete_buckets);	

	int new_n = n - delete_buckets;
	int new_array_size = new_n + buckets;
	int* new_a = new int[new_array_size + 1];
	Create_New_Array(new_a, a, array_size);

	int* F = new int[buckets + 1];
	int* NF = new int[buckets + 1];

	Create_Found_Uniform_Selection(F, new_a, new_n, buckets);
	Create_Not_Found(NF, new_a, new_n, new_array_size);
	
	float AvgF, AvgNF, TotF = 0, TotNF = 0;
	int MaxF = -1, MaxNF = -1;
	int number_exp = 5, count = 0;

	for (int experiment = 0; experiment < number_exp; experiment++)
	{
		for (int i = 0; i < buckets; i++)
		{
			search(T, F[i], MAX, count);
			TotF = TotF + count;
			if (count > MaxF)
				MaxF = count;
			search(T, NF[i], MAX, count);
			TotNF = TotNF + count;
			if (count > MaxNF)
			{
				MaxNF = count;	
			}
			
		}
	}

	AvgF = TotF / (buckets * number_exp);
	AvgNF = TotNF / (buckets * number_exp);

	cout << setw(20) << "DELETED";
	cout << setw(20) << AvgF;
	cout << setw(20) << MaxF;
	cout << setw(20) << AvgNF;
	cout << setw(20) << MaxNF;
	cout << endl;





}
void perf_all()
{
	
	perf_alpha(Fill_0_8);
	perf_alpha(Fill_0_85);
	perf_alpha(Fill_0_9);
	perf_alpha(Fill_0_95);
	perf_alpha(Fill_0_99);
	perf_delete();


}

int main()
{
	demo();
	cout << endl;
	cout << setw(20) << "Fill Factor" << setw(20) << "Average Found" << setw(20) << "Maximum Found" << setw(20) << "Average Not Found" << setw(20) << "Maximum Not Found" << endl;
	srand(time(NULL));
	perf_all();

	return 0;
}



