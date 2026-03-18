#include <iostream>
#include <thread>
#include <vector>
using namespace std;

void calculateSquare(int num)
{
	int square = num * num;

	cout << "Square of " << num << " is " << square << endl;
}

int main()
{
	vector<thread> threads;
	for (int i = 1; i <= 5; i++)
	{
		threads.push_back(thread(calculateSquare, i));
	}

	for (auto& th : threads)
	{
		th.join(); // Wait for all threads to finish
	}

	return 0;
}
