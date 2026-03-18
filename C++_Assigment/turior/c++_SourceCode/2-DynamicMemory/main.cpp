#include <iostream>
using namespace std;

int main()
{
	// pointers (stack memory)
	int num = 20;
	int* ptr2 = &num;
    // Dynamically allocate memory for an integer (heap memory = new)
    int* ptr = new int;   // new allocates memory and returns its address
    int size; // array size

    *ptr = 25;            // store value 25 in the dynamically allocated memory

    // Display the value and address
    cout << "Value stored in dynamic memory (*ptr): " << *ptr << endl;
    cout << "Address of dynamic memory (ptr): " << ptr << endl;


    //Dynamic memory with an array
	cout << "Enter number of elements: ";
	cin >> size;

	// Allocate dynamic array
	int* arr = new int[size];

	// Input values
	cout << "Enter " << size << " integers:\n";
	for (int i = 0; i < size; ++i)
	{
	   cout << "Element " << i + 1 << ": ";
	   cin >> arr[i];
	}

	// Print values
	cout << "\nYou entered:\n";
	for (int i = 0; i < size; ++i)
	{
	   cout << arr[i] << " ";
	}
	cout << endl;

    // Free the dynamically allocated memory
    delete ptr;           // always delete memory allocated with new
    // Free the memory
    delete[] arr;

    return 0;
}
