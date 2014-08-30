#include "PoolUsageCounterHolder.h"
#include "PoolUsageCounter.h"
#include "Logger.h"

#include <boost/range/algorithm/for_each.hpp>

using namespace utils;

PoolUsageCounterHolder & 
PoolUsageCounterHolder::Get()
{
	static PoolUsageCounterHolder instance;
	return instance;
}

PoolUsageCounterHolder::PoolUsageCounterHolder()
{}

void 
PoolUsageCounterHolder::Register(const PoolUsageCounter * pCounter)
{
	counters_.push_back(pCounter);
}

void 
PoolUsageCounterHolder::Validate() const
{
	boost::for_each(counters_,
		[](const Counters::value_type & v)
	{
		if (v->Count())
		{
			utils::Logger().Error() << "Memory Leaks detected for pool [" << v->Name() << "]. "
				<< "Count = " << v->Count();
		}
	});
}
