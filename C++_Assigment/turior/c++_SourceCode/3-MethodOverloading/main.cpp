#include "Calculator.h"
#include <iostream>
using namespace std;

int main()
{
	Calculator c1;
	cout << "The addition of two integers (2 and 5): " << c1.add(2, 5) << endl;
	cout << "The addition of three integers (1, 2 and 3): " << c1.add(1, 2, 3) << endl;
	cout << "The addition of two floats (3.1 and 5.2): " << c1.add(3.1F, 5.2F) << endl;

	return 0;
}
