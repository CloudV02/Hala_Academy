#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
using namespace std;

mutex mtx; // mutex : mutual execution
condition_variable cv;
bool dataReady = false;

void waitForData()
{
	cout << "Thread waiting for data..." << endl;
	unique_lock<mutex> lock(mtx);
	cv.wait(lock, []{ return dataReady; }); // Wait until dataReady is true
	cout << "Data is ready!" << endl;
}

void sendData()
{
	this_thread::sleep_for(chrono::seconds(2));
	cout << "Data is ready, notifying..." << endl;
	{
		lock_guard<mutex> lock(mtx);
		dataReady = true;
	}
	cv.notify_one(); // Notify one waiting thread
}

int main()
{
	thread t1(waitForData);
	thread t2(sendData);

	t1.join();
	t2.join();

	return 0;
}
