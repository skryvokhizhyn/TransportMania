#pragma once

#include <boost/noncopyable.hpp>

#include <thread>
#include <deque>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <atomic>
#include <cstdint>

namespace utils
{
	class ThreadPool
		: boost::noncopyable
	{
	public:
		struct Task
		{
			virtual ~Task();
			virtual void Do() = 0;
		};

		using TaskPtr = std::unique_ptr<Task>;

	public:
		ThreadPool(std::uint8_t cnt);

		void Enqueue(TaskPtr && task);
		void Stop();

	private:
		TaskPtr Dequeue();
		void Run();

	private:
		std::vector<std::thread> threads_;
		std::mutex mutex_;
		std::condition_variable condTaskAdded_;
		std::deque<TaskPtr> queue_;
		std::atomic_bool stopped_;
	};

} // namespace trm
