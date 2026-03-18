#include <iostream>
#include <list>
using namespace std;

void display(list<int> l)
{
	for (int n : l)
	{
		cout << n << " ";
	}
	cout << endl;
}

int main()
{
	list<int> numbers;

	// Add elements
	numbers.push_back(10); // 10
	numbers.push_back(20); // 10 20
	numbers.push_front(5); // 5 10 20

	display(numbers); // Output: 5 10 20

	// Remove element
	numbers.pop_front(); // Removes 5
	//numbers.pop_back(); // Removes 20

	// Insert 15 after first element (iterator)
	list<int>::const_iterator it = numbers.cbegin();
	it++; // move to second element
	numbers.insert(it, 15); // Insert before 20

	display(numbers); // Output: 10 15 20

	return 0;
}
