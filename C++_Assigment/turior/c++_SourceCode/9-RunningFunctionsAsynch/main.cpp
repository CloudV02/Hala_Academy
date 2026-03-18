#include <iostream>
#include <thread>
#include <future>
#include <chrono>
using namespace std;

void computeSquare(int num, promise<int> resultPromise)
{
    this_thread::sleep_for(chrono::seconds(5)); // Simulate work
    int result = num * num;
    resultPromise.set_value(result); // Send result to future
}

int main()
{
    promise<int> promise;
	future<int> result = promise.get_future(); // Receive result from another thread

	thread worker(computeSquare, 10, move(promise));

	cout << "Performing other tasks while waiting for result..." << endl;

    int square = result.get(); // Blocks until the value is available
	cout << "Square of 10 is: " << square << endl;

    worker.join(); // Ensure thread completes

    return 0;
}
