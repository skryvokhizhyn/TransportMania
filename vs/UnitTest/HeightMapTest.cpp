//#define BOOST_TEST_DYN_LINK

#include <HeightMap.h>
#include <HeightMapExtended.h>
#include <HeightMapLoaderCache.h>
#include <HeightMapLoaderConstant.h>
#include <GeometryUtils.h>
#include <Triangle3d.h>
#include <Point2d.h>
#include <boost/test/unit_test.hpp>

using namespace trm::terrain;
using namespace trm;

namespace
{
	// this view is x below horizontal and y below vertical. As if you look at a picture
	const HeightMap::Value v4x4[] = 
	{
		0, 1, 0, 0,
		0, 1, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};

	struct Test4x4HeightMap
	{
		HeightMap::Container sample4x4 = PrepareDataFromVectorReversed(v4x4, 16);
	};
}

BOOST_FIXTURE_TEST_CASE(HeightMapTest1, Test4x4HeightMap)
{
	HeightMap hm;
	hm.Swap(sample4x4);
	BOOST_CHECK_EQUAL(hm.At(Point2d(1, 1)), 0.0f);
	BOOST_CHECK_EQUAL(hm.At(Point2d(1, 2)), 1.0f);
	BOOST_CHECK_EQUAL(hm.At(Point2d(1, 3)), 1.0f);
	BOOST_CHECK_EQUAL(hm.GetSize(), 4u);
}

BOOST_AUTO_TEST_CASE(HeightMapTest2)
{
	HeightMap::Container v = 
	{
		0, 0, 0, 0,
		0, 0, 0, 0
	};
	
	HeightMap hm;
	BOOST_CHECK_THROW (hm.Swap(v), std::runtime_error);
}

BOOST_FIXTURE_TEST_CASE(HeightMapTest3, Test4x4HeightMap)
{
	HeightMapExtended hme(4);
	hme.Swap(sample4x4);
	BOOST_CHECK_EQUAL(hme.At(Point2d(1, 2)), 1.0f);
	BOOST_CHECK_EQUAL(hme.At(Point2d(1, 3)), 1.0f);
}

BOOST_FIXTURE_TEST_CASE(HeightMapTest4, Test4x4HeightMap)
{
	HeightMapExtended hme(16);
	hme.Swap(sample4x4);
	BOOST_CHECK_EQUAL(hme.At(Point2d(2, 5)), 0.125f);
}

BOOST_FIXTURE_TEST_CASE(LODUtilsTest1, Test4x4HeightMap)
{
	HeightMapExtended hme(5);
	hme.Swap(sample4x4);

	Triangle3d t(Point3d(0, 0, 0), Point3d(0, 4, 0), Point3d(4, 4, 0));
	Triangle3dPair tp = utils::SplitTriangle(t, hme);

	BOOST_CHECK_EQUAL(tp.first.l(), Point3d(0, 4, 0));
	BOOST_CHECK_EQUAL(tp.first.e(), Point3d(2, 2, 0.36f));
	BOOST_CHECK_EQUAL(tp.first.r(), Point3d(0, 0, 0));
	BOOST_CHECK_EQUAL(tp.second.l(), Point3d(4, 4, 0));
	BOOST_CHECK_EQUAL(tp.second.e(), Point3d(2, 2, 0.36f));
	BOOST_CHECK_EQUAL(tp.second.r(), Point3d(0, 4, 0));
}

BOOST_FIXTURE_TEST_CASE(HeightMapTest5, Test4x4HeightMap)
{
	HeightMap hm;
	hm.Swap(sample4x4);

	Triangle3dPair tp = utils::GetTriangles(hm);

	BOOST_CHECK_EQUAL(tp.first, Triangle3d(Point3d(0, 0, 0), Point3d(0, 3, 0), Point3d(3, 3, 0)));
	BOOST_CHECK_EQUAL(tp.second, Triangle3d(Point3d(3, 3, 0), Point3d(3, 0, 0), Point3d(0, 0, 0)));
}

BOOST_AUTO_TEST_CASE(HeightMapLoaderCacheTest1)
{
	HeightMapLoaderPtr hmlPtr = std::make_shared<HeightMapLoaderConstant>(5, 1.0f, 2.0f);
	HeightMapLoaderCache c(2u, hmlPtr);

	HeightMap hm;
	c.Get(Point2d(0, 0), 2, hm);

	BOOST_CHECK_EQUAL(hm.At(Point2d(0, 0)), 1.0f);
}

BOOST_AUTO_TEST_CASE(HeightMapLoaderCacheTest2)
{
	HeightMapLoaderPtr hmlPtr = std::make_shared<HeightMapLoaderConstant>(5, 1.0f, 2.0f);
	HeightMapLoaderCache c(2u, hmlPtr);

	HeightMap hm;
	c.Get(Point2d(4, 4), 1, hm);

	BOOST_CHECK_EQUAL(hm.At(Point2d(0, 0)), 2.0f);
}

