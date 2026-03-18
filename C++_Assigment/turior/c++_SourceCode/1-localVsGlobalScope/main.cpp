#include <iostream>
#include <string>
using namespace std;

int counter; // global variable

void greet()
{
	string name = "Sam"; // Local to greet()
	cout << "Hi " << name << endl;
}

void test()
{
	int x; // uninitialized local variable
	cout << x << endl; // ❌ Unsafe: value is unpredictable
	double y;
	cout << y << endl;
}

void increase()
{
	counter++; // modifies the global variable
}

int main()
{
	greet();
	test();
	cout <<"Counter value before calling increase : " << counter << endl;
	increase();
	cout <<"Counter value is now : " << counter << endl;

	return 0;
}
