#include <boost/test/unit_test.hpp>

#include <SymmetricMap.h>

using namespace trm;

typedef SymmetricMap<int, int> SymmetricIntMap;

BOOST_AUTO_TEST_CASE(SymmetricMapTest1)
{
	SymmetricIntMap sm;
	
	BOOST_CHECK(sm.Insert(SymmetricIntMap::KeyPair(0, 1), 0));
	BOOST_CHECK(!sm.Insert(SymmetricIntMap::KeyPair(0, 1), 0));
	BOOST_CHECK(!sm.Insert(SymmetricIntMap::KeyPair(1, 0), 0));
	BOOST_CHECK(sm.Insert(SymmetricIntMap::KeyPair(1, 2), 0));
	BOOST_CHECK(sm.Insert(SymmetricIntMap::KeyPair(0, 0), 0));
	BOOST_CHECK(!sm.Insert(SymmetricIntMap::KeyPair(0, 1), 0));
}

BOOST_AUTO_TEST_CASE(SymmetricMapTest2)
{
	SymmetricIntMap sm;
	
	BOOST_CHECK(sm.Insert(SymmetricIntMap::KeyPair(0, 1), 0));
	BOOST_CHECK(!sm.Insert(SymmetricIntMap::KeyPair(0, 1), 0));
	BOOST_CHECK(!sm.Erase(SymmetricIntMap::KeyPair(1, 3)));
	BOOST_CHECK(sm.Erase(SymmetricIntMap::KeyPair(0, 1)));
	BOOST_CHECK(sm.Insert(SymmetricIntMap::KeyPair(0, 1), 0));
	BOOST_CHECK(!sm.Insert(SymmetricIntMap::KeyPair(0, 1), 0));
	BOOST_CHECK(sm.Erase(SymmetricIntMap::KeyPair(1, 0)));
	BOOST_CHECK(sm.Insert(SymmetricIntMap::KeyPair(0, 1), 0));
	BOOST_CHECK(!sm.Insert(SymmetricIntMap::KeyPair(0, 1), 0));
}

BOOST_AUTO_TEST_CASE(SymmetricMapTest3)
{
	SymmetricIntMap sm;
	
	BOOST_CHECK(sm.Insert(SymmetricIntMap::KeyPair(0, 1), 1));
	BOOST_CHECK(sm.Insert(SymmetricIntMap::KeyPair(2, 1), 2));
	
	const auto found1 = sm.Find(SymmetricIntMap::KeyPair(1, 0));

	BOOST_CHECK(found1);
	BOOST_CHECK_EQUAL(found1->first, 1);
	BOOST_CHECK(!found1->second);

	const auto found2 = sm.Find(SymmetricIntMap::KeyPair(1, 2));

	BOOST_CHECK(found2);
	BOOST_CHECK_EQUAL(found2->first, 2);
	BOOST_CHECK(!found2->second);

	const auto found3 = sm.Find(SymmetricIntMap::KeyPair(2, 1));

	BOOST_CHECK(found3);
	BOOST_CHECK_EQUAL(found3->first, 2);
	BOOST_CHECK(found3->second);

	BOOST_CHECK(!sm.Find(SymmetricIntMap::KeyPair(1, 3)));

	BOOST_CHECK(sm.Erase(SymmetricIntMap::KeyPair(1, 0)));

	BOOST_CHECK(!sm.Find(SymmetricIntMap::KeyPair(1, 0)));
}

BOOST_AUTO_TEST_CASE(SymmetricMapTest4)
{
	SymmetricIntMap sm;

	BOOST_CHECK(sm.Insert(SymmetricIntMap::KeyPair(0, 1), 1));
	BOOST_CHECK(sm.Insert(SymmetricIntMap::KeyPair(2, 1), 2));

	BOOST_CHECK(sm.Normalized(SymmetricIntMap::KeyPair(0, 1)));
	BOOST_CHECK(!sm.Normalized(SymmetricIntMap::KeyPair(1, 0)));
}

BOOST_AUTO_TEST_CASE(SymmetricMapTest5)
{
	SymmetricIntMap sm;

	sm.Insert(SymmetricIntMap::KeyPair(0, 1), 1);
	
	BOOST_CHECK(sm.Exists(SymmetricIntMap::KeyPair(0, 1)));
	BOOST_CHECK(sm.Exists(SymmetricIntMap::KeyPair(1, 0)));
	BOOST_CHECK(!sm.Exists(SymmetricIntMap::KeyPair(1, 1)));
}

BOOST_AUTO_TEST_CASE(SymmetricMapTest7)
{
	SymmetricIntMap sm;

	BOOST_CHECK(!sm.Exists(SymmetricIntMap::KeyPair(0, 1)));

	sm.Insert(SymmetricIntMap::KeyPair(0, 1), 1);
	
	BOOST_CHECK(sm.Exists(SymmetricIntMap::KeyPair(0, 1)));

	sm.Clear();

	BOOST_CHECK(!sm.Exists(SymmetricIntMap::KeyPair(0, 1)));
}
