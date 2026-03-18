#include "Pair.h"
#include <string>
using namespace std;

int main()
{
	Pair<int> intPair(1, 2);
	intPair.display();

	Pair<string> stringPair("Hello", "World");
	stringPair.display();

	return 0;
}
