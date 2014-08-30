//#define BOOST_TEST_DYN_LINK
#define BOOST_TEST_MODULE Unit_Tests
#define BOOST_TEST_NO_MAIN

#include <LeakDetectorInitializer.h>
#include <PoolUsageCounterHolder.h>

#include <boost/test/unit_test.hpp>

using namespace utils;

BOOST_GLOBAL_FIXTURE(LeakDetectorInitializer);

struct PoolUsageCounterHolderRAII
{
	~PoolUsageCounterHolderRAII()
	{ utils::PoolUsageCounterHolder::Get().Validate(); }
};

BOOST_GLOBAL_FIXTURE(PoolUsageCounterHolderRAII);