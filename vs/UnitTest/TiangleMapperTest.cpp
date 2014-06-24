#include <boost/test/unit_test.hpp>

#include <TriangleMapper.h>
#include <Triangle2d.h>

using namespace trm;

namespace
{
	TriangleMapper TM9U(Triangle2d(Point2d(0, 0), Point2d(0, 8), Point2d(8, 8)));
	TriangleMapper TM17U(Triangle2d(Point2d(0, 0), Point2d(0, 16), Point2d(16, 16)));
}

BOOST_AUTO_TEST_CASE(TriangleMapperTest0)
{
	BOOST_REQUIRE_THROW(TM9U.GetTriangleByNumber(0), std::logic_error);
}

BOOST_AUTO_TEST_CASE(TriangleMapperTest1)
{
	BOOST_CHECK_EQUAL(TM9U.GetTriangleByNumber(1), Triangle2d(Point2d(0, 0), Point2d(0, 4), Point2d(4, 4)));
}

BOOST_AUTO_TEST_CASE(TriangleMapperTest2)
{
	BOOST_CHECK_EQUAL(TM9U.GetTriangleByNumber(2), Triangle2d(Point2d(0, 4), Point2d(2, 2), Point2d(0, 0)));
}

BOOST_AUTO_TEST_CASE(TriangleMapperTest3)
{
	BOOST_CHECK_EQUAL(TM9U.GetTriangleByNumber(3), Triangle2d(Point2d(4, 4), Point2d(2, 2), Point2d(0, 4)));
}

BOOST_AUTO_TEST_CASE(TriangleMapperTest4)
{
	BOOST_CHECK_EQUAL(TM17U.GetTriangleByNumber(3), Triangle2d(Point2d(16, 16), Point2d(8, 8), Point2d(0, 16)));
}

BOOST_AUTO_TEST_CASE(TriangleMapperTest5)
{
	BOOST_CHECK_EQUAL(TM17U.GetTriangleByNumber(7), Triangle2d(Point2d(0, 16), Point2d(8, 16), Point2d(8, 8)));
}

BOOST_AUTO_TEST_CASE(TriangleMapperTest6)
{
	BOOST_CHECK_EQUAL(TM17U.GetTriangleByNumber(5), Triangle2d(Point2d(0, 0), Point2d(0, 8), Point2d(8, 8)));
}

BOOST_AUTO_TEST_CASE(TriangleMapperTest7)
{
	TriangleMapper TM3D(Triangle2d(Point2d(2, 2), Point2d(2, 0), Point2d(0, 0)));

	BOOST_CHECK_EQUAL(TM3D.GetTriangleByNumber(7), Triangle2d(Point2d(2, 0), Point2d(1, 0), Point2d(1, 1)));
}
