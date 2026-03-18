#include <iostream>
using namespace std;

int main()
{
    int guess;
    int number = 7;
    unsigned int count = 0;

    cout << "Guess the number: ";
    cin >> guess;

    while (guess != number)
    {
        cout << "Wrong! Try again: ";
        cin >> guess;
    }

    cout << "Correct! Number to guess was : " << number << endl;

    do
    {
        cout << "Guess the number: ";
        cin >> guess;
    }
    while (guess != number);

    cout << "Correct! Number to guess was : " << number << endl;

	return 0;
}