BOOST_AUTO_TEST_CASE(HeightMapLoaderCacheTest3)
{
	HeightMapLoaderPtr hmlPtr = std::make_shared<HeightMapLoaderConstant>(5, 1.0f, 2.0f);
	HeightMapLoaderCache c(2u, hmlPtr);

	HeightMapLoaderPtr hmlPtr2 = std::make_shared<HeightMapLoaderConstant>(5, 3.0f, 4.0f);
	HeightMapLoaderCache c2(2u, hmlPtr2);

	HeightMap hm2;
	c2.Get(Point2d(0, 0), 2, hm2);

	BOOST_CHECK_EQUAL(hm2.At(Point2d(0, 0)), 3.0f);

	c.Set(Point2d(0, 0), hm2);

	HeightMap hm1;
	c.Get(Point2d(0, 0), 2, hm1);

	BOOST_CHECK_EQUAL(hm1.At(Point2d(0, 0)), 3.0f);

	HeightMap hm3;
	c.Get(Point2d(4, 4), 1, hm3);

	BOOST_CHECK_EQUAL(hm3.At(Point2d(0, 0)), 2.0f);

	c.Get(Point2d(0, 0), 1, hm3);
	
	BOOST_CHECK_EQUAL(hm3.At(Point2d(0, 0)), 3.0f);
}

BOOST_AUTO_TEST_CASE(HeightMapLoaderCacheTest4)
{
	HeightMapLoaderPtr hmlPtr = std::make_shared<HeightMapLoaderConstant>(3, 1.0f, 2.0f);
	HeightMapLoaderCache c(3u, hmlPtr);

	HeightMap hm;
	c.Get(Point2d(0, 0), 3, hm);

	BOOST_CHECK_EQUAL(hm.At(Point2d(0, 0)), 1.0f);

	HeightMap::Container cont(9, 3.0);
	hm.Swap(cont);

	c.Set(Point2d(0, 0), hm);
	c.Get(Point2d(0, 0), 1, hm);

	BOOST_CHECK_EQUAL(hm.At(Point2d(0, 0)), 3.0f);

	BOOST_REQUIRE_THROW(hm.At(Point2d(2, 2)), std::out_of_range);
	BOOST_REQUIRE_THROW(hm.At(Point2d(3, 3)), std::out_of_range);
	BOOST_REQUIRE_THROW(c.Get(Point2d(3, 3), 1, hm), std::runtime_error);

	BOOST_REQUIRE_THROW(c.Get(Point2d(0, 0), 2, hm), std::runtime_error);
	BOOST_REQUIRE_THROW(c.Set(Point2d(0, 0), hm), std::runtime_error);
	
	c.Get(Point2d(2, 2), 1, hm);

	BOOST_CHECK_EQUAL(hm.At(Point2d(0, 0)), 3.0f);

	c.Get(Point2d(0, 0), 3, hm);

	BOOST_CHECK_EQUAL(hm.At(Point2d(2, 2)), 3.0f);
}

BOOST_AUTO_TEST_CASE(HeightMapLoaderCacheTest5)
{
	HeightMapLoaderPtr hmlPtr = std::make_shared<HeightMapLoaderConstant>(5, 1.0f, 2.0f);
	HeightMapLoaderCache c(3u, hmlPtr);

	HeightMap hm;
	c.Get(Point2d(0, 0), 3, hm);

	BOOST_CHECK_EQUAL(hm.At(Point2d(0, 0)), 1.0f);

	HeightMap::Container cont(9, 3.0);
	hm.Swap(cont);

	c.Set(Point2d(0, 0), hm);
	c.Get(Point2d(0, 0), 1, hm);

	BOOST_CHECK_EQUAL(hm.At(Point2d(0, 0)), 3.0f);

	c.Get(Point2d(3, 3), 1, hm);

	BOOST_CHECK_EQUAL(hm.At(Point2d(0, 0)), 2.0f);
}

BOOST_AUTO_TEST_CASE(HeightMapLoaderCacheTest6)
{
	HeightMapLoaderPtr hmlPtr = std::make_shared<HeightMapLoaderConstant>(5, 1.0f, 1.0f, 1.0f);
	HeightMapLoaderCache c(3u, hmlPtr);

	HeightMap hm;

	c.Get(Point2d(0, 0), 1, hm);
	BOOST_CHECK_EQUAL(hm.At(Point2d(0, 0)), 1.0f);

	c.Get(Point2d(1, 0), 1, hm);
	BOOST_CHECK_EQUAL(hm.At(Point2d(0, 0)), 2.0f);

	c.Get(Point2d(2, 2), 1, hm);
	BOOST_CHECK_EQUAL(hm.At(Point2d(0, 0)), 13.0f);
}

BOOST_AUTO_TEST_CASE(HeightMapLoaderCacheTest7)
{
	HeightMapLoaderPtr hmlPtr = std::make_shared<HeightMapLoaderConstant>(5, 1.0f, 1.0f);
	HeightMapLoaderCache c(3u, hmlPtr);

	HeightMap hm;

	c.Get(Point2d(2, 2), 1, hm);
	BOOST_CHECK_EQUAL(hm.At(Point2d(0, 0)), 1.0);

	HeightMapLoaderPtr hmlPtr1 = std::make_shared<HeightMapLoaderConstant>(5, 1.0f, 1.0f, 1.0f);
	HeightMapLoaderCache c1(3u, hmlPtr1);

	c1.Get(Point2d(2, 2), 3, hm);
	BOOST_CHECK_EQUAL(hm.At(Point2d(2, 2)), 13.0f);

	c.Set(Point2d(0, 0), hm);

	c.Get(Point2d(2, 2), 1, hm);
	BOOST_CHECK_EQUAL(hm.At(Point2d(0, 0)), 13.0f);
}