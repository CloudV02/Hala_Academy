#include <iostream>
using namespace std;

void passByValue(int x)
{
    x = 100;  // only modifies the copy
    if(x > 50)
    {
    	x++; // x= x+1
    	x *= 2; // x = x * 2
    	cout << "Value of x inside the function passByValue :" << x << endl;
    }
}

void passByReference(int& x)
{
    x = 100;  // modifies the original variable
}

int main()
{
    int num = 50;
    passByValue(num);
    cout << "After passing by value, num = " << num << endl;  // Output: 50

    passByReference(num);
    cout << "After passing by reference, num = " << num << endl;  // Output: 100

    return 0;
}
