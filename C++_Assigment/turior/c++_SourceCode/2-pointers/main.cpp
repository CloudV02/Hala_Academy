#include <iostream>
using namespace std;

int main()
{
    int num = 10;          // normal integer variable
    int num2 = 25;
    int* ptr = &num;       // pointer variable, stores address of num
    int* ptr2 = ptr;

    // Print the value and address
    cout << "Value of num: " << num << endl;
    cout << "Address of num (&num): " << &num << endl;
    cout << "Value stored in ptr (address): " << ptr << endl;
    cout << "Value pointed to by ptr (*ptr): " << *ptr << endl;

    cout << "Value stored in ptr2 (address): " << ptr2 << endl;
    cout << "Value pointed to by ptr2 (*ptr2): " << *ptr2 << endl;

    ptr = &num2;

    cout << "Value of num: " << num << endl;
	cout << "Address of num (&num): " << &num << endl;
	cout << "Value stored in ptr (address): " << ptr << endl;
	cout << "Value pointed to by ptr (*ptr): " << *ptr << endl;

	cout << "Value stored in ptr2 (address): " << ptr2 << endl;
	cout << "Value pointed to by ptr2 (*ptr2): " << *ptr2 << endl;

    return 0;
}
