#ifndef COMPLEXNUMBER_H
#define COMPLEXNUMBER_H

template <typename T>
class ComplexNumber
{
private:
	T real;
	T imag;

public:
	ComplexNumber(T r, T i);
	// Overload the '+' operator
	ComplexNumber<T> operator+(const ComplexNumber<T>& other);
	void display() const;
};

#include "ComplexNumber.tpp"

#endif

