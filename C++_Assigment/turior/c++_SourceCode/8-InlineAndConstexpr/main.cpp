#include <iostream>
using namespace std;

constexpr double calculateArea(double radius)
{
	return 3.14159 * radius * radius; // compile-time calculation
}

inline int add(int a, int b)
{
	 return a + b;
}

int main()
{
	double area = calculateArea(5.0); // computed at compile-time
	cout << "Area: " << area << endl;

	int sum = add(10, 20); // no overhead from function call
	cout << "Sum: " << sum << endl;

	return 0;
}
