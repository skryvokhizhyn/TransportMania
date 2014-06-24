#include <WeakStoringCache.h>

#include <boost/test/unit_test.hpp>

using namespace trm;

typedef WeakStoringCache<int, int> WeakStoringCacheType;

BOOST_AUTO_TEST_CASE(WeakStoringCacheTest1)
{
	WeakStoringCacheType wsc;

	BOOST_CHECK_EQUAL(wsc.Check(1), WeakStoringCacheType::Missing);

	{
		WeakStoringCacheType::Ptr p = wsc.Get(1);

		BOOST_CHECK_EQUAL(wsc.Check(1), WeakStoringCacheType::Active);
	}

	BOOST_CHECK_EQUAL(wsc.Check(1), WeakStoringCacheType::Cleared);

	WeakStoringCacheType::Ptr p = wsc.Get(1);

	BOOST_CHECK_EQUAL(wsc.Check(1), WeakStoringCacheType::Active);
}