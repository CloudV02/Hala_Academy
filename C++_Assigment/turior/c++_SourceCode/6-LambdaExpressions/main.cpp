#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> nums = {3, 7, 2, 9, 4};

    // Lambda to check if a number is even
    auto isEven = [](int x) { return x % 2 == 0; };
    auto isOdd = [](int x) { return x % 2 != 0; };

    cout << "Even numbers: ";
    for (int n : nums)
    {
        if (isEven(n))
        {
            cout << n << " ";
        }
    }

    cout << "\nOdd numbers: ";
	for (int n : nums)
	{
		if (isOdd(n))
		{
			cout << n << " ";
		}
	}

    return 0;
}
