#include "PoolUsageCounter.h"
#include "PoolUsageCounterHolder.h"

using namespace utils;

PoolUsageCounter::PoolUsageCounter(std::string name)
	: count_(0)
	, name_(std::move(name))
{
	PoolUsageCounterHolder::Get().Register(this);
}

void 
PoolUsageCounter::Inc() 
{ 
	++count_; 
}

void 
PoolUsageCounter::Dec() 
{ 
	--count_; 
}

int
PoolUsageCounter::Count() const
{
	return count_;
}

const std::string &
PoolUsageCounter::Name() const
{
	return name_;
}
