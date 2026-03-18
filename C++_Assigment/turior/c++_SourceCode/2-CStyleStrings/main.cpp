#include <cstring>
#include <iostream>
#include <string>
using namespace std;


int main()
{
	//Declaring and initializing C-style strings
	char name[] = "Alice";
	char name2[4] = "Tom";
	char name3[14];

	//Length a C-Style string
	cout << strlen(name) << endl;

	//Copy an array into another one
	strcpy(name3, name2); //Copying of name2 into name3
	strcat(name3, " and "); //Adding " and " at the end of name3
	strcat(name3, name); //Adding "Alice" at the end of name3
	cout << name3 << endl; // "Tom and Alice"

	//comparing strings
	if(strcmp(name3, name2) != 0)
	{
		cout << "They are not the same" << endl;
	}
	else
	{
		cout << "They are the same" << endl;
	}

	return 0;
}
