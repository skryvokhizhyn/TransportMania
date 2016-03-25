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

BOOST_AUTO_TEST_CASE(RailRoadTangentDirectionWiseLineTest1)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(1, 0, 0));

	RailRoadTangent rrt(Point2d(1, 0), Point2d(2, 0), Degrees(90));
	rrl.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(1, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentDirectionWiseLineTest2)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(1, 0, 0));

	RailRoadTangent rrt(Point2d(1, 0), Point2d(-2, 0), Degrees(90));
	rrl.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(-1, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentDirectionWiseLineTest3)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadTangent rrt(Point2d(1, 0), Point2d(1.1f, 1.0f), Degrees(90));
	rrl.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(2, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentDirectionWiseLineTest4)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadTangent rrt(Point2d(1, 0), Point2d(0.9f, 1.0f), Degrees(90));
	rrl.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(-2, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentDirectionWiseLineTest5)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadTangent rrt(Point2d(0, 0), Point2d(1, 0), Degrees(90));
	rrl.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(2, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentDirectionWiseLineTest6)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadTangent rrt(Point2d(2, 0), Point2d(1, 0), Degrees(90));
	rrl.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(-2, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentDirectionWiseLineTest7)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadTangent rrt(Point2d(1, 0), Point2d(0, 1), Degrees(90));
	rrl.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(-2, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentDirectionWiseLineTest8)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadTangent rrt(Point2d(1, 0), Point2d(-2, -2), Degrees(90));
	rrl.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(-2, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentDirectionWiseLineTest9)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadTangent rrt(Point2d(1, 0), Point2d(0, -1), Degrees(90));
	rrl.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(-2, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentNoResultTest1)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadTangent rrt(Point2d(1, 0), Point2d(1, 1), Degrees(45));
	rrl.Accept(rrt);

	BOOST_CHECK(!rrt.GetTangentVector());
}

BOOST_AUTO_TEST_CASE(RailRoadTangentNoResultTest2)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadTangent rrt(Point2d(1, 0), Point2d(1, 1), Degrees(45));
	rrl.Accept(rrt);

	BOOST_CHECK(!rrt.GetTangentVector());
}

BOOST_AUTO_TEST_CASE(RailRoadTangentNoResultTest3)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadTangent rrt(Point2d(1, 1), Point2d(2, 1), Degrees(45));
	rrl.Accept(rrt);

	BOOST_CHECK(!rrt.GetTangentVector());
}

BOOST_AUTO_TEST_CASE(RailRoadTangentNoResultTest4)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadTangent rrt(Point2d(2, 1), Point2d(1, 1), Degrees(45));
	rrl.Accept(rrt);

	BOOST_CHECK(!rrt.GetTangentVector());
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTestAtArcEdge1)
{
	RailRoadArc rra({ Point3d(1, 1, 0), Degrees(-90), Point2d(0, 1), 1.0f });

	RailRoadTangent rrt(Point2d(1, 1), Point2d(1, 0), Degrees(90));
	rra.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(0, -1, 1));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTestAtArcEdge2)
{
	RailRoadArc rra({ Point3d(1, 1, 0), Degrees(-90), Point2d(0, 1), 1.0f });

	RailRoadTangent rrt(Point2d(1, 1), Point2d(1, 2), Degrees(90));
	rra.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(0, 1, -1));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTestAtArcEdge3)
{
	RailRoadArc rra({ Point3d(1, 1, 0), Degrees(-90), Point2d(0, 1), 1.0f });

	RailRoadTangent rrt(Point2d(0, 0), Point2d(-1, 0), Degrees(90));
	rra.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(-1, 0, 1));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentTestAtArcEdge4)
{
	RailRoadArc rra({ Point3d(1, 1, 0), Degrees(-90), Point2d(0, 1), 1.0f });

	RailRoadTangent rrt(Point2d(0, 0), Point2d(1, 0), Degrees(90));
	rra.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(1, 0, -1));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentMiddleArcShiftedTest1)
{
	RailRoadArc rra({ Point3d(2, 2, 1), Degrees(-180), Point2d(1, 2), 1.0f });

	RailRoadTangent rrt(Point2d(1, 1), Point2d(-1, -1), Degrees(90));
	rra.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(-1.0f, 0.0f, 0.5f));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentMiddleArcShiftedTest2)
{
	RailRoadArc rra({ Point3d(2, 2, 1), Degrees(-180), Point2d(1, 2), 1.0f });

	RailRoadTangent rrt(Point2d(1, 1), Point2d(2, 2), Degrees(90));
	rra.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(1.0f, 0.0f, -0.5f));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentMiddleArcShiftedTest3)
{
	RailRoadArc rra({ Point3d(5, 5, 0), Degrees(-90), Point2d(5, 10), -0.0f });

	RailRoadTangent rrt(Point2d(0, 10), Point2d(0, 9), Degrees(45));
	rra.Accept(rrt);

	BOOST_CHECK(utils::CheckNear(rrt.GetTangentVector()->x(), 0.0f, 0.00001f));
	BOOST_CHECK_EQUAL(rrt.GetTangentVector()->y(), -5.0f);
	BOOST_CHECK_EQUAL(rrt.GetTangentVector()->z(), -0.0f);
}

BOOST_AUTO_TEST_CASE(RailRoadTangentMiddleArcShiftedTest4)
{
	RailRoadArc rra({ Point3d(5, 5, 0), Degrees(-90), Point2d(5, 10), 0.0f });

	RailRoadTangent rrt(Point2d(5, 5), Point2d(6, 5), Degrees(45));
	rra.Accept(rrt);

	BOOST_CHECK_EQUAL(rrt.GetTangentVector(), Point3d(5, 0, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadTangentNotOnArcTest1)
{
	RailRoadArc rra({ Point3d(10, 10, 0), Degrees(90), Point2d(0, 10), 1.0f });

	RailRoadTangent rrt(Point2d(0, 0), Point2d(10, 20), Degrees(45));
	rra.Accept(rrt);

	BOOST_CHECK(!rrt.GetTangentVector());
}
