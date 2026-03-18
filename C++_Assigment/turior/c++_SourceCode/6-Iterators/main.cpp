#include <iostream>
#include <vector>
using namespace std;

int main()
{
    vector<string> fruits = {"apple", "banana", "cherry"};

    // Regular iterator – Modify first element
    vector<string>::iterator it = fruits.begin();
    for (it = fruits.begin(); it != fruits.end(); it++)
   	{
   		cout << *it << " "; // apple banana cherry
   	}
   	cout << endl;

   	it = fruits.begin();
    *it = "grape"; // replaces "apple"


    // const_iterator – Read-only traversal
    cout << "Using const_iterator: ";
    vector<string>::const_iterator cit;
    for (cit = fruits.cbegin(); cit != fruits.cend(); cit++)
    {
        cout << *cit << " "; // grape banana cherry
    }
    cout << endl;

    // reverse_iterator – Traverse in reverse order
    cout << "Using reverse_iterator: ";
    vector<string>::reverse_iterator rit;
    for (rit = fruits.rbegin(); rit != fruits.rend(); rit++)
    {
        cout << *rit << " "; // cherry banana grape
    }
    cout << endl;

    return 0;
}
