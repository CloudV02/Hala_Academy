#include <iostream>
using namespace std;

template <typename T>
void swapValues(T& a, T& b)
{
	T temp = a;
	a = b;
	b = temp;
}

int main()
{
	int x = 10, y = 20;
	swapValues(x, y);
	cout << "x: " << x << ", y: " << y << endl;

	double a = 1.5, b = 2.5;
	swapValues(a, b);
	cout << "a: " << a << ", b: " << b << endl;

	bool b1 = false, b2 = true;
	swapValues(b1, b2);
	cout << "b1: " << b1 << ", b2: " << b2 << endl;

	return 0;
}
