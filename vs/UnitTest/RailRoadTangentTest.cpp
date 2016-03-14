#include <boost/test/unit_test.hpp>

#include <RailRoadArc.h>
#include <RailRoadLine.h>
#include <CommonUtils.h>
#include <RailRoadTangent.h>
#include <Point2d.h>

#include <boost/math/constants/constants.hpp>

using namespace trm;

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
	RailRoadArc rra({ Point3d(10, 10, 0), Degrees(-90), Point2d(0, 10), 1.0f });

	RailRoadTangent rrt(Point2d(10, 10), Point2d(10, 0), Degrees(90));
	rra.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(0, -10, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest11)
{
	RailRoadArc rra({ Point3d(10, 10, 0), Degrees(-90), Point2d(0, 10), 1.0f });

	RailRoadTangent rrt(Point2d(10, 10), Point2d(11, 0), Degrees(90));
	rra.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(0, -10, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest12)
{
	RailRoadArc rra({ Point3d(10, 10, 0), Degrees(-90), Point2d(0, 10), 1.0f });

	RailRoadTangent rrt(Point2d(10, 10), Point2d(11, 10), Degrees(90));
	rra.Accept(rrt);

	BOOST_CHECK(!rrt.GetTangentVector());
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest13)
{
	RailRoadArc rra({ Point3d(10, 10, 0), Degrees(-90), Point2d(0, 10), 1.0f });

	RailRoadTangent rrt(Point2d(10, 10), Point2d(11, 11), Degrees(45));
	rra.Accept(rrt);

	BOOST_CHECK(!rrt.GetTangentVector());
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest14)
{
	RailRoadArc rra({ Point3d(10, 10, 0), Degrees(-90), Point2d(0, 10), 1.0f });

	RailRoadTangent rrt(Point2d(10, 10), Point2d(11, 11), Degrees(90));
	rra.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(0, 10, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest15)
{
	RailRoadArc rra({ Point3d(10, 10, 0), Degrees(-90), Point2d(0, 10), 1.0f });

	RailRoadTangent rrt(Point2d(10, 10), Point2d(11, 11), Degrees(90));
	rra.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(0, 10, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest16)
{
	RailRoadArc rra({ Point3d(10, 10, 0), Degrees(-90), Point2d(0, 10), 1.0f });

	RailRoadTangent rrt(Point2d(0, 0), Point2d(-1, -1), Degrees(90));
	rra.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector()->x(), -10.0f);
	BOOST_CHECK(utils::CheckNear(rrt.GetTangentVector()->y(), 0.0f, 0.00001f));
	BOOST_CHECK_EQUAL(rrt.GetTangentVector()->z(), 0.0f);
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest17)
{
	RailRoadArc rra({ Point3d(10, 10, 0), Degrees(-180), Point2d(0, 10), 1.0f });

	RailRoadTangent rrt(Point2d(0, 0), Point2d(1, -1), Degrees(90));
	rra.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector()->x(), 10.0f);
	BOOST_CHECK(utils::CheckNear(rrt.GetTangentVector()->y(), 0.0f, 0.00001f));
	BOOST_CHECK_EQUAL(rrt.GetTangentVector()->z(), 0.0f);
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTest18)
{
	RailRoadArc rra({ Point3d(20, 10, 0), Degrees(-180), Point2d(10, 10), 1.0f });

	RailRoadTangent rrt(Point2d(10, 0), Point2d(1, -1), Degrees(90));
	rra.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector()->x(), -10.0f);
	BOOST_CHECK(utils::CheckNear(rrt.GetTangentVector()->y(), 0.0f, 0.00001f));
	BOOST_CHECK_EQUAL(rrt.GetTangentVector()->z(), 0.0f);
}
