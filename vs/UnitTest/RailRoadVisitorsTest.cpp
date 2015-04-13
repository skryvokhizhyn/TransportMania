#include <boost/test/unit_test.hpp>

#include <RailRoadSizer.h>
#include <RailRoadParametersTaker.h>
#include <RailRoadArc.h>
#include <RailRoadLine.h>
#include <CommonUtils.h>
#include <RailRoadClosestPoint.h>
#include <Point2d.h>

#include <boost/math/constants/constants.hpp>

using namespace trm;

BOOST_AUTO_TEST_CASE(RailRoadSizerTest1)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(1, 0, 0));

	RailRoadSizer rrs;
	rrl.Accept(rrs);

	BOOST_CHECK_EQUAL(rrs.GetLenght(), 1.0f);
}

BOOST_AUTO_TEST_CASE(RailRoadSizerTest2)
{
	RailRoadArc rra(Point3d(0, 0, 0), Degrees(360), Point2d(10, 0), Rotation::AntiClockwise);

	RailRoadSizer rrs;
	rra.Accept(rrs);

	BOOST_CHECK_EQUAL(rrs.GetLenght(), 2 * 10.0f * boost::math::constants::pi<float>());
}

BOOST_AUTO_TEST_CASE(RailRoadSizerTest3)
{
	RailRoadArc rra(Point3d(0, 0, 0), Degrees(180), Point2d(0, 10), Rotation::AntiClockwise);

	RailRoadSizer rrs;
	rra.Accept(rrs);

	BOOST_CHECK_EQUAL(rrs.GetLenght(), 10.0f * boost::math::constants::pi<float>());
}

BOOST_AUTO_TEST_CASE(RailRoadParametersTakerTest1)
{
	RailRoadArc rra(Point3d(0, 0, 0), Degrees(180), Point2d(0, 10), Rotation::AntiClockwise);

	RailRoadParametersTaker rrpt;
	rra.Accept(rrpt);

	BOOST_CHECK_EQUAL(rrpt.GetStart(), Point3d(0, 0, 0));

	const Point3d & p = rrpt.GetEnd();

	BOOST_CHECK_PREDICATE(std::less_equal<float>(), (p.x())(0.00001f));
	BOOST_CHECK(utils::CheckEqual(p.y(), 20.0f));
	BOOST_CHECK(utils::CheckEqual(p.z(), 0.0f));
}

BOOST_AUTO_TEST_CASE(RailRoadParametersTakerTest2)
{
	RailRoadArc rra(Point3d(0, 0, 0), Degrees(180), Point2d(0, 10), Rotation::Clockwise);

	RailRoadParametersTaker rrpt;
	rra.Accept(rrpt);

	BOOST_CHECK_EQUAL(rrpt.GetStart(), Point3d(0, 0, 0));

	const Point3d & p = rrpt.GetEnd();

	BOOST_CHECK_PREDICATE(std::less_equal<float>(), (p.x())(0.00001f));
	BOOST_CHECK(utils::CheckEqual(p.y(), 20.0f));
	BOOST_CHECK(utils::CheckEqual(p.z(), 0.0f));
}

