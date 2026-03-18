#include <iostream>
#include <vector>
#include <algorithm>
using namespace std;

int main()
{
    vector<int> data = {4, 7, 1, 9, 3, 6};
    unsigned int count = 0;

    cout << "Numbers: ";
    // Lambda to print elements
    auto print = [](int x) { cout << x << " "; };
    for_each(data.begin(), data.end(), print);
    cout << endl;

    // Sort in descending order using lambda
    sort(data.begin(), data.end(), [](int a, int b) { return a > b;});

    // Print sorted data
    cout << "Sorted (desc): ";
    for (int x : data)
    {
        cout << x << " ";
    }
    cout << endl;

    // Find first element greater than 5
    auto it = find_if(data.begin(), data.end(), [](int x) { return x > 5;});

    // Print found value
    if (it != data.end())
    {
        cout << "First value > 5: " << *it << endl;
    }
    else
    {
        cout << "No value > 5 found." << endl;
    }

    //stateful lambda expression
    auto conditionalFilter = [&count](int n)
    {
        count++;
        return (count <= 3 && n % 2 != 0);
    };

    auto it2 = find_if(data.begin(), data.end(), conditionalFilter);
    while (it2 != data.end())
    {
        cout << "Found: " << *it2 << endl;
        it2 = find_if(++it2, data.end(), conditionalFilter);
    }

    return 0;
}
