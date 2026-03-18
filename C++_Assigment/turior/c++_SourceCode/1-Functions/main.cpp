#include <iostream>
#include <string>
using namespace std;

//no return value, no parameters
void greet()
{
	cout << "Hello World!" << endl;
}

//no return value but with parameters
void greetUser(string name)
{
	cout << "Hello, " << name << "!" << endl;
}

//return value without parameters
int getLuckyNumber()
{
	return 7;
}

//return value and parameters
int add(int a, int b)
{
	return a + b;
}

int main()
{
	greet(); // calls a void function
	greetUser("Alice"); // passes a string
	int lucky = getLuckyNumber(); // stores return value
	cout << "Our lucky number is: " << lucky << endl;
	int result = add(lucky, 3); // stores sum
	cout << "Sum is: " << result << endl;

	return 0;
}
