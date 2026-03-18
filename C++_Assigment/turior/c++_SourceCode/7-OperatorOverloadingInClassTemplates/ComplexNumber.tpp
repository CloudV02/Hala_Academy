#include <iostream>
using namespace std;

template <typename T>
ComplexNumber<T>::ComplexNumber(T r, T i) : real(r), imag(i) {}

// Overload the '+' operator
template <typename T>
ComplexNumber<T> ComplexNumber<T>::operator+(const ComplexNumber<T>& other) 
{
	return ComplexNumber<T>(real + other.real, imag + other.imag);
}

template <typename T>
void ComplexNumber<T>::display() const 
{
	cout << real << " + " << imag << "i" << endl;
}