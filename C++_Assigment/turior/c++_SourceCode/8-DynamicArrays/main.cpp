#include <iostream>
using namespace std;

int* createArray(int size)
{
	return new int[size]; // dynamically allocate array
}

void deleteArray(int* array)
{
	delete[] array; // deallocate array memory
}

int main()
{
	const unsigned int arraySize = 5U;
	int* array = createArray(arraySize); // allocate array of 5 integers
	for (int i = 0; i < arraySize; i++)
	{
		array[i] = i * 10; // assigning values
	}

	for (int i = 0; i < arraySize; i++)
	{
		cout << array[i] << " "; // output values
	}
	cout << endl;

	deleteArray(array); // free memory
	return 0;
}
