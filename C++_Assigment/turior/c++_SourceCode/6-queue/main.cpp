#include <iostream>
#include <queue>
using namespace std;

void displayQueue(queue<int> q)
{
	cout << "[ ";
	while (!q.empty())
	{
		cout << q.front() << " "; //q.front() is the first element of the queue
		q.pop(); // removes the top element
	}
	cout << "]" << endl;
}

int main()
{
	queue<int> q; //FIFO (First In First Out)

	// Pushing elements into the queue
	q.push(10); // Container state: [10]
	displayQueue(q);
	q.push(20); // Container state: [10, 20]
	displayQueue(q);
	q.push(30); // Container state: [10, 20, 30]
	displayQueue(q);

	// Popping elements from the queue
	cout << "First pop: ";
	q.pop(); // First pop: Container state: [20, 30]
	displayQueue(q);

	cout << "Second pop: ";
	q.pop(); // Second pop: Container state: [30]
	displayQueue(q);

	cout << "Third pop: ";
	q.pop(); // Third pop: Container state: [empty]
	displayQueue(q);

	cout << endl;

	return 0;
}
