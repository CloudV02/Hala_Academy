#ifndef CUSTOMALLOCATOR_H
#define CUSTOMALLOCATOR_H

#include <cstdlib> // for use of size_t

class CustomAllocator
{
	public:
		static void* allocate(size_t size);
		static void deallocate(void* pointer);
};

#endif
