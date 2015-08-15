#include <boost/test/unit_test.hpp>

#include <RailRoadArc.h>
#include <RailRoadLine.h>
#include <CommonUtils.h>
#include <RailRoadClosestPoint.h>
#include <Point2d.h>

#include <boost/math/constants/constants.hpp>

using namespace trm;

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest1)
{
	RailRoadLine rrl(Point3d(0, 0, 1), Point3d(1, 0, 0));

	RailRoadClosestPoint rrcp1(Point2d(0, 0), true, 0.0001f);
	rrl.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point3d(0, 0, 1));

	RailRoadClosestPoint rrcp2(Point2d(1, 0), true, 0.0001f);
	rrl.Accept(rrcp2);

	BOOST_CHECK_EQUAL(rrcp2.GetPoint(), Point3d(1, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest2)
{
	RailRoadLine rrl(Point3d(0, 0, 1), Point3d(1, 0, 0));

	RailRoadClosestPoint rrcp1(Point2d(-1, 0), true, 0.0001f);
	rrl.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point3d(0, 0, 1));

	RailRoadClosestPoint rrcp2(Point2d(2, 0), true, 0.0001f);
	rrl.Accept(rrcp2);

	BOOST_CHECK_EQUAL(rrcp2.GetPoint(), Point3d(1, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest21)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(1, 0, 1));

	RailRoadClosestPoint rrcp1(Point2d(-std::numeric_limits<float>::min(), 0), true, 0.0001f);
	rrl.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point3d(0, 0, 0));

	RailRoadClosestPoint rrcp2(Point2d(1 + std::numeric_limits<float>::min(), 0), true, 0.0001f);
	rrl.Accept(rrcp2);

	BOOST_CHECK_EQUAL(rrcp2.GetPoint(), Point3d(1, 0, 1));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest22)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(1, 0, 1));

	RailRoadClosestPoint rrcp1(Point2d(std::numeric_limits<float>::min(), 0), true, 0.0001f);
	rrl.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point3d(0, 0, 0));

	RailRoadClosestPoint rrcp2(Point2d(1 - std::numeric_limits<float>::min(), 0), true, 0.0001f);
	rrl.Accept(rrcp2);

	BOOST_CHECK_EQUAL(rrcp2.GetPoint(), Point3d(1, 0, 1));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest3)
{
	RailRoadLine rrl(Point3d(0, 0, 1), Point3d(1, 0, 0));

	RailRoadClosestPoint rrcp1(Point2d(-1, 0), false, 0.0001f);
	rrl.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point3d(-1, 0, 2));

	RailRoadClosestPoint rrcp2(Point2d(2, 0), false, 0.0001f);
	rrl.Accept(rrcp2);

	BOOST_CHECK_EQUAL(rrcp2.GetPoint(), Point3d(2, 0, -1));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest31)
{
	RailRoadLine rrl(Point3d(0, 0, 1), Point3d(1, 0, 0));

	RailRoadClosestPoint rrcp1(Point2d(0.1f, 0.0f), false, 0.0001f);
	rrl.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point3d(0.1f, 0.0f, 0.9f));

	RailRoadClosestPoint rrcp2(Point2d(0.9f, 0.0f), false, 0.0001f);
	rrl.Accept(rrcp2);
	 
	BOOST_CHECK_EQUAL(rrcp2.GetPoint(), Point3d(0.9f, 0.0f, 0.1f));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest32)
{
	RailRoadLine rrl(Point3d(0, 0, -1), Point3d(1, 0, 0));

	RailRoadClosestPoint rrcp1(Point2d(0.1f, 0.0f), false, 0.0001f);
	rrl.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point3d(0.1f, 0.0f, -0.9f));

	RailRoadClosestPoint rrcp2(Point2d(0.9f, 0.0f), false, 0.0001f);
	rrl.Accept(rrcp2);

	BOOST_CHECK_EQUAL(rrcp2.GetPoint(), Point3d(0.9f, 0.0f, -0.1f));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest4)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(1, 0, 1));

	RailRoadClosestPoint rrcp1(Point2d(-1, 1), true, 0.0001f);
	rrl.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point3d(0, 0, 0));

	RailRoadClosestPoint rrcp2(Point2d(2, -1), true, 0.0001f);
	rrl.Accept(rrcp2);

	BOOST_CHECK_EQUAL(rrcp2.GetPoint(), Point3d(1, 0, 1));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest5)
{
	RailRoadArc rra(Point3d(10, 0, 0), Degrees(360), Point2d(0, 0), Rotation::Clockwise);

	RailRoadClosestPoint rrcp1(Point2d(0, 0), true, 0.0001f);
	rra.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point3d(10, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest6)
{
	RailRoadArc rra(Point3d(10, 0, 0), Degrees(360), Point2d(0, 0), Rotation::Clockwise);

	RailRoadClosestPoint rrcp1(Point2d(10, 0), true, 0.0001f);
	rra.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point3d(10, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest7)
{
	RailRoadArc rra(Point3d(10, 0, 0), Degrees(360), Point2d(0, 0), Rotation::Clockwise);

	RailRoadClosestPoint rrcp1(Point2d(5, 0), true, 0.0001f);
	rra.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point3d(10, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest8)
{
	RailRoadArc rra(Point3d(10, 10, 0), Degrees(360), Point2d(0, 10), Rotation::Clockwise);

	RailRoadClosestPoint rrcp1(Point2d(5, 10), true, 0.0001f);
	rra.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point3d(10, 10, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest9)
{
	RailRoadArc rra(Point3d(10, 10, 0), Degrees(360), Point2d(0, 10), Rotation::Clockwise);

	RailRoadClosestPoint rrcp1(Point2d(15, 10), true, 0.0001f);
	rra.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point3d(10, 10, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest10)
{
	RailRoadArc rra(Point3d(10, 10, 0), Degrees(90), Point2d(0, 10), Rotation::Clockwise);

	RailRoadClosestPoint rrcp1(Point2d(20, 10), true, 0.0001f);
	rra.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point3d(10, 10, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest11)
{
	RailRoadArc rra(Point3d(10, 10, 0), Degrees(90), Point2d(0, 10), Rotation::Clockwise);

	RailRoadClosestPoint rrcp1(Point2d(20, 10), false, 0.0001f);
	rra.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point3d(10, 10, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest12)
{
	RailRoadLine rrl(Point3d(10, 10, 5), Point3d(15, 10, 10));

	RailRoadClosestPoint rrcp1(Point2d(14, 10), false, 0.0001f);
	rrl.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point3d(14, 10, 9));
}

// RailRoadPointer

BOOST_AUTO_TEST_CASE(RailRoadPointerTest4)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 2, 2));

	RailRoadClosestPoint rrp1(Point2d(1, 1), false, 0.0001f);
	rrl.Accept(rrp1);

	BOOST_CHECK_EQUAL(rrp1.GetPoint(), Point3d(1, 1, 1));

	RailRoadClosestPoint rrp2(Point2d(1, 1), true, 0.0001f);
	rrl.Accept(rrp2);

	BOOST_CHECK_EQUAL(rrp2.GetPoint(), Point3d(1, 1, 1));
}

BOOST_AUTO_TEST_CASE(RailRoadPointerTest5)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 2, 2));

	RailRoadClosestPoint rrp1(Point2d(3, 3), false, 0.0001f);
	rrl.Accept(rrp1);

	BOOST_CHECK_EQUAL(rrp1.GetPoint(), Point3d(3, 3, 3));

	RailRoadClosestPoint rrp2(Point2d(3, 3), true, 0.0001f);
	rrl.Accept(rrp2);

	BOOST_CHECK_EQUAL(rrp2.GetPoint(), Point3d(2, 2, 2));
}

BOOST_AUTO_TEST_CASE(RailRoadPointerTest6)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 2, 2));

	RailRoadClosestPoint rrp1(Point2d(-1, -1), false, 0.0001f);
	rrl.Accept(rrp1);

	BOOST_CHECK_EQUAL(rrp1.GetPoint(), Point3d(-1, -1, -1));

	RailRoadClosestPoint rrp2(Point2d(-1, -1), true, 0.0001f);
	rrl.Accept(rrp2);

	BOOST_CHECK_EQUAL(rrp2.GetPoint(), Point3d(0, 0, 0));
}
