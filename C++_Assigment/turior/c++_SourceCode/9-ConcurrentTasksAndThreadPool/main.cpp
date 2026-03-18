#include "ThreadPool.h"
#include <thread>
#include <mutex>
using namespace std;

mutex mtx;

int main()
{
	ThreadPool pool(3);

	for (int i = 0; i < 10; ++i)
	{
		pool.enqueue([i]
		{
			lock_guard<mutex> lock(mtx);
			cout << "Task " << i << " is being processed." << endl;
		});
	}

	return 0;
}
