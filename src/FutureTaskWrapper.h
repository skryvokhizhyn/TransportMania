#pragma once

#include "ThreadPool.h"

#include <future>

namespace trm
{
	template<typename Ret>
	class FutureTaskWrapper
		: public utils::ThreadPool::Task
	{
	public:
		using FutureTask = std::future<Ret>;

	public:
		FutureTaskWrapper(std::function<Ret ()> && func)
			: task_(std::move(func))
		{}

		FutureTask GetFuture()
		{
			return task_.get_future();
		}

		virtual void Do() override
		{
			task_();
		}

	private:
		std::packaged_task<Ret()> task_;
	};

} // namespace trm
