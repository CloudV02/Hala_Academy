#include <iostream>
using namespace std;

int main()
{
	unsigned int score = 80;
	unsigned int age = 19;
	bool isStudent = true;

	//Just an if
	if (score >= 90)
	{
		// Grade A
		cout << "Excellent" << endl;
	}

	//If and else
	if (score > 50)
	{
		// Over the average
		cout << "Over the average" << endl;
	}
	else
	{
		//below the average
		cout << "Below the average" << endl;
	}


	//If, elseif, else
	if (score >= 90)
    {
        // Grade A
    	cout << "Grade A" << endl;
    }
    else if (score >= 80)
    {
        // Grade B
    	cout << "Grade B" << endl;
    }
    else
    {
        // Grade C or lower
    	cout << "Other grades" << endl;
    }

    //If with at least one condition that must be true
    if (age < 18 || isStudent) // || = OR, && = and, ! = not
    {
        cout << "You qualify for a discount." << endl;
    }
    else
    {
        cout << "No discount available." << endl;
    }

    //If with both conditions that must be true
    if (age > 18 && score >= 70)
    {
    	cout << "You will get a reward";
    }

	return 0;
}
