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
	
	int i = 0;
	bool d = false;

	BOOST_CHECK(sm.Find(SymmetricIntMap::KeyPair(1, 0), i, d));
	BOOST_CHECK_EQUAL(i, 1);
	BOOST_CHECK(!d);

	BOOST_CHECK(sm.Find(SymmetricIntMap::KeyPair(1, 2), i, d));
	BOOST_CHECK_EQUAL(i, 2);
	BOOST_CHECK(!d);

	BOOST_CHECK(sm.Find(SymmetricIntMap::KeyPair(2, 1), i, d));
	BOOST_CHECK_EQUAL(i, 2);
	BOOST_CHECK(d);

	BOOST_CHECK(!sm.Find(SymmetricIntMap::KeyPair(1, 3), i, d));

	BOOST_CHECK(sm.Erase(SymmetricIntMap::KeyPair(1, 0)));

	BOOST_CHECK(!sm.Find(SymmetricIntMap::KeyPair(1, 0), i, d));
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
