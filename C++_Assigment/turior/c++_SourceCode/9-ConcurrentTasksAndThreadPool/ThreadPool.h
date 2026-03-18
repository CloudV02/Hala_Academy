#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <iostream>
#include <thread>
#include <vector>
#include <queue>
#include <mutex>
#include <condition_variable>
#include <functional>
using namespace std;

class ThreadPool
{
	private:
		vector<thread> workers;
		queue<function<void()>> tasks;
		mutex mtx;
		condition_variable cv;
		bool stop = false;

	public:
		ThreadPool(size_t numThreads);
		void enqueue(function<void()> task);
		~ThreadPool();
};

#endif
