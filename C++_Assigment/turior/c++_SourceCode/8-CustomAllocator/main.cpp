#include "CustomAllocator.h"
#include <iostream>
using namespace std;

int main()
{
	int* ptr = static_cast<int*>(CustomAllocator::allocate(sizeof(int)));
	*ptr = 100; // assign a value
	cout << "Allocated value: " << *ptr << endl;

	CustomAllocator::deallocate(ptr); // free the memory
	return 0;
}
