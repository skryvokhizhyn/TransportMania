//#define BOOST_TEST_DYN_LINK

#include <Point3d.h>
#include <Point4d.h>
#include <Point2d.h>
#include <CommonUtils.h>
#include <PointHash.h>

#include <boost/test/unit_test.hpp>

using namespace trm;
using namespace utils;

BOOST_AUTO_TEST_CASE(PointTest1)
{
	Point3d v1(1.0, 2.0, 3.0);

	BOOST_CHECK(CheckEqual(v1.x(), 1.0f));
	BOOST_CHECK(CheckEqual(v1.y(), 2.0f));
	BOOST_CHECK(CheckEqual(v1.z(), 3.0f));
}

BOOST_AUTO_TEST_CASE(PointTest2)
{
	Point3d v1(1, 2, 3);
	Point3d v2(v1);

	BOOST_CHECK(CheckEqual(v2.x(), 1.0f));
	BOOST_CHECK(CheckEqual(v2.y(), 2.0f));
	BOOST_CHECK(CheckEqual(v2.z(), 3.0f));
}

BOOST_AUTO_TEST_CASE(PointTest3)
{
	Point3d v1(1, 2, 3);
	Point3d v2 = v1;

	BOOST_CHECK(CheckEqual(v2.x(), 1.0f));
	BOOST_CHECK(CheckEqual(v2.y(), 2.0f));
	BOOST_CHECK(CheckEqual(v2.z(), 3.0f));
}

BOOST_AUTO_TEST_CASE(PointTest4)
{
	Point3d v1(1, 2, 3);
	Point3d v2 = v1;

	BOOST_CHECK_EQUAL(v1, v2);
}

BOOST_AUTO_TEST_CASE(PointTest5)
{
	Point3d v1(1, 2, 3);
	Point3d v2(2, 3, 4);

	BOOST_CHECK_NE(v1, v2);
}


BOOST_AUTO_TEST_CASE(PointTest6)
{
	Point4d p4(1, 2, 3, 4);
	Point2d p2 = Point2d::Cast(p4);

	BOOST_CHECK_EQUAL(p2, Point2d(1, 2));
}

BOOST_AUTO_TEST_CASE(PointTest7)
{
	Point2d p2(1, 2);
	Point4d p4 = Point4d::Cast(p2);

	BOOST_CHECK_EQUAL(p4, Point4d(1, 2, 0, 0));
}


BOOST_AUTO_TEST_CASE(PointHashTest1)
{
	BOOST_CHECK_NE(hash_value(Point3d(0, 1, 0)), hash_value(Point3d(1, 0, 0)));
	BOOST_CHECK_NE(hash_value(Point3d(0, 1, 0)), hash_value(Point3d(0, 0, 1)));
	BOOST_CHECK_EQUAL(hash_value(Point3d(0, 1, 0)), hash_value(Point3d(0, 1, 0)));
	BOOST_CHECK_NE(hash_value(Point3d(0, 0, 0)), hash_value(Point3d(0, 0, 1)));
}
