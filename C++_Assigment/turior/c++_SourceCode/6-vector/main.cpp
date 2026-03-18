#include <iostream>
#include <vector>
using namespace std;

void printVector(vector<int> vec)
{
	// Print elements
	for (int v : vec)
	{
		cout << v << " ";
	}
	cout << endl;
}

int main()
{
	// Create a vector of integers and add elements
	vector<int> numbers = {10, 20, 30, 40, 50}; // 10 20 30 40 50

	// Add elements using push_back
	numbers.push_back(60); // 10 20 30 40 50 60

	printVector(numbers);

	numbers.pop_back(); // 10 20 30 40 50

	printVector(numbers);

	return 0;
}