BOOST_AUTO_TEST_CASE(RailRoadParametersTakerTest3)
{
	RailRoadArc rra(Point3d(0, 0, 0), Degrees(90), Point2d(0, 10), Rotation::Clockwise);

	RailRoadParametersTaker rrpt;
	rra.Accept(rrpt);

	BOOST_CHECK_EQUAL(rrpt.GetStart(), Point3d(0, 0, 0));
	BOOST_CHECK_EQUAL(rrpt.GetEnd(), Point3d(-10, 10, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadParametersTakerTest4)
{
	RailRoadArc rra(Point3d(0, 0, 0), Degrees(90), Point2d(0, 10), Rotation::AntiClockwise);

	RailRoadParametersTaker rrpt;
	rra.Accept(rrpt);

	BOOST_CHECK_EQUAL(rrpt.GetStart(), Point3d(0, 0, 0));
	BOOST_CHECK_EQUAL(rrpt.GetEnd(), Point3d(10, 10, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest1)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(1, 0, 0));

	RailRoadClosestPoint rrcp1(Point2d(0, 0), true);
	rrl.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point2d(0, 0));

	RailRoadClosestPoint rrcp2(Point2d(1, 0), true);
	rrl.Accept(rrcp2);

	BOOST_CHECK_EQUAL(rrcp2.GetPoint(), Point2d(1, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest2)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(1, 0, 0));

	RailRoadClosestPoint rrcp1(Point2d(-1, 0), true);
	rrl.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point2d(0, 0));

	RailRoadClosestPoint rrcp2(Point2d(2, 0), true);
	rrl.Accept(rrcp2);

	BOOST_CHECK_EQUAL(rrcp2.GetPoint(), Point2d(1, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest21)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(1, 0, 0));

	RailRoadClosestPoint rrcp1(Point2d(-std::numeric_limits<float>::min(), 0), true);
	rrl.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point2d(0, 0));

	RailRoadClosestPoint rrcp2(Point2d(1 + std::numeric_limits<float>::min(), 0), true);
	rrl.Accept(rrcp2);

	BOOST_CHECK_EQUAL(rrcp2.GetPoint(), Point2d(1, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest22)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(1, 0, 0));

	RailRoadClosestPoint rrcp1(Point2d(std::numeric_limits<float>::min(), 0), true);
	rrl.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point2d(0, 0));

	RailRoadClosestPoint rrcp2(Point2d(1 - std::numeric_limits<float>::min(), 0), true);
	rrl.Accept(rrcp2);

	BOOST_CHECK_EQUAL(rrcp2.GetPoint(), Point2d(1, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest3)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(1, 0, 0));

	RailRoadClosestPoint rrcp1(Point2d(-1, 0), false);
	rrl.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point2d(-1, 0));

	RailRoadClosestPoint rrcp2(Point2d(2, 0), false);
	rrl.Accept(rrcp2);

	BOOST_CHECK_EQUAL(rrcp2.GetPoint(), Point2d(2, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest4)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(1, 0, 0));

	RailRoadClosestPoint rrcp1(Point2d(-1, 1), true);
	rrl.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point2d(0, 0));

	RailRoadClosestPoint rrcp2(Point2d(2, -1), true);
	rrl.Accept(rrcp2);

	BOOST_CHECK_EQUAL(rrcp2.GetPoint(), Point2d(1, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest5)
{
	RailRoadArc rra(Point3d(10, 0, 0), Degrees(360), Point2d(0, 0), Rotation::Clockwise);

	RailRoadClosestPoint rrcp1(Point2d(0, 0), true);
	rra.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point2d(10, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest6)
{
	RailRoadArc rra(Point3d(10, 0, 0), Degrees(360), Point2d(0, 0), Rotation::Clockwise);

	RailRoadClosestPoint rrcp1(Point2d(10, 0), true);
	rra.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point2d(10, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest7)
{
	RailRoadArc rra(Point3d(10, 0, 0), Degrees(360), Point2d(0, 0), Rotation::Clockwise);

	RailRoadClosestPoint rrcp1(Point2d(5, 0), true);
	rra.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point2d(10, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest8)
{
	RailRoadArc rra(Point3d(10, 10, 0), Degrees(360), Point2d(0, 10), Rotation::Clockwise);

	RailRoadClosestPoint rrcp1(Point2d(5, 10), true);
	rra.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point2d(10, 10));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest9)
{
	RailRoadArc rra(Point3d(10, 10, 0), Degrees(360), Point2d(0, 10), Rotation::Clockwise);

	RailRoadClosestPoint rrcp1(Point2d(15, 10), true);
	rra.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point2d(10, 10));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest10)
{
	RailRoadArc rra(Point3d(10, 10, 0), Degrees(90), Point2d(0, 10), Rotation::Clockwise);

	RailRoadClosestPoint rrcp1(Point2d(20, 10), true);
	rra.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point2d(10, 10));
}

BOOST_AUTO_TEST_CASE(RailRoadClosestPointTest11)
{
	RailRoadArc rra(Point3d(10, 10, 0), Degrees(90), Point2d(0, 10), Rotation::Clockwise);

	RailRoadClosestPoint rrcp1(Point2d(20, 10), false);
	rra.Accept(rrcp1);

	BOOST_CHECK_EQUAL(rrcp1.GetPoint(), Point2d(10, 10));
}
