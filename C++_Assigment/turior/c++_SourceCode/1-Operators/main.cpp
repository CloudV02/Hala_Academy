#include <iostream>
using namespace std;

int main()
{
	int add;
	int subtract;
	int mult;
	double div;
	int mod;
	int number1 = 10;
	int number2 = 10;

	/*
	  Arithmetic operators
	 */
	// Addition
	add = number1 + number2;

	// Subtraction
	subtract = number1 - number2;

	// Multiplication
	mult = number1 * number2;

	// Division
	div = static_cast<double>(number1)/ static_cast<double>(number2);
	//div = (double)number1/ (double)number2; // C-style cast
	cout << "division is equal to : " << div << endl;

	// Modulus
	mod = number1 % number2;
	cout << "Modulus of number1 divided by number2 is : " << mod << endl;

	/*
		Logical operators
	 */
	bool isGreater = number1 >= number2;
	bool isEqual = (number1 == number2);

	// Logical AND
	cout << boolalpha << endl; // tức là bthg bool khi in ra nó sẽ in là 1, nhưng mà khi ta gọi cái boolalpha này thì nó sẽ in ra là chữ 'true' chứ không phải là 1 nữa
	bool andResult = isGreater && isEqual; // false, because isEqual is false
	/*
	AND LOGICAL OPERATOR
	a  b  result
	0  0    0
	0  1    0
	1  0    0
	1  1    1
	*/
	// Logical OR
	bool orResult = isGreater || isEqual; // true, because isGreater is true

	/*
	OR LOGICAL OPERATOR
	a  b  result
	0  0    0
	0  1    1
	1  0    1
	1  1    1
	*/

	// Logical NOT
	bool notResult = !isEqual; // true, because isEqual is false

	// Output results
	cout << "AND Result: " << andResult << endl;
	cout << "OR Result: " << orResult << endl;
	cout << "NOT Result: " << notResult << endl;

	return 0;
}
