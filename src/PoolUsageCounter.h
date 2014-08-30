#pragma once

#include <boost/noncopyable.hpp>
#include <string>

namespace utils
{
	class PoolUsageCounter
	{
	public:
		PoolUsageCounter(std::string name);

		void Inc();
		void Dec();

		int Count() const;
		const std::string & Name() const;

	private:
		void operator = (const PoolUsageCounter &) = delete;

	private:
		int count_;
		const std::string name_;
	};

	struct DummyPoolUsageCounter
	{
		DummyPoolUsageCounter(std::string) {}

		void Inc() {}
		void Dec() {}
	};

#ifdef _DEBUG
	using PoolUsageCounterImpl = PoolUsageCounter;
#else
	using PoolUsageCounterImpl = DummyPoolUsageCounter;
#endif

} // namespace utils
