#include "ThreadPool.h"
#include "SafeExecute.h"

using namespace utils;

ThreadPool::ThreadPool(std::uint8_t cnt)
	: stopped_(false)
{
	threads_.reserve(cnt);
	for (auto i = 0; i < cnt; ++i)
	{
		threads_.emplace_back([=]() 
		{
			this->Run();
		});
	}
}

void
ThreadPool::Enqueue(TaskPtr && task)
{
	{
		std::unique_lock<std::mutex> _(mutex_);
		queue_.push_back(std::move(task));
	}
	
	condTaskAdded_.notify_one();
}

void
ThreadPool::Stop()
{
	stopped_.store(true, std::memory_order_release);
	
	// force all threads to wake up if no pending tasks are in the queue
	condTaskAdded_.notify_all();

	// wait for completion of all threads
	for (auto & thread : threads_)
	{
		thread.join();
	}
}

ThreadPool::TaskPtr
ThreadPool::Dequeue()
{
	TaskPtr task;

	{
		std::unique_lock<std::mutex> guard(mutex_);
		condTaskAdded_.wait(guard,
			[&]() { return !queue_.empty() || stopped_.load(std::memory_order_acquire); });

		if (queue_.empty())
			return TaskPtr();

		task = std::move(queue_.front());
		queue_.pop_front();
	}

	return task;
}

void
ThreadPool::Run()
{
	bool running = true;

	while (running)
	{
		utils::SafeExecuteWithLog([=, &running]()
		{
			TaskPtr task = Dequeue();
			if (!task)
			{
				running = false;
				return;
			}

			task->Do();
		});
	}
}

ThreadPool::Task::~Task()
{
}
