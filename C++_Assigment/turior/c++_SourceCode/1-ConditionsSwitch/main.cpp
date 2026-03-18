#include <iostream>
using namespace std;

int main()
{
	char grade;
	cin >> grade;
    switch (grade)
    {
        case 'A':
            cout << "Excellent!" << endl;
            break;
        case 'B':
        case 'C':
        case 'D':
            cout << "Good!" << endl;
            break;
        default:
        	cout << "Try again" << endl;
    }

    //Equivalent with an if statement
    if(grade == 'A')
    {
    	cout << "Excellent!" << endl;
    }
    else if (grade == 'B' || grade == 'C' || grade == 'D')
    {
    	cout << "Good!" << endl;
    }
    else
    {
    	cout << "Try again" << endl;
    }

	return 0;
}
