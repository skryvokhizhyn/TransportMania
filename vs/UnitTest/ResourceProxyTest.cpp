#include <boost/test/unit_test.hpp>

#include <ResourceProxy.h>

using namespace utils;

struct Test
{
	int i;

	int Get() { return i; }
	int Get() const { return i; }

	Test(int a, int b)
		: i(a + b)
	{}
};

BOOST_AUTO_TEST_CASE(ResourceProxyTest1)
{
	ResourceProxy<Test, StaticHoldingStrategy> rpt;
	
	rpt.Init(2, 3);

	BOOST_CHECK_EQUAL(rpt->i, 5);

	rpt->i = 6;
	BOOST_CHECK_EQUAL(rpt->i, 6);

	ResourceProxy<Test, StaticHoldingStrategy> rpt1;
	BOOST_CHECK_EQUAL(rpt1->i, 6);

	rpt.Term();	
}

BOOST_AUTO_TEST_CASE(ResourceProxyTest2)
{
	ResourceProxy<Test, StaticHoldingStrategy> rpt;
	
	rpt.Init(2, 3);

	const ResourceProxy<Test, StaticHoldingStrategy> rpt1;
	
	BOOST_CHECK_EQUAL(rpt->i, 5);
	BOOST_CHECK_EQUAL(rpt1->i, 5);

	BOOST_CHECK_EQUAL(rpt1->Get(), 5);
	BOOST_CHECK_EQUAL(rpt->Get(), 5);

	rpt.Term();	
}

BOOST_AUTO_TEST_CASE(ResourceProxyTest3)
{
	ResourceProxy<Test, ByValueHoldingStrategy> rpt(1, 2);

	BOOST_CHECK_EQUAL(rpt->i, 3);

	rpt->i = 6;
	BOOST_CHECK_EQUAL(rpt->i, 6);

	ResourceProxy<Test, ByValueHoldingStrategy> rpt1(2, 3);
	BOOST_CHECK_EQUAL(rpt1->i, 5);

	rpt1 = rpt;
	BOOST_CHECK_EQUAL(rpt1->i, 6);
}

BOOST_AUTO_TEST_CASE(ResourceProxyTest4)
{
	Test t(1, 2);

	ResourceProxy<Test, StaticRefHoldingStrategy> rpt;
	rpt.Init(t);

	BOOST_CHECK_EQUAL(rpt->i, 3);

	rpt->i = 6;
	BOOST_CHECK_EQUAL(rpt->i, 6);

	ResourceProxy<Test, StaticRefHoldingStrategy> rpt1;
	rpt1 = rpt;
	BOOST_CHECK_EQUAL(rpt1->i, 6);
}

BOOST_AUTO_TEST_CASE(ResourceProxyTest5)
{
	const Test t(1, 2);

	ResourceProxy<const Test, StaticRefHoldingStrategy> rpt;
	rpt.Init(t);

	BOOST_CHECK_EQUAL(rpt->i, 3);

	ResourceProxy<const Test, StaticRefHoldingStrategy> rpt1;
	rpt1 = rpt;
	BOOST_CHECK_EQUAL(rpt1->i, 3);
}