#pragma once

#include <vector>

namespace utils
{
	class PoolUsageCounter;

	class PoolUsageCounterHolder
	{
	private:
		using Counters = std::vector<const PoolUsageCounter *>;

	public:
		static PoolUsageCounterHolder & Get();

		void Register(const PoolUsageCounter * pCounter);

		void Validate() const;

	private:
		PoolUsageCounterHolder();

	private:
		Counters counters_;
	};

} // namespace utils
