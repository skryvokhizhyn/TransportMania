#include <boost/test/unit_test.hpp>

#include <Point2d.h>
#include <SubjectAction.h>
#include <EventStateMachineDefs.h>

using namespace trm::impl;
using namespace trm;

namespace trm
{
namespace impl
{
	std::ostream & operator << (std::ostream & o, const ActionType a)
	{
		switch (a)
		{
		case ActionType::Bend:
			o << "Bend";
			break;
		case ActionType::Zoom:
			o << "Zoom";
			break;
		case ActionType::Rotate:
			o << "Rotate";
			break;
		case ActionType::Undefined:
			o << "Undefined";
			break;
		}

		return o;
	}

} // namespace impl
} // namespace trm

BOOST_AUTO_TEST_CASE(GetSubjectActionTest1)
{
	PointMove p1(Point2d(0, 0));
	PointMove p2(Point2d(0, 0));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(45));

	BOOST_CHECK_EQUAL(at, ActionType::Undefined);
}

BOOST_AUTO_TEST_CASE(GetSubjectActionUndefinedTest1)
{
	PointMove p1(Point2d(0, 0), Point2d(-1.0f, 0.0f));
	PointMove p2(Point2d(0, 0), Point2d(0.0f, 1.0f));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(45));

	BOOST_CHECK_EQUAL(at, ActionType::Undefined);
}

BOOST_AUTO_TEST_CASE(GetSubjectActionUndefinedTest2)
{
	PointMove p1(Point2d(5, 1));
	PointMove p2(Point2d(0, 0));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(45));

	BOOST_CHECK_EQUAL(at, ActionType::Undefined);
}

BOOST_AUTO_TEST_CASE(GetSubjectActionUndefinedTest3)
{
	PointMove p1(Point2d(0, 0));
	PointMove p2(Point2d(0, 0), Point2d(1.0f, 0.0f));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(45));

	BOOST_CHECK_EQUAL(at, ActionType::Undefined);
}

BOOST_AUTO_TEST_CASE(GetSubjectActionUndefinedTest4)
{
	PointMove p1(Point2d(0, 0), Point2d(-1.0f, 0.0f));
	PointMove p2(Point2d(0, 0));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(45));

	BOOST_CHECK_EQUAL(at, ActionType::Undefined);
}

BOOST_AUTO_TEST_CASE(GetSubjectActionZoomTest1)
{
	PointMove p1(Point2d(0, 0), Point2d(-1.0f, 0.0f));
	PointMove p2(Point2d(0, 0), Point2d(1.0f, 0.0f));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(45));

	BOOST_CHECK_EQUAL(at, ActionType::Zoom);
	BOOST_CHECK_EQUAL(GetZoomValue(p1, p2), 1.0f);
}

BOOST_AUTO_TEST_CASE(GetSubjectActionZoomTest2)
{
	PointMove p1(Point2d(0, 0), Point2d(-1.0f, 0.0f));
	PointMove p2(Point2d(2.0f, 1.0f), Point2d(3.0f, 1.0f));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(45));

	BOOST_CHECK_EQUAL(at, ActionType::Zoom);
	BOOST_CHECK_EQUAL(GetZoomValue(p1, p2), 1.0f);
}

BOOST_AUTO_TEST_CASE(GetSubjectActionZoomTest3)
{
	PointMove p1(Point2d(0, 0), Point2d(0.0f, 3.0f));
	PointMove p2(Point2d(5.0f, 100.0f), Point2d(3.0f, 80.0f));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(45));

	BOOST_CHECK_EQUAL(at, ActionType::Zoom);
}

BOOST_AUTO_TEST_CASE(GetSubjectActionZoomTest4)
{
	PointMove p1(Point2d(-1.0f, 0.0f), Point2d(0, 0));
	PointMove p2(Point2d(3.0f, 1.0f), Point2d(2.0f, 1.0f));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(45));

	BOOST_CHECK_EQUAL(at, ActionType::Zoom);
	BOOST_CHECK_EQUAL(GetZoomValue(p1, p2), -1.0f);
}

BOOST_AUTO_TEST_CASE(GetSubjectActionRotateTest1)
{
	PointMove p1(Point2d(0, 0), Point2d(0.0f, 1.0f));
	PointMove p2(Point2d(5, 0), Point2d(5.0f, -1.0f));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(45));

	BOOST_CHECK_EQUAL(at, ActionType::Rotate);
}

BOOST_AUTO_TEST_CASE(GetSubjectActionRotateTest2)
{
	PointMove p1(Point2d(-100, 0), Point2d(-80.0f, -70.0f));
	PointMove p2(Point2d(0, 0), Point2d(10.0f, 71.0f));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(45));

	BOOST_CHECK_EQUAL(at, ActionType::Rotate);
}

