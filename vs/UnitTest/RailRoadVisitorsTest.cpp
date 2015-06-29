#include <boost/test/unit_test.hpp>

#include <RailRoadSizer.h>
#include <RailRoadParametersTaker.h>
#include <RailRoadArc.h>
#include <RailRoadLine.h>
#include <CommonUtils.h>
#include <RailRoadClosestPoint.h>
#include <RailRoadTangent.h>
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

// RailRoadClosestPoint

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

// RailRoadTangent

namespace trm
{
	std::ostream & operator << (std::ostream & o, const OptionalPoint3d & p)
	{
		if (p)
		{
			o << p.get();
		}
		else
		{
			o << "None";
		}

		return o;
	}
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest21)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(1, 0, 0));

	RailRoadTangent rrt(Point2d(1, 0), Point2d(2, 0), Degrees(90));
	rrl.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(1, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest22)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(1, 0, 0));

	RailRoadTangent rrt(Point2d(1, 0), Point2d(-2, 0), Degrees(90));
	rrl.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(-1, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest3)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadTangent rrt(Point2d(1, 0), Point2d(1.1f, 1.0f), Degrees(90));
	rrl.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(2, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest31)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadTangent rrt(Point2d(1, 0), Point2d(0.9f, 1.0f), Degrees(90));
	rrl.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(-2, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest4)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadTangent rrt(Point2d(1, 0), Point2d(1, 1), Degrees(45));
	rrl.Accept(rrt);

	BOOST_CHECK(!rrt.GetTangentVector());
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest5)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadTangent rrt(Point2d(1, 0), Point2d(1, 1), Degrees(45));
	rrl.Accept(rrt);

	BOOST_CHECK(!rrt.GetTangentVector());
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest6)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadTangent rrt(Point2d(1, 0), Point2d(2, 2), Degrees(90));
	rrl.Accept(rrt); 

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(2, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest7)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadTangent rrt(Point2d(1, 0), Point2d(0, 1), Degrees(90));
	rrl.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(-2, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest8)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadTangent rrt(Point2d(1, 0), Point2d(-2, -2), Degrees(90));
	rrl.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(-2, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest9)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadTangent rrt(Point2d(1, 0), Point2d(0, -1), Degrees(90));
	rrl.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(-2, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest10)
{
	RailRoadArc rra(Point3d(10, 10, 0), Degrees(90), Point2d(0, 10), Rotation::Clockwise);

	RailRoadTangent rrt(Point2d(10, 10), Point2d(10, 0), Degrees(90));
	rra.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(0, -10, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest11)
{
	RailRoadArc rra(Point3d(10, 10, 0), Degrees(90), Point2d(0, 10), Rotation::Clockwise);

	RailRoadTangent rrt(Point2d(10, 10), Point2d(11, 0), Degrees(90));
	rra.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(0, -10, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest12)
{
	RailRoadArc rra(Point3d(10, 10, 0), Degrees(90), Point2d(0, 10), Rotation::Clockwise);

	RailRoadTangent rrt(Point2d(10, 10), Point2d(11, 10), Degrees(90));
	rra.Accept(rrt);

	BOOST_CHECK(!rrt.GetTangentVector());
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest13)
{
	RailRoadArc rra(Point3d(10, 10, 0), Degrees(90), Point2d(0, 10), Rotation::Clockwise);

	RailRoadTangent rrt(Point2d(10, 10), Point2d(11, 11), Degrees(45));
	rra.Accept(rrt);

	BOOST_CHECK(!rrt.GetTangentVector());
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest14)
{
	RailRoadArc rra(Point3d(10, 10, 0), Degrees(90), Point2d(0, 10), Rotation::Clockwise);

	RailRoadTangent rrt(Point2d(10, 10), Point2d(11, 11), Degrees(90));
	rra.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(0, 10, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest15)
{
	RailRoadArc rra(Point3d(10, 10, 0), Degrees(90), Point2d(0, 10), Rotation::Clockwise);

	RailRoadTangent rrt(Point2d(10, 10), Point2d(11, 11), Degrees(90));
	rra.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(0, 10, 0));
}

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
