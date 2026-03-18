#include "ThreadPool.h"

ThreadPool::ThreadPool(size_t numThreads)
{
	for (size_t i = 0; i < numThreads; i++)
	{
		workers.emplace_back([this]
		{
			while (true)
			{
				function<void()> task;
				{
					unique_lock<mutex> lock(this->mtx);
					this->cv.wait(lock, [this] { return !this->tasks.empty() || this->stop; });
					if (this->stop && this->tasks.empty()) return;
					task = move(this->tasks.front());
					this->tasks.pop();
				}
				task();
			}
		});
	}
}

void ThreadPool::enqueue(function<void()> task)
{
	{
		lock_guard<mutex> lock(mtx);
		tasks.push(task);
	}
	cv.notify_one();
}

ThreadPool::~ThreadPool()
{
	{
		lock_guard<mutex> lock(mtx);
		stop = true;
	}
	cv.notify_all();
	for (thread& worker : workers)
	{
		if (worker.joinable()) worker.join();
	}
}