BOOST_AUTO_TEST_CASE(GetSubjectActionRotateTest3)
{
	PointMove p1(Point2d(0, 0), Point2d(0, 1));
	PointMove p2(Point2d(1, 1), Point2d(1, 0));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(45));

	BOOST_CHECK_EQUAL(at, ActionType::Rotate);
	BOOST_CHECK_EQUAL(GetRotateAngle(p1, p2), Degrees(-90));
}

BOOST_AUTO_TEST_CASE(GetSubjectActionRotateTest4)
{
	PointMove p1(Point2d(0, 0), Point2d(0, -1));
	PointMove p2(Point2d(1, -1), Point2d(1, 0));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(45));

	BOOST_CHECK_EQUAL(at, ActionType::Rotate);
	BOOST_CHECK_EQUAL(GetRotateAngle(p1, p2), Degrees(90));
}

BOOST_AUTO_TEST_CASE(GetSubjectActionRotateTest5)
{
	PointMove p1(Point2d(0, 7), Point2d(0, 3));
	PointMove p2(Point2d(5, -1), Point2d(5, 0));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(10));

	BOOST_CHECK_EQUAL(at, ActionType::Rotate);
	BOOST_CHECK_GT(GetRotateAngle(p1, p2), Degrees(0));
} 

BOOST_AUTO_TEST_CASE(GetSubjectActionRotateTest51)
{
	PointMove p1(Point2d(0, 3), Point2d(0, 7));
	PointMove p2(Point2d(5, 0), Point2d(5, -1));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(10));

	BOOST_CHECK_EQUAL(at, ActionType::Rotate);
	BOOST_CHECK_LT(GetRotateAngle(p1, p2), Degrees(0));
} 

BOOST_AUTO_TEST_CASE(GetSubjectActionRotateTest52)
{
	PointMove p1(Point2d(0, -3), Point2d(0, 7));
	PointMove p2(Point2d(5, 0), Point2d(5, -1));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(10));

	BOOST_CHECK_EQUAL(at, ActionType::Rotate);
	BOOST_CHECK_LT(GetRotateAngle(p1, p2), Degrees(0));
} 

BOOST_AUTO_TEST_CASE(GetSubjectActionRotateTest6)
{
	PointMove p1(Point2d(0, 7), Point2d(0, 3));
	PointMove p2(Point2d(5, 15), Point2d(5, 17));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(10));

	BOOST_CHECK_EQUAL(at, ActionType::Rotate);
	BOOST_CHECK_GT(GetRotateAngle(p1, p2), Degrees(0));
}

BOOST_AUTO_TEST_CASE(GetSubjectActionRotateTest7)
{
	PointMove p2(Point2d(0, 7), Point2d(0, 3));
	PointMove p1(Point2d(5, -1), Point2d(5, 0));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(10));

	BOOST_CHECK_EQUAL(at, ActionType::Rotate);
	BOOST_CHECK_GT(GetRotateAngle(p1, p2), Degrees(0));
}

BOOST_AUTO_TEST_CASE(GetSubjectActionRotateTest8)
{
	PointMove p2(Point2d(0, 7), Point2d(0, 3));
	PointMove p1(Point2d(5, 15), Point2d(5, 17));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(10));

	BOOST_CHECK_EQUAL(at, ActionType::Rotate);
	BOOST_CHECK_GT(GetRotateAngle(p1, p2), Degrees(0));
}

BOOST_AUTO_TEST_CASE(GetSubjectActionBendTest1)
{
	PointMove p1(Point2d(0, 0), Point2d(0.0f, 1.0f));
	PointMove p2(Point2d(5, 0), Point2d(5.0f, 1.0f));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(45));

	BOOST_CHECK_EQUAL(at, ActionType::Bend);
}

BOOST_AUTO_TEST_CASE(GetSubjectActionBendTest2)
{
	PointMove p1(Point2d(0, 0), Point2d(0.0f, 1.0f));
	PointMove p2(Point2d(0, 0), Point2d(1.0f, 7.0f));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(45));

	BOOST_CHECK_EQUAL(at, ActionType::Bend);
}

BOOST_AUTO_TEST_CASE(GetSubjectActionBendTest3)
{
	PointMove p1(Point2d(0, 0), Point2d(0.0f, -91.0f));
	PointMove p2(Point2d(60, 0), Point2d(61.0f, -7.0f));
	const ActionType at = GetSubjectAction(p1, p2, Degrees(45));

	BOOST_CHECK_EQUAL(at, ActionType::Bend);
}