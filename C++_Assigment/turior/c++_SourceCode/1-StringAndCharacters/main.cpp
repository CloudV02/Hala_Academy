#include <string>
#include <iostream>
using namespace std;

int main()
{
	//Declaration and initialization of a string
	string name = "Alice";
	cout << "Hello, " << name << endl;

	//Character Access
	char first = name[0];
	cout << "First later of " << name << " is " << first << endl;

	//length of a string
	cout << "Length is " << name.length() << endl;

	//substring (part of the string) (substr(start, length))
	cout << "The two first letters are " << name.substr(0, 2) << endl;
	cout << "The three last letters are " << name.substr(2, 3) << endl;

	//Displaying name in capital letters
    for (unsigned int i = 0; i < name.length(); i++)
    {
        cout << static_cast<char>(toupper(static_cast<int>(name[i]))) << endl; // name[0] = 'A', name[1] = 'l'
    }

    cout << name << endl;

	return 0;
}
