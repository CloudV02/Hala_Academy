#include <iostream>
using namespace std;

//with metaprogramming
template <int A, int B>
struct GCD
{
	static const int value = GCD<B, A % B>::value;
};

template <int A>
struct GCD<A, 0>
{
	static const int value = A;
};

//without metaprogramming
int gcdCalculation(int A, int B)
{
	if (B == 0)
	{
		return A;
	}
	return gcdCalculation(B, A % B);
}

int main()
{
	cout << "GCD of 56 and 98: " << GCD<56, 98>::value << endl; // Output: 14
	cout << "GCD of 56 and 98: " << gcdCalculation(56, 98) << endl; // Output: 14
	return 0;
}
