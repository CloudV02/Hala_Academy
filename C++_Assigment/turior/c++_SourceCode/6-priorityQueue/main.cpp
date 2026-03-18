#include <iostream>
#include <queue>
using namespace std;

void displayQueue(priority_queue<int> q)
{
	cout << "[ ";
	while (!q.empty())
	{
		cout << q.top() << " "; //q.top() is the highest priority element in the priority queue
		q.pop(); // removes the top element
	}
	cout << "]" << endl;
}

int main()
{
	priority_queue<int> pq; //FIFO (First In First Out)

	// Inserting elements into the priority queue
	pq.push(10); // Container state: [10]
	displayQueue(pq);
	pq.push(30); // Container state: [30, 10]
	displayQueue(pq);
	pq.push(20); // Container state: [30, 20, 10]
	displayQueue(pq);

	// Displaying and removing elements from the priority queue
	cout << "First pop: ";
	pq.pop(); // First pop: Container state: [20, 10]
	displayQueue(pq);

	cout << "Second pop: ";
	pq.pop(); // Second pop: Container state: [10]
	displayQueue(pq);

	cout << "Third pop: ";
	pq.pop(); // Third pop: Container state: [empty]
	displayQueue(pq);

	cout << endl;

	return 0;
}
