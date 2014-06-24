//#define BOOST_TEST_DYN_LINK

#include <HeightMap.h>
#include <HeightMapExtended.h>
#include <GeometryUtils.h>
#include <Triangle3d.h>
#include <Point2d.h>
#include <boost/test/unit_test.hpp>

using namespace trm::terrain;
using namespace trm;

namespace
{
	const HeightMap::Type v4x4[] = 
	{
		0, 1, 0, 0,
		0, 1, 0, 0,
		0, 0, 0, 0,
		0, 0, 0, 0
	};
}

BOOST_AUTO_TEST_CASE(HeightMapTest1)
{
	HeightMap hm;
	hm.Load(v4x4);
	BOOST_CHECK_EQUAL(hm.At(Point2d(1, 1)), 0.0f);
	BOOST_CHECK_EQUAL(hm.At(Point2d(1, 2)), 1.0f);
	BOOST_CHECK_EQUAL(hm.At(Point2d(1, 3)), 1.0f);
	BOOST_CHECK_EQUAL(hm.GetSize(), 4u);
}

BOOST_AUTO_TEST_CASE(HeightMapTest2)
{
	const HeightMap::Type v[] = 
	{
		0, 0, 0, 0,
		0, 0, 0, 0
	};
	
	HeightMap hm;
	BOOST_CHECK_THROW (hm.Load(v), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(HeightMapTest3)
{
	HeightMapExtended hme(4);
	hme.Load(v4x4);
	BOOST_CHECK_EQUAL(hme.At(Point2d(1, 2)), 1.0f);
	BOOST_CHECK_EQUAL(hme.At(Point2d(1, 3)), 1.0f);
}

BOOST_AUTO_TEST_CASE(HeightMapTest4)
{
	HeightMapExtended hme(16);
	hme.Load(v4x4);
	BOOST_CHECK_EQUAL(hme.At(Point2d(2, 5)), 0.125f);
}

BOOST_AUTO_TEST_CASE(LODUtilsTest1)
{
	HeightMapExtended hme(5);
	hme.Load(v4x4);

	Triangle3d t(Point3d(0, 0, 0), Point3d(0, 4, 0), Point3d(4, 4, 0));
	Triangle3dPair tp = utils::SplitTriangle(t, hme);

	BOOST_CHECK_EQUAL(tp.first.l(), Point3d(0, 4, 0));
	BOOST_CHECK_EQUAL(tp.first.e(), Point3d(2, 2, 0.36f));
	BOOST_CHECK_EQUAL(tp.first.r(), Point3d(0, 0, 0));
	BOOST_CHECK_EQUAL(tp.second.l(), Point3d(4, 4, 0));
	BOOST_CHECK_EQUAL(tp.second.e(), Point3d(2, 2, 0.36f));
	BOOST_CHECK_EQUAL(tp.second.r(), Point3d(0, 4, 0));
}

BOOST_AUTO_TEST_CASE(HeightMapTest5)
{
	HeightMap hm;
	hm.Load(v4x4);

	Triangle3dPair tp = utils::GetTriangles(hm);

	BOOST_CHECK_EQUAL(tp.first, Triangle3d(Point3d(0, 0, 0), Point3d(0, 3, 0), Point3d(3, 3, 0)));
	BOOST_CHECK_EQUAL(tp.second, Triangle3d(Point3d(3, 3, 0), Point3d(3, 0, 0), Point3d(0, 0, 0)));
}
