#include <iostream>
#include <stack>
using namespace std;

void displayStack(stack<int> s)
{
	cout << "[";
	while (!s.empty())
	{
		cout << s.top() << " "; //s.top() is the last element of the stack
		s.pop(); // removes the top element
	}
	cout << "]" << endl;
}

int main()
{
	stack<int> s; //LIFO (Last In First Out)

	// Pushing elements onto the stack
	s.push(10); // Container state: [10]
	displayStack(s);
	s.push(20); // Container state: [10, 20]
	displayStack(s);
	s.push(30); // Container state: [10, 20, 30]
	displayStack(s);

	// Popping elements from the stack
	cout << "First pop: ";
	s.pop(); // First pop: Container state: [10, 20]
	displayStack(s);

	cout << "Second pop: ";
	s.pop(); // Second pop: Container state: [10]
	displayStack(s);

	cout << "Third pop: ";
	s.pop(); // Third pop: Container state: [empty]
	displayStack(s);

	cout << endl;

	return 0;
}
