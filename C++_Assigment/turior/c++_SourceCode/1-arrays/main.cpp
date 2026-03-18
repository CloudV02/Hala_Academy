#include <iostream>
using namespace std;

int main()
{
	//declaration and initialization of an array
    int scores[3] = {85, 90, 78};

    //Accessing and modifying
    scores[2] = 100; // 85 90 100

    //Looping through an array
    for (int i = 0; i < 3; i++)
    {
        cout << scores[i] << endl;
    }

	return 0;
}
