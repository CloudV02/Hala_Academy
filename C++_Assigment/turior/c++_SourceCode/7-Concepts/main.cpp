#include <iostream>
using namespace std;

template<typename T>
concept Numeric = is_arithmetic<T>::value;

template<Numeric T>
T add(T a, T b)
{
	return a + b;
}

int main()
{
	cout << add(5, 3) << endl;	   // Works fine
	cout << add(3.14, 2.71) << endl; // Works fine

	// Uncommenting the following line would cause a compile-time error:
	//cout << add("Hello", "World") << endl; // Compile-time error

	return 0;
}
