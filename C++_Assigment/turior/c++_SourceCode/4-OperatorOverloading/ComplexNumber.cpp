#include "ComplexNumber.h"

ComplexNumber::ComplexNumber(double r, double i) : real(r), imag(i) {}

ComplexNumber ComplexNumber::operator+(const ComplexNumber& other) const
{
	return ComplexNumber(real + other.real, imag + other.imag);
}

bool ComplexNumber::operator==(const ComplexNumber& other) const
{
	return (real == other.real && imag == other.imag);
}

ostream& operator<<(ostream& out, const ComplexNumber& c)
{
	out << c.real << " + " << c.imag << "i";
	return out;
}

void ComplexNumber::display() const
{
	cout << real << " + " << imag << "i" << endl;
}
