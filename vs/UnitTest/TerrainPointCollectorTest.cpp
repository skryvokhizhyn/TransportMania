#include <boost/test/unit_test.hpp>

#include <TerrainPointCollector.h>
#include <TerrainCoverDrawableObject.h>

#include <boost/assign/list_of.hpp>
#include <boost/assign/std/map.hpp>
#include <boost/range/algorithm/copy.hpp>

using namespace trm;
using namespace boost::assign;

namespace std
{
	std::ostream & operator << (std::ostream & o, const IndexVector & iv)
	{
		o << "(";
		boost::copy(iv, std::ostream_iterator<int>(o, ","));
		o << ")";

		return o;
	}
}

BOOST_AUTO_TEST_CASE(TerrainPointCollectorTest1)
{
	TerrainCoverDrawableObject::PointMap pm;
	boost::assign::insert(pm.left)
		(Point2d(0, 0), 0) (Point2d(1, 0), 1);

	auto ids = TerrainCoverDrawableObject::GetTriangulationIndexes(pm);
	BOOST_CHECK_EQUAL(ids.size(), 0u);
}

BOOST_AUTO_TEST_CASE(TerrainPointCollectorTest2)
{
	TerrainCoverDrawableObject::PointMap pm;
	boost::assign::insert(pm.left)
		(Point2d(0, 0), 0) (Point2d(1, 0), 1)
		(Point2d(0, 1), 2);

	auto ids = TerrainCoverDrawableObject::GetTriangulationIndexes(pm);
	BOOST_CHECK_EQUAL(ids.size(), 3u);
	BOOST_CHECK_EQUAL(ids, IndexVector({2, 0, 1}));
}

BOOST_AUTO_TEST_CASE(TerrainPointCollectorTest3)
{
	TerrainCoverDrawableObject::PointMap pm;
	boost::assign::insert(pm.left)
							(Point2d(1, 1), 2)
		(Point2d(0, 0), 0)	(Point2d(1, 0), 1);

	auto ids = TerrainCoverDrawableObject::GetTriangulationIndexes(pm);
	BOOST_CHECK_EQUAL(ids.size(), 3u);
	BOOST_CHECK_EQUAL(ids, IndexVector({0, 1, 2}));
}

BOOST_AUTO_TEST_CASE(TerrainPointCollectorTest4)
{
	TerrainCoverDrawableObject::PointMap pm;
	boost::assign::insert(pm.left)
		(Point2d(0, 1), 1) (Point2d(1, 1), 2)
		(Point2d(0, 0), 0);

	auto ids = TerrainCoverDrawableObject::GetTriangulationIndexes(pm);
	BOOST_CHECK_EQUAL(ids.size(), 3u);
	BOOST_CHECK_EQUAL(ids, IndexVector({0, 2, 1}));
}

BOOST_AUTO_TEST_CASE(TerrainPointCollectorTest5)
{
	TerrainCoverDrawableObject::PointMap pm;
	boost::assign::insert(pm.left)
		(Point2d(0, 1), 2) (Point2d(1, 1), 3)
		(Point2d(0, 0), 0) (Point2d(1, 0), 1);

	auto ids = TerrainCoverDrawableObject::GetTriangulationIndexes(pm);
	BOOST_CHECK_EQUAL(ids.size(), 6);
	BOOST_CHECK_EQUAL(ids, IndexVector({0, 3, 2,  0, 1, 3}));
}

BOOST_AUTO_TEST_CASE(TerrainPointCollectorTest6)
{
	TerrainCoverDrawableObject::PointMap pm;
	boost::assign::insert(pm.left)
		(Point2d(0, 2), 4) (Point2d(1, 2), 5)
		(Point2d(0, 1), 2) (Point2d(1, 1), 3)
		(Point2d(0, 0), 0) (Point2d(1, 0), 1);

	auto ids = TerrainCoverDrawableObject::GetTriangulationIndexes(pm);
	BOOST_CHECK_EQUAL(ids.size(), 12);
	BOOST_CHECK_EQUAL(ids, IndexVector({0, 3, 2,  0, 1, 3,  2, 5, 4,  2, 3, 5}));
}

BOOST_AUTO_TEST_CASE(TerrainPointCollectorTest7)
{
	TerrainCoverDrawableObject::PointMap pm;

	auto ids = TerrainCoverDrawableObject::GetTriangulationIndexes(pm);
	BOOST_CHECK_EQUAL(ids.size(), 0);
}

BOOST_AUTO_TEST_CASE(TerrainDraftRoadGeneratorTest8)
{
	TerrainCoverDrawableObject::PointMap pm;
	boost::assign::insert(pm.left)
		(Point2d(0, 1), 1) (Point2d(1, 1), 2)	(Point2d(2, 1), 3) (Point2d(3, 1), 4)
		(Point2d(0, 0), 0)						(Point2d(2, 0), 5);

	auto ids = TerrainCoverDrawableObject::GetTriangulationIndexes(pm);
	BOOST_CHECK_EQUAL(ids.size(), 9);
	BOOST_CHECK_EQUAL(ids, IndexVector({0, 2, 1,  2, 5, 3,  5, 4, 3}));
}

BOOST_AUTO_TEST_CASE(TerrainPointCollectorTest9)
{
	TerrainCoverDrawableObject::PointMap pm;
	boost::assign::insert(pm.left)
		(Point2d(0, 1), 1) (Point2d(1, 1), 2)	(Point2d(2, 1), 3)	(Point2d(3, 1), 4)
		(Point2d(0, 0), 0)											(Point2d(3, 0), 5);

	auto ids = TerrainCoverDrawableObject::GetTriangulationIndexes(pm);
	BOOST_CHECK_EQUAL(ids.size(), 6);
	BOOST_CHECK_EQUAL(ids, IndexVector({0, 2, 1,  3, 5, 4}));
}

BOOST_AUTO_TEST_CASE(TerrainPointCollectorTest10)
{
	TerrainCoverDrawableObject::PointMap pm;
	boost::assign::insert(pm.left)
		(Point2d(0, 1), 1) (Point2d(1, 1), 2)	(Point2d(3, 1), 3)	(Point2d(4, 1), 4)
		(Point2d(0, 0), 0)											(Point2d(4, 0), 5);

	auto ids = TerrainCoverDrawableObject::GetTriangulationIndexes(pm);
	BOOST_CHECK_EQUAL(ids.size(), 6);
	BOOST_CHECK_EQUAL(ids, IndexVector({0, 2, 1,  3, 5, 4}));
}

BOOST_AUTO_TEST_CASE(TerrainPointCollectorTest11)
{
	TerrainCoverDrawableObject::PointMap pm;
	boost::assign::insert(pm.left)
		(Point2d(0, 1), 1)	(Point2d(1, 1), 2)	(Point2d(2, 1), 3)	(Point2d(3, 1), 4)
							(Point2d(1, 0), 0)	(Point2d(2, 0), 5);

	auto ids = TerrainCoverDrawableObject::GetTriangulationIndexes(pm);
	BOOST_CHECK_EQUAL(ids.size(), 12);
	BOOST_CHECK_EQUAL(ids, IndexVector({1, 0, 2,  0, 3, 2,  0, 5, 3,  5, 4, 3}));
}