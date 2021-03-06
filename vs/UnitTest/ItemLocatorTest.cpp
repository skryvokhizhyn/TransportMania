#include <boost/test/unit_test.hpp>

#include <ItemLocator.h>
#include <Point2d.h>

using namespace trm;

BOOST_AUTO_TEST_CASE(ItemLocatorTest1)
{
	ItemLocator il;
	UniqueId id = UniqueId::Generate();
	il.Put(id, {Point2d(0, 0), Point2d(0, 1), Point2d(1, 1), Point2d(1, 0)});

	BOOST_CHECK_EQUAL(il.At({Point2d(0, 0), Point2d(0, 1), Point2d(1, 1)}).size(), 1u);
	BOOST_CHECK_EQUAL(il.At({Point2d(0.3f, 0.3f), Point2d(0.3f, 0.6f), Point2d(0.6f, 0.6f)}).size(), 1u);
	BOOST_CHECK_EQUAL(il.At({Point2d(-0.3f, -0.3f), Point2d(0.3f, 0.6f), Point2d(0.6f, 0.6f)}).size(), 1u);
	BOOST_CHECK_EQUAL(il.At({Point2d(-0.3f, -0.3f), Point2d(-0.3f, 2.0f), Point2d(2.6f, 2.6f), 
		Point2d(2.6f, -0.3f)}).size(), 1u);

	BOOST_CHECK_EQUAL(il.At({Point2d(-0.3f, -0.3f), Point2d(-0.3f, -0.6f), Point2d(-0.6f, -0.6f)}).size(), 0u);

	il.Remove(id);

	BOOST_CHECK_EQUAL(il.At({Point2d(0, 0), Point2d(0, 1), Point2d(1, 1)}).size(), 0u);
}

BOOST_AUTO_TEST_CASE(ItemLocatorTest2)
{
	ItemLocator il;
	UniqueId id = UniqueId::Generate();
	il.Put(id, {Point2d(0, 0), Point2d(0, 1), Point2d(1, 1), Point2d(1, 0)});

	BOOST_CHECK_THROW(il.Put(id, {Point2d(0, 0)}), std::runtime_error);
	BOOST_CHECK_THROW(il.Remove(UniqueId::Generate()), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(ItemLocatorTest3)
{
	ItemLocator il;
	UniqueId id1 = UniqueId::Generate();
	UniqueId id2 = UniqueId::Generate();

	il.Put(id1, {Point2d(-1, -1), Point2d(-1, 1), Point2d(0, 1), Point2d(0, -1)});
	il.Put(id2, {Point2d(1, 1), Point2d(1, -1), Point2d(0, -1), Point2d(0, 1)});
	
	auto ids = il.At({Point2d(-0.5, -0.5), Point2d(-0.5, 0.5), Point2d(0.5, 0.5)});

	// order not guaranteed
	BOOST_CHECK_EQUAL(ids[0], id1);
	BOOST_CHECK_EQUAL(ids[1], id2);
}

#include <map>

BOOST_AUTO_TEST_CASE(ItemLocatorTest4)
{
	ItemLocator il;
	il.Put(UniqueId::Generate(), {Point2d(-2, -1), Point2d(-2, 1), Point2d(-1, 1), Point2d(-1, -1)});
	il.Put(UniqueId::Generate(), {Point2d(2, 1), Point2d(2, -1), Point2d(1, -1), Point2d(1, 1)});
	
	BOOST_CHECK_EQUAL(il.At({Point2d(-0.5, -0.5), Point2d(-0.5, 0.5), Point2d(0.5, 0.5)}).size(), 0u);
}

BOOST_AUTO_TEST_CASE(ItemLocatorTest5)
{
	ItemLocator il;
	il.Put(UniqueId::Generate(), {Point2d(0, 0), Point2d(1, 1), Point2d(2, 2), Point2d(3, 3), Point2d(3, 2), Point2d(2, 1), Point2d(1, 0)});
	
	BOOST_CHECK_EQUAL(il.At({Point2d(1, 2), Point2d(1.5f, 2.0f), Point2d(2, 3)}).size(), 0u);
}
