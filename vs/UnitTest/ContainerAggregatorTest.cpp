#include <ContainerAggregator.h>

#include <boost/test/unit_test.hpp>

#include <boost/range/algorithm/for_each.hpp>

#include <vector>

using namespace utils;

namespace
{
	struct Callback2
	{
		void operator () (int a, int b)
		{
			++cnt;
			val += (a + b) * cnt;
		}

		int val = 0;
		int cnt = 0;
	};

	struct Callback3
	{
		void operator () (int a, int b, int c)
		{
			++cnt;
			val += (a + b + c) * cnt;
		}

		int val = 0;
		int cnt = 0;
	};
}

BOOST_AUTO_TEST_CASE(ArgumentUnpackerTest1)
{
	using Container = std::vector<int>;
	Container v{0, 1};
	Callback2 cb;

	priv::ArgumentUnpacker<Callback2, Container, int, 2>::Apply(cb, v);

	BOOST_CHECK_EQUAL(cb.val, 1);
	BOOST_CHECK_EQUAL(cb.cnt, 1);
}

BOOST_AUTO_TEST_CASE(ContainerAggregatorTest1)
{
	using Container = std::vector<int>;
	Container v{0, 1, 2, 3, 4};
	Callback2 cb;

	ContainerAggregator<Callback2, int, 2> ca(cb);

	boost::for_each(v, ca);

	BOOST_CHECK_EQUAL(cb.val, 11);
	BOOST_CHECK_EQUAL(cb.cnt, 2);
}

BOOST_AUTO_TEST_CASE(ContainerAggregatorTest2)
{
	using Container = std::vector<int>;
	Container v{0, 1, 2, 3, 4, 5};
	Callback2 cb;

	ContainerAggregator<Callback2, int, 2> ca(cb);

	boost::for_each(v, ca);

	BOOST_CHECK_EQUAL(cb.val, 1 + 10 + 27);
	BOOST_CHECK_EQUAL(cb.cnt, 3);
}

BOOST_AUTO_TEST_CASE(ContainerAggregatorTest3)
{
	using Container = std::vector<int>;
	Container v{0, 1, 2, 3, 4, 5};
	Callback3 cb;

	ContainerAggregator<Callback3, int, 3> ca(cb);

	boost::for_each(v, ca);

	BOOST_CHECK_EQUAL(cb.val, 3 + 24);
	BOOST_CHECK_EQUAL(cb.cnt, 2);
}