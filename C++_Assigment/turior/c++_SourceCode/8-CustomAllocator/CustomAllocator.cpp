#include "CustomAllocator.h"
#include <iostream>
using namespace std;

void* CustomAllocator::allocate(size_t size)
{
	return operator new(size); // allocate memory manually
}

void CustomAllocator::deallocate(void* pointer)
{
	cout << "Memory of the pointer freed." << endl;
	operator delete(pointer); // manually free memory
}
