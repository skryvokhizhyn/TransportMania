#include <boost/test/unit_test.hpp>

#include <RailRoadSizer.h>
#include <RailRoadParametersTaker.h>
#include <RailRoadArc.h>
#include <RailRoadLine.h>
#include <CommonUtils.h>
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
	RailRoadArc rra(Point3d(0, 0, 0), Degrees(360), Point2d(10, 0), Direction::Right);

	RailRoadSizer rrs;
	rra.Accept(rrs);

	BOOST_CHECK_EQUAL(rrs.GetLenght(), 2 * 10.0f * boost::math::constants::pi<float>());
}

BOOST_AUTO_TEST_CASE(RailRoadSizerTest3)
{
	RailRoadArc rra(Point3d(0, 0, 0), Degrees(180), Point2d(0, 10), Direction::Right);

	RailRoadSizer rrs;
	rra.Accept(rrs);

	BOOST_CHECK_EQUAL(rrs.GetLenght(), 10.0f * boost::math::constants::pi<float>());
}

BOOST_AUTO_TEST_CASE(RailRoadParametersTakerTest1)
{
	RailRoadArc rra(Point3d(0, 0, 0), Degrees(180), Point2d(0, 10), Direction::Right);

	RailRoadParametersTaker rrpt;
	rra.Accept(rrpt);

	BOOST_CHECK_EQUAL(rrpt.GetStart(), Point3d(0, 0, 0));

	const Point3d p = rrpt.GetEnd();

	BOOST_CHECK_PREDICATE(std::less_equal<float>(), (p.x())(0.00001f));
	BOOST_CHECK(utils::CheckEqual(p.y(), 20.0f));
	BOOST_CHECK(utils::CheckEqual(p.z(), 0.0f));
}

BOOST_AUTO_TEST_CASE(RailRoadParametersTakerTest2)
{
	RailRoadArc rra(Point3d(0, 0, 0), Degrees(180), Point2d(0, 10), Direction::Left);

	RailRoadParametersTaker rrpt;
	rra.Accept(rrpt);

	BOOST_CHECK_EQUAL(rrpt.GetStart(), Point3d(0, 0, 0));

	const Point3d p = rrpt.GetEnd();

	BOOST_CHECK_PREDICATE(std::less_equal<float>(), (p.x())(0.00001f));
	BOOST_CHECK(utils::CheckEqual(p.y(), 20.0f));
	BOOST_CHECK(utils::CheckEqual(p.z(), 0.0f));
}

BOOST_AUTO_TEST_CASE(RailRoadParametersTakerTest3)
{
	RailRoadArc rra(Point3d(0, 0, 0), Degrees(90), Point2d(0, 10), Direction::Left);

	RailRoadParametersTaker rrpt;
	rra.Accept(rrpt);

	BOOST_CHECK_EQUAL(rrpt.GetStart(), Point3d(0, 0, 0));
	BOOST_CHECK_EQUAL(rrpt.GetEnd(), Point3d(-10, 10, 0));
}

BOOST_AUTO_TEST_CASE(RailRoadParametersTakerTest4)
{
	RailRoadArc rra(Point3d(0, 0, 0), Degrees(90), Point2d(0, 10), Direction::Right);

	RailRoadParametersTaker rrpt;
	rra.Accept(rrpt);

	BOOST_CHECK_EQUAL(rrpt.GetStart(), Point3d(0, 0, 0));
	BOOST_CHECK_EQUAL(rrpt.GetEnd(), Point3d(10, 10, 0));
}