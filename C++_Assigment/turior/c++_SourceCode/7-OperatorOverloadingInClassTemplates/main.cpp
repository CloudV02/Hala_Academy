#include "ComplexNumber.h"

int main()
{
	ComplexNumber<int> c1(1, 2), c2(3, 4);
	ComplexNumber<int> c3 = c1 + c2;

	c3.display(); // Output: 4 + 6i

	ComplexNumber<double> c4(3.5, 1.5), c5(3.0, 4.0);
	ComplexNumber<double> c6 = c4 + c5;

	c6.display(); // Output: 6.5 + 5.5i

	return 0;
}
