#include "ComplexNumber.h"

int main()
{
	ComplexNumber c1(2.5, 2.5);
	ComplexNumber c2(1.5, 2.5);

	ComplexNumber c3 = c1 + c2; // Using overloaded '+' operator
	c3.display();

	if(c1 == c2)
	{
		cout << "There are the same" << endl;
	}
	else
	{
		cout << "They are different" << endl;
	}

	cout << c1 << endl;
}
