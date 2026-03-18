#include "Person.h"
#include <memory> // for use of unique_ptr
using namespace std;

int main()
{
	unique_ptr<Person> personPtr = make_unique<Person>("John");
	personPtr->greet();
	// No need to call delete; the memory is automatically managed
	return 0;
}
