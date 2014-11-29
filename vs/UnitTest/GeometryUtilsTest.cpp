//#define BOOST_TEST_DYN_LINK

#include <Point2d.h>
#include <GeometryUtils.h>
#include <Line.h>

#include <boost/test/unit_test.hpp>

using namespace utils;
using namespace trm;

namespace utils
{
	std::ostream & operator << (std::ostream & o, const utils::Codirection c)
	{
		switch (c)
		{
		case Codirection::Same:
			o << "Same";
			break;

		case Codirection::Opposite:
			o << "Opposite";
			break;

		case Codirection::None:
			o << "None";
			break;
		}

		return o;
	}
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest1)
{
	Point2d p1(0.0f, 0.0f);
	Point2d p2(1.0f, 1.0f);
	Point2d p3(2, 2);
	Point2d p4(-1.3f, -1.3f);
	Point2d p5(-1.3f, -1.2f);

	const Line l = GetLine(p1, p2);

	const float epsilon = std::numeric_limits<float>::epsilon();

	BOOST_CHECK_PREDICATE( std::less_equal<float>(), (l.a * p1.x() + l.b * p1.y() + l.c)(epsilon) );
	BOOST_CHECK_PREDICATE( std::less_equal<float>(), (l.a * p2.x() + l.b * p2.y() + l.c)(epsilon) );
	BOOST_CHECK_PREDICATE( std::less_equal<float>(), (l.a * p3.x() + l.b * p3.y() + l.c)(epsilon) );
	BOOST_CHECK_PREDICATE( std::less_equal<float>(), (l.a * p4.x() + l.b * p4.y() + l.c)(epsilon) );
	BOOST_CHECK_PREDICATE( std::greater<float>(), (abs(l.a * p5.x() + l.b * p5.y() + l.c))(epsilon) );
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest2)
{
	Point2d p1(-1.0f, 0.0f);
	Point2d p2(1.0f, 0.0f);
	Point2d p4(0.0f, -1.0f);
	Point2d p5(0.0f, 1.0f);

	const Line l1 = GetLine(p1, p2);
	const Line l2 = GetLine(p4, p5);

	const Point2d i = utils::GetIntersectionPoint(l1, l2);
	BOOST_CHECK_EQUAL(i, Point2d(0.0f, 0.0f));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest3)
{
	Point2d p1(-2.0f, -1.0f);
	Point2d p2(0.0f, 1.0f);
	Point2d p4(-2.0f, 1.0f);
	Point2d p5(0.0f, -1.0f);

	const Line l1 = GetLine(p1, p2);
	const Line l2 = GetLine(p4, p5);

	const Point2d i = utils::GetIntersectionPoint(l1, l2);
	BOOST_CHECK_EQUAL(i, Point2d(-1.0f, 0.0f));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetIntersectionTest4)
{
	Point2d p1(-1.5, 3);
	Point2d p2(1.5, 3);
	Point2d p4(-1.5, 6);

	const Line l1 = GetLine(p1, p4);
	const Line l2 = GetLine(p1, p2);

	const Point2d i1 = utils::GetIntersectionPoint(l1, l2);
	BOOST_CHECK_EQUAL(i1, p1);
	const Point2d i2 = utils::GetIntersectionPoint(l2, l1);
	BOOST_CHECK_EQUAL(i2, p1);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetIntersectionTest5)
{
	Point2d p1(-0.3f, 0.3f);
	Point2d p2(0.3f, -0.3f);
	Point2d p4(2.6f, 3.3f);

	const Line l1 = GetLine(p1, p4);
	const Line l2 = GetLine(p1, p2);

	const Point2d i1 = utils::GetIntersectionPoint(l1, l2);
	BOOST_CHECK_EQUAL(i1, p1);
	const Point2d i2 = utils::GetIntersectionPoint(l2, l1);
	BOOST_CHECK_EQUAL(i2, p1);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetIntersectionTest6)
{
	Point2d p1(0.0f, 0.3f);
	Point2d p2(0.0f, -0.3f);
	Point2d p4(1.0, 0.3f);

	const Line l1 = GetLine(p1, p4);
	const Line l2 = GetLine(p1, p2);

	const Point2d i1 = utils::GetIntersectionPoint(l1, l2);
	BOOST_CHECK_EQUAL(i1, p1);
	const Point2d i2 = utils::GetIntersectionPoint(l2, l1);
	BOOST_CHECK_EQUAL(i2, p1);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetIntersectionTest7)
{
	Point2d p1(0.0f, 0.3f);
	Point2d p2(0.0f, -0.3f);
	Point2d p4(1.0, 0.3f);

	const Line l1 = GetLine(p1, p4);
	const Line l2 = GetPerpendicularAtPoint(l1, p1);

	const Point2d i1 = utils::GetIntersectionPoint(l1, l2);
	BOOST_CHECK_EQUAL(i1, p1);
	const Point2d i2 = utils::GetIntersectionPoint(l2, l1);
	BOOST_CHECK_EQUAL(i2, p1);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest4)
{
	Point2d p1(-2.0f, -1.0f);
	Point2d p2(0.0f, 1.0f);
	Point2d p4(-2.0f, 1.0f);
	Point2d p5(0.0f, -1.0f);

	const Line l1 = GetLine(p1, p2);
	const Line l2 = GetLine(p4, p5);

	BOOST_CHECK(!utils::CheckParallel(l1, l2));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest5)
{
	Point2d p1(-2.0f, -2.0f);
	Point2d p2(2.0f, -2.0f);
	Point2d p4(2.0f, 2.0f);
	Point2d p5(4.0f, 2.0f);

	const Line l1 = GetLine(p1, p2);
	const Line l2 = GetLine(p4, p5);

	BOOST_CHECK(utils::CheckParallel(l1, l2));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest6)
{
	BOOST_CHECK_EQUAL(utils::GetAngle(Point3d(-1, 0, 0), Point3d(0, 1, 0)), Degrees(90));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest7)
{
	BOOST_CHECK_EQUAL(utils::GetAngle(Point3d(-1, 0, 0), Point3d(1, 0, 0)), Degrees(180));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest8)
{
	const Angle a = utils::GetAngle(Point3d(-1, 0, 0), Point3d(0, -1, 0));

	BOOST_CHECK_EQUAL(a, Degrees(90));
	BOOST_CHECK_NE(a, Degrees(270));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest9)
{
	const Angle a = utils::GetAngle(Point3d(0, -1, 0), Point3d(-1, 0, 0));

	BOOST_CHECK_EQUAL(a, Degrees(90));
	BOOST_CHECK_NE(a, Degrees(270));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest61)
{
	BOOST_CHECK_EQUAL(utils::GetAngle(Point2d(-1, 0), Point2d(0, 1)), Degrees(90));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest62)
{
	BOOST_CHECK_EQUAL(utils::GetAngle(Point2d(-5, 0), Point2d(0, 1)), Degrees(90));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest71)
{
	BOOST_CHECK_EQUAL(utils::GetAngle(Point2d(-1, 0), Point2d(1, 0)), Degrees(180));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest81)
{
	const Angle a = utils::GetAngle(Point2d(-1, 0), Point2d(0, -1));

	BOOST_CHECK_EQUAL(a, Degrees(90));
	BOOST_CHECK_NE(a, Degrees(270));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest91)
{
	const Angle a = utils::GetAngle(Point2d(0, -1), Point2d(-1, 0));

	BOOST_CHECK_EQUAL(a, Degrees(90));
	BOOST_CHECK_NE(a, Degrees(270));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest10)
{
	const bool r = utils::CheckColinear(Point2d(1, 1), Point2d(2, 2));

	BOOST_CHECK(r);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest11)
{
	const bool r = utils::CheckColinear(Point2d(0, 1), Point2d(0, -2));
	
	BOOST_CHECK(!r);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest12)
{
	const bool r = utils::CheckColinear(Point2d(1, 1), Point2d(2, 3));

	BOOST_CHECK(!r);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest13)
{
	const bool r = utils::CheckColinear(Point2d(-1, 1), Point2d(2, -2));

	BOOST_CHECK(!r);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest14)
{
	const bool r = utils::CheckColinear(Point2d(1, -1), Point2d(-2, 2));

	BOOST_CHECK(!r);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest15)
{
	const bool r = utils::CheckColinear(Point2d(1, 0), Point2d(-2, 0));

	BOOST_CHECK(!r);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest161)
{
	const bool r = utils::CheckColinear(Point2d(0, 1), Point2d(0, 2));

	BOOST_CHECK(r);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest162)
{
	const bool r = utils::CheckColinear(Point2d(0, 1), Point2d(1, 2));

	BOOST_CHECK(!r);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest163)
{
	const bool r = utils::CheckColinear(Point2d(1, 2), Point2d(0, 1));

	BOOST_CHECK(!r);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest164)
{
	const bool r = utils::CheckColinear(Point2d(1, 2), Point2d(1, 0));

	BOOST_CHECK(!r);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest17)
{
	BOOST_CHECK_THROW(utils::CheckColinear(Point2d(0, 0), Point2d(0, 2)), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest18)
{
	BOOST_CHECK_THROW(utils::CheckColinear(Point2d(0, 1), Point2d(0, 0)), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(GeometryCheckCodirectionalTest1)
{
	const Codirection r = utils::CheckCodirectional(Point2d(1, 0), Point2d(-2, 0));

	BOOST_CHECK_EQUAL(r, Codirection::Opposite);
}

BOOST_AUTO_TEST_CASE(GeometryCheckCodirectionalTest2)
{
	const Codirection r = utils::CheckCodirectional(Point2d(1, 0), Point2d(2, 0));

	BOOST_CHECK_EQUAL(r, Codirection::Same);
}

BOOST_AUTO_TEST_CASE(GeometryCheckCodirectionalTest3)
{
	const Codirection r = utils::CheckCodirectional(Point2d(1, 2), Point2d(-2, 0));

	BOOST_CHECK_EQUAL(r, Codirection::None);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest16)
{
	const Angle a = utils::GetAngle(Point3d(-1, 0, 0), Point3d(1, 0, 0));

	BOOST_CHECK_EQUAL(a, Degrees(180));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTestGetSignedAngle17)
{
	const Angle a = utils::GetSignedAngle(Point2d(1, 0), Point2d(-1, 0));

	BOOST_CHECK_EQUAL(a, -Degrees(0));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTestGetSignedAngle18)
{
	const Angle a = utils::GetSignedAngle(Point2d(-1, 0), Point2d(0, 1));

	BOOST_CHECK_EQUAL(a, Degrees(-90));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTestGetSignedAngle19)
{
	const Angle a = utils::GetSignedAngle(Point2d(-1, 0), Point2d(0, -1));

	BOOST_CHECK_EQUAL(a, Degrees(90));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTestGetSignedAngle20)
{
	const Angle a = utils::GetSignedAngle(Point2d(0, -1), Point2d(-1, 0));

	BOOST_CHECK_EQUAL(a, -Degrees(90));
}
//
//#include <boost/units/cmath.hpp>
//
//BOOST_AUTO_TEST_CASE(GeometryUtilsTestGetSignedAngle21)
//{
//	const Angle a = utils::GetSignedAngle(Point2d(0, 1), Point2d(boost::units::cos(Degrees(135)), boost::units::sin(Degrees(135))));
//
//	BOOST_CHECK_EQUAL(a, Degrees(45));
//}

BOOST_AUTO_TEST_CASE(GeometryUtilsTestGetSignedAngle22)
{
	const Angle a = utils::GetSignedAngle(Point2d(-1, 0), Point2d(1, 0));

	BOOST_CHECK_EQUAL(a, Degrees(0));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTestGetSignedAngle1801)
{
	const Angle a = utils::GetSignedAngle180(Point2d(1, 0), Point2d(1, 0));

	BOOST_CHECK_EQUAL(a, Degrees(0));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTestGetSignedAngle18011)
{
	const Angle a = utils::GetSignedAngle180(Point2d(5, 0), Point2d(1, 0));

	BOOST_CHECK_EQUAL(a, Degrees(0));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTestGetSignedAngle1802)
{
	const Angle a = utils::GetSignedAngle180(Point2d(-1, 0), Point2d(0, 1));

	BOOST_CHECK_EQUAL(a, Degrees(-90));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTestGetSignedAngle18021)
{
	const Angle a = utils::GetSignedAngle180(Point2d(-3, 0), Point2d(0, 1));

	BOOST_CHECK_EQUAL(a, Degrees(-90));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTestGetSignedAngle18022)
{
	const Angle a = utils::GetSignedAngle180(Point2d(-1, 0), Point2d(0, 4));

	BOOST_CHECK_EQUAL(a, Degrees(-90));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTestGetSignedAngle1803)
{
	const Angle a = utils::GetSignedAngle180(Point2d(0, 1), Point2d(-1, 0));

	BOOST_CHECK_EQUAL(a, Degrees(90));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTestGetSignedAngle18031)
{
	const Angle a = utils::GetSignedAngle180(Point2d(0, 3), Point2d(-1, 0));

	BOOST_CHECK_EQUAL(a, Degrees(90));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTestGetSignedAngle18032)
{
	const Angle a = utils::GetSignedAngle180(Point2d(0, 1), Point2d(-8, 0));

	BOOST_CHECK_EQUAL(a, Degrees(90));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTestGetSignedAngle1804)
{
	const Angle a = utils::GetSignedAngle180(Point2d(1, 0), Point2d(-1, 0));

	BOOST_CHECK_EQUAL(a, Degrees(-180));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTestGetSignedAngle1805)
{
	const Angle a = utils::GetSignedAngle180(Point2d(-1, 0), Point2d(1, 0));

	BOOST_CHECK_EQUAL(a, Degrees(-180));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTestGetSignedAngle1806)
{
	const Angle a = utils::GetSignedAngle180(Point2d(1, 0), Point2d(static_cast<float>(sqrt(2)/2), static_cast<float>(sqrt(2)/2)));

	BOOST_CHECK(a - Degrees(45) < Degrees(0.1f));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTestGetSignedAngle1807)
{
	const Angle a = utils::GetSignedAngle180(Point2d(0, 1), Point2d(static_cast<float>(sqrt(2)/2), static_cast<float>(sqrt(2)/2)));

	BOOST_CHECK(a - Degrees(-45) < Degrees(0.1f));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest22)
{
	const Point2d p = utils::RotateVector(Point2d(0, -1), Degrees(90), Rotation::Clockwise);

	BOOST_CHECK_EQUAL(p, Point2d(-1, 0));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest23)
{
	const Point2d p = utils::RotateVector(Point2d(0, -1), Degrees(90), Rotation::AntiClockwise);

	BOOST_CHECK_EQUAL(p, Point2d(1, 0));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest24)
{
	const Point2d p = utils::RotateVector(Point2d(0, -1), Degrees(180), Rotation::Clockwise);

	BOOST_CHECK_EQUAL(p, Point2d(0, 1));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest25)
{
	const Point2d p = utils::RotateVector(Point2d(0, -1), Degrees(180), Rotation::AntiClockwise);

	BOOST_CHECK_EQUAL(p, Point2d(0, 1));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest26)
{
	const Point2d p = utils::RotateVector(Point2d(0, -1), Degrees(135), Rotation::Clockwise);

	BOOST_CHECK_CLOSE(p.x(), -0.707107f, 0.0001f);
	BOOST_CHECK_CLOSE(p.y(), 0.707107f, 0.0001f);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsTest27)
{
	const Point2d p = utils::RotateVector(Point2d(0, -1), Degrees(135), Rotation::AntiClockwise);

	BOOST_CHECK_CLOSE(p.x(), 0.707107f, 0.0001f);
	BOOST_CHECK_CLOSE(p.y(), 0.707107f, 0.0001f);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetDistanceTest1)
{
	const Line l = utils::GetLine(Point2d(0, 0), Point2d(1, 0));

	const float d1 = utils::GetDistance(l, Point2d(0, 0));
	const float d2 = utils::GetDistance(l, Point2d(1, 0));
	const float d3 = utils::GetDistance(l, Point2d(-1, 0));

	const float d4 = utils::GetDistance(l, Point2d(0, 1));
	const float d5 = utils::GetDistance(l, Point2d(1, 1));
	const float d6 = utils::GetDistance(l, Point2d(-1, 1));

	const float d7 = utils::GetDistance(l, Point2d(0, -1));
	const float d8 = utils::GetDistance(l, Point2d(1, -1));
	const float d9 = utils::GetDistance(l, Point2d(-1, -1));

	BOOST_CHECK_EQUAL(d1, 0);
	BOOST_CHECK_EQUAL(d2, 0);
	BOOST_CHECK_EQUAL(d3, 0);

	BOOST_CHECK_EQUAL(d4, 1);
	BOOST_CHECK_EQUAL(d5, 1);
	BOOST_CHECK_EQUAL(d6, 1);

	BOOST_CHECK_EQUAL(d7, 1);
	BOOST_CHECK_EQUAL(d8, 1);
	BOOST_CHECK_EQUAL(d9, 1);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetDistanceTest2)
{
	const Line l = utils::GetLine(Point2d(0, 0), Point2d(0, 1));

	const float d1 = utils::GetDistance(l, Point2d(1, 0));

	BOOST_CHECK_EQUAL(d1, 1);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsCheckPerpendicularTest1)
{
	const Line l1 = utils::GetLine(Point2d(0, 0), Point2d(0, 1));
	const Line l2 = utils::GetLine(Point2d(0, 0), Point2d(1, 0));

	BOOST_CHECK(utils::CheckPerpendicular(l1, l2));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsCheckPerpendicularTest2)
{
	const Line l1 = utils::GetLine(Point2d(0, 0), Point2d(0, 1));
	const Line l2 = utils::GetLine(Point2d(0, 0), Point2d(1, 1));

	BOOST_CHECK(!utils::CheckPerpendicular(l1, l2));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsCheckPerpendicularTest3)
{
	const Line l1 = utils::GetLine(Point2d(1, 0), Point2d(1, 1));
	const Line l2 = utils::GetLine(Point2d(0, 0), Point2d(0, 1));

	BOOST_CHECK(!utils::CheckPerpendicular(l1, l2));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsCheckPerpendicularTest4)
{
	const Line l1 = utils::GetLine(Point2d(0, 0), Point2d(1, 2));
	const Line l2 = utils::GetLine(Point2d(0, 0), Point2d(2, -1));

	BOOST_CHECK(utils::CheckPerpendicular(l1, l2));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsCheckPerpendicularTest5)
{
	const Line l1 = utils::GetLine(Point2d(0, 0), Point2d(2, 1));
	const Line l2 = utils::GetLine(Point2d(0, 0), Point2d(-1, 2));

	BOOST_CHECK(utils::CheckPerpendicular(l1, l2));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetPerpendicularTest1)
{
	const Line l1 = utils::GetLine(Point2d(0, 0), Point2d(0, 1));
	const Line l2 = utils::GetPerpendicularAtPoint(l1, Point2d(0, -7));
	const Line l3 = utils::GetLine(Point2d(0, -1), Point2d(1, -1));

	BOOST_CHECK(utils::CheckPerpendicular(l1, l2));
	BOOST_CHECK(utils::CheckParallel(l2, l3));
	BOOST_CHECK_EQUAL(l2.a * 5 + l2.b * (-7) + l2.c, 0.0f);
}

//BOOST_AUTO_TEST_CASE(GeometryUtilsGetPerpendicularTest2)
//{
//	const Line l1 = utils::GetLine(Point2d(0, 0), Point2d(0, 1));
//	BOOST_CHECK_THROW(utils::GetPerpendicularAtPoint(l1, Point2d(1, -7)), std::runtime_error);
//}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetPerpendicularTest3)
{
	const Line l1 = utils::GetLine(Point2d(0, 0), Point2d(1, 1));
	const Line l2 = utils::GetPerpendicularAtPoint(l1, Point2d(2, 2));
	const Line l3 = utils::GetLine(Point2d(0, -1), Point2d(1, -2));

	BOOST_CHECK(utils::CheckPerpendicular(l1, l2));
	BOOST_CHECK(utils::CheckParallel(l2, l3));
	BOOST_CHECK_EQUAL(l2.a * (4) + l2.b * (0) + l2.c, 0.0f);
	BOOST_CHECK_EQUAL(l2.a * (0) + l2.b * (4) + l2.c, 0.0f);
	BOOST_CHECK_EQUAL(l2.a * (6) + l2.b * (-2) + l2.c, 0.0f);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetPerpendicularTest4)
{
	const Line l1 = utils::GetLine(Point2d(0, 0), Point2d(1, 0));
	const Line l2 = utils::GetPerpendicularAtPoint(l1, Point2d(0, 0));
	const Line l3 = utils::GetLine(Point2d(1, -1), Point2d(1, -2));

	BOOST_CHECK(utils::CheckPerpendicular(l1, l2));
	BOOST_CHECK(utils::CheckParallel(l2, l3));
	BOOST_CHECK_EQUAL(l2.a * (0) + l2.b * (0) + l2.c, 0.0f);
	BOOST_CHECK_EQUAL(l2.a * (0) + l2.b * (2) + l2.c, 0.0f);
	BOOST_CHECK_EQUAL(l2.a * (0) + l2.b * (-1) + l2.c, 0.0f);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetPerpendicularTest5)
{
	const Line l1 = utils::GetLine(Point2d(0, 0), Point2d(1, 2));
	const Line l2 = utils::GetPerpendicularAtPoint(l1, Point2d(1, 2));
	//const Line l3 = utils::GetLine(Point2d(1, -1), Point2d(1, -2));

	BOOST_CHECK(utils::CheckPerpendicular(l1, l2));
	//BOOST_CHECK(utils::CheckParallel(l2, l3));
	BOOST_CHECK_EQUAL(l2.a * (5) + l2.b * (0) + l2.c, 0.0f);
	BOOST_CHECK_EQUAL(l2.a * (0) + l2.b * (2.5f) + l2.c, 0.0f);
	BOOST_CHECK_EQUAL(l2.a * (3) + l2.b * (1) + l2.c, 0.0f);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetPerpendicularTest6)
{
	const Line l1 = utils::GetLine(Point2d(0, 0), Point2d(2, 1));
	const Line l2 = utils::GetPerpendicularAtPoint(l1, Point2d(2, 1));
	//const Line l3 = utils::GetLine(Point2d(1, -1), Point2d(1, -2));

	BOOST_CHECK(utils::CheckPerpendicular(l1, l2));
	//BOOST_CHECK(utils::CheckParallel(l2, l3));
	BOOST_CHECK_EQUAL(l2.a * (2.5f) + l2.b * (0) + l2.c, 0.0f);
	BOOST_CHECK_EQUAL(l2.a * (1.5f) + l2.b * (2) + l2.c, 0.0f);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetPerpendicularTest7)
{
	const Line l1 = utils::GetLine(Point2d(0, 1), Point2d(2, 1));
	const Line l2 = utils::GetPerpendicularAtPoint(l1, Point2d(2, 1));

	BOOST_CHECK(utils::CheckPerpendicular(l1, l2));
	BOOST_CHECK_EQUAL(l2.a * (2.0f) + l2.b * (1.0f) + l2.c, 0.0f);
	BOOST_CHECK_EQUAL(l2.a * (2.0f) + l2.b * (2.0f) + l2.c, 0.0f);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsCheckParallelTes1)
{
	const Line l1 = utils::GetLine(Point2d(0, 0), Point2d(1, 0));
	const Line l2 = utils::GetLine(Point2d(0, 0), Point2d(0, 1));

	BOOST_CHECK(!utils::CheckParallel(l1, l2));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetParallelTest1)
{
	const Line l1 = utils::GetLine(Point2d(0, 0), Point2d(1, 0));
	const Line l2 = utils::GetParallelAtPoint(l1, Point2d(1, 1));

	BOOST_CHECK(utils::CheckParallel(l1, l2));
	BOOST_CHECK_EQUAL(l2.a * 1 + l2.b * (1) + l2.c, 0.0f);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetParallelTest2)
{
	const Line l1 = utils::GetLine(Point2d(0, 0), Point2d(0, 1));
	const Line l2 = utils::GetParallelAtPoint(l1, Point2d(1, 1));

	BOOST_CHECK(utils::CheckParallel(l1, l2));
	BOOST_CHECK_EQUAL(l2.a * 1 + l2.b * (1) + l2.c, 0.0f);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetParallelTest3)
{
	const Line l1 = utils::GetLine(Point2d(1, 0), Point2d(1, 1));
	const Line l2 = utils::GetParallelAtPoint(l1, Point2d(1, 1));

	BOOST_CHECK_EQUAL(l1.a, l2.a);
	BOOST_CHECK_EQUAL(l1.b, l2.b);
	BOOST_CHECK_EQUAL(l1.c, l2.c);
	BOOST_CHECK_EQUAL(l2.a * 1 + l2.b * (1) + l2.c, 0.0f);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetParallelTest4)
{
	const Line l1 = utils::GetLine(Point2d(0, 0), Point2d(1, 1));
	const Line l2 = utils::GetParallelAtPoint(l1, Point2d(1, 2));

	BOOST_CHECK(utils::CheckParallel(l1, l2));
	BOOST_CHECK_EQUAL(l2.a * 1 + l2.b * (2) + l2.c, 0.0f);
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetDirection1)
{
	const Point2d p1 = utils::GetDirectionVector(Point2d(0, 0), Point2d(1, 1));

	BOOST_CHECK_EQUAL(p1, Point2d(0.707106769f, 0.707106769f));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetDirection2)
{
	const Point2d p1 = utils::GetDirectionVector(Point2d(1, 1), Point2d(0, 0));

	BOOST_CHECK_EQUAL(p1, Point2d(-0.707106769f, -0.707106769f));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetDirection3)
{
	const Point2d p1 = utils::GetDirectionVector(Point2d(0, 0), Point2d(-1, 1));

	BOOST_CHECK_EQUAL(p1, Point2d(-0.707106769f, 0.707106769f));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetDirection4)
{
	const Point2d p1 = utils::GetDirectionVector(Point2d(0, 0), Point2d(1, -1));

	BOOST_CHECK_EQUAL(p1, Point2d(0.707106769f, -0.707106769f));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetDirection5)
{
	const Point2d p1 = utils::GetDirectionVector(Point2d(0, 0), Point2d(1, 0));

	BOOST_CHECK_EQUAL(p1, Point2d(1, 0));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetDirection6)
{
	const Point2d p1 = utils::GetDirectionVector(Point2d(0, 0), Point2d(0, 5));

	BOOST_CHECK_EQUAL(p1, Point2d(0, 1));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsGetDirection7)
{
	const Point2d p1 = utils::GetDirectionVector(Point2d(1, 1), Point2d(-1, 1));

	BOOST_CHECK_EQUAL(p1, Point2d(-1, 0));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsCrossProductTest1)
{
	const Point3d p = utils::GetCrossProduct(Point3d(1, 0, 0), Point3d(0, -1, 0));

	BOOST_CHECK_EQUAL(p, Point3d(0, 0, -1));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsCrossProductTest2)
{
	const Point3d p = utils::GetCrossProduct(Point3d(1, 0, 0), Point3d(0, 0, 1));

	BOOST_CHECK_EQUAL(p, Point3d(0, -1, 0));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsCrossProductTest3)
{
	const Point3d p = utils::GetCrossProduct(Point3d(1, 0, 0), Point3d(1, 0, 0));

	BOOST_CHECK_EQUAL(p, Point3d(0, 0, 0));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsCrossProductTest4)
{
	const Point3d p = utils::GetCrossProduct(Point3d(1, 0, 0), Point3d(-1, 0, 0));

	BOOST_CHECK_EQUAL(p, Point3d(0, 0, 0));
}

BOOST_AUTO_TEST_CASE(GeometryUtilsCrossProductTest5)
{
	const Point3d p = utils::GetCrossProduct(Point3d(1, 0, 0), Point3d(0.5f, 0, 1));

	BOOST_CHECK_EQUAL(p, Point3d(0, -1, 0));
}

BOOST_AUTO_TEST_CASE(GeometryCheckTriangleValidTest1)
{
	BOOST_CHECK_EQUAL(CheckTriangleValid(Triangle3d(Point3d(), Point3d(), Point3d(0, 0, 1))), false);
	BOOST_CHECK_EQUAL(CheckTriangleValid(Triangle3d(Point3d(0, 0, 1), Point3d(), Point3d())), false);
	BOOST_CHECK_EQUAL(CheckTriangleValid(Triangle3d(Point3d(), Point3d(0, 0, 1), Point3d())), false);

	BOOST_CHECK_EQUAL(CheckTriangleValid(Triangle3d(Point3d(0, 1, 1), Point3d(0, 0, 1), Point3d(1, 0, 0))), true);
}

BOOST_AUTO_TEST_CASE(GeometryGetNormaleForTriangleNonNormalizedTest1)
{
	BOOST_CHECK_THROW(utils::GetNormaleForTriangleNonNormalized(Triangle3d()), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(GeometryGetNormaleForTriangleNonNormalizedTest2)
{
	const Point3d p = utils::GetNormaleForTriangleNonNormalized(Triangle3d(Point3d(1, 0, 0), Point3d(0, 0, 0), Point3d(0, 1, 0)));

	BOOST_CHECK_EQUAL(p, Point3d(0, 0, 1));
}

BOOST_AUTO_TEST_CASE(GeometryGetRotationAngle360Test1)
{
	Angle a = utils::GetRotationAngle360(Point2d(-1, 0), Point2d(0, 1), Rotation::Clockwise);

	BOOST_CHECK_EQUAL(a, Degrees(90));
}

BOOST_AUTO_TEST_CASE(GeometryGetRotationAngle360Test2)
{
	Angle a = utils::GetRotationAngle360(Point2d(-1, 0), Point2d(1, 0), Rotation::Clockwise);

	BOOST_CHECK_EQUAL(a, Degrees(180));
}

BOOST_AUTO_TEST_CASE(GeometryGetRotationAngle360Test3)
{
	Angle a = utils::GetRotationAngle360(Point2d(-1, 0), Point2d(0, -1), Rotation::Clockwise);

	BOOST_CHECK_EQUAL(a, Degrees(270));
}

BOOST_AUTO_TEST_CASE(GeometryGetRotationAngle360Test4)
{
	Angle a = utils::GetRotationAngle360(Point2d(-1, 0), Point2d(-1, 0), Rotation::Clockwise);

	BOOST_CHECK_EQUAL(a, Degrees(0));
}

BOOST_AUTO_TEST_CASE(GeometryGetRotationAngle360Test5)
{
	Angle a = utils::GetRotationAngle360(Point2d(-1, 0), Point2d(-1, 0), Rotation::AntiClockwise);

	BOOST_CHECK_EQUAL(a, Degrees(0));
}

BOOST_AUTO_TEST_CASE(GeometryGetRotationAngle360Test6)
{
	Angle a = utils::GetRotationAngle360(Point2d(-1, 0), Point2d(0, 1), Rotation::AntiClockwise);

	BOOST_CHECK_EQUAL(a, Degrees(270));
}

BOOST_AUTO_TEST_CASE(GeometryGetRotationAngle360Test7)
{
	Angle a = utils::GetRotationAngle360(Point2d(-1, 0), Point2d(1, 0), Rotation::AntiClockwise);

	BOOST_CHECK_EQUAL(a, Degrees(180));
}

BOOST_AUTO_TEST_CASE(GeometryGetRotationAngle360Test8)
{
	Angle a = utils::GetRotationAngle360(Point2d(-1, 0), Point2d(0, -1), Rotation::AntiClockwise);

	BOOST_CHECK_EQUAL(a, Degrees(90));
}

// check for a case when the func could return 360 degrees. Now it's explicitly checked.
BOOST_AUTO_TEST_CASE(GeometryGetRotationAngle360Test9)
{
	Angle a = utils::GetRotationAngle360(Point2d(2, -1.74845553e-007f), Point2d(2, 0), Rotation::Clockwise);

	BOOST_CHECK_EQUAL(a, Degrees(0));
}

