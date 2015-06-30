#include <boost/test/unit_test.hpp>

#include <RailRoadFactory.h>
#include <Point2d.h>
#include <RailRoadArc.h>

using namespace trm;

namespace trm
{
	std::ostream & operator << (std::ostream & o, const Rotation r)
	{
		switch (r)
		{
		case Rotation::Clockwise:
			o << "Clockwise";
			break;

		case Rotation::AntiClockwise:
			o << "AntoClockwise";
			break;
		}

		return o;
	}
}

BOOST_AUTO_TEST_CASE(RailROadFactoryTest1)
{
	RailRoadPtr rPtr = RailRoadFactory::Arc(Point3d(), Point3d(1, 0, 0), Point2d(0, 4));
	
	auto aPtr = std::dynamic_pointer_cast<RailRoadArc>(rPtr);
	
	BOOST_CHECK_EQUAL(aPtr->GetStart(), Point3d());
	BOOST_CHECK_EQUAL(aPtr->GetCenter(), Point2d(0, 2));
	BOOST_CHECK_EQUAL(aPtr->GetAngle(), Degrees(180));
	BOOST_CHECK_EQUAL(aPtr->GetRotation(), Rotation::AntiClockwise);
}

BOOST_AUTO_TEST_CASE(RailROadFactoryTest2)
{
	RailRoadPtr rPtr = RailRoadFactory::Arc(Point3d(), Point3d(0, 1, 0), Point2d(-4, 0));
	
	auto aPtr = std::dynamic_pointer_cast<RailRoadArc>(rPtr);
	
	BOOST_CHECK_EQUAL(aPtr->GetStart(), Point3d());
	BOOST_CHECK_EQUAL(aPtr->GetCenter(), Point2d(-2, 0));
	BOOST_CHECK_EQUAL(aPtr->GetAngle(), Degrees(180));
	BOOST_CHECK_EQUAL(aPtr->GetRotation(), Rotation::AntiClockwise);
}

BOOST_AUTO_TEST_CASE(RailROadFactoryTest3)
{
	RailRoadPtr rPtr = RailRoadFactory::Arc(Point3d(), Point3d(-1, 0, 0), Point2d(0, 4));
	
	auto aPtr = std::dynamic_pointer_cast<RailRoadArc>(rPtr);
	
	BOOST_CHECK_EQUAL(aPtr->GetStart(), Point3d());
	BOOST_CHECK_EQUAL(aPtr->GetCenter(), Point2d(0, 2));
	BOOST_CHECK_EQUAL(aPtr->GetAngle(), Degrees(180));
	BOOST_CHECK_EQUAL(aPtr->GetRotation(), Rotation::Clockwise);
}

BOOST_AUTO_TEST_CASE(RailROadFactoryTest4)
{
	RailRoadPtr rPtr = RailRoadFactory::Arc(Point3d(1, 1, 0), Point3d(1, 1, 0), Point2d(-3, 5));
	
	auto aPtr = std::dynamic_pointer_cast<RailRoadArc>(rPtr);
	
	BOOST_CHECK_EQUAL(aPtr->GetStart(), Point3d(1, 1, 0));
	BOOST_CHECK_EQUAL(aPtr->GetCenter(), Point2d(-1, 3));
	BOOST_CHECK_EQUAL(aPtr->GetAngle(), Degrees(180));
	BOOST_CHECK_EQUAL(aPtr->GetRotation(), Rotation::AntiClockwise);
}

// conversion from 'double' to 'const float'
#pragma warning(disable: 4244)

BOOST_AUTO_TEST_CASE(RailROadFactoryTest5)
{
	RailRoadPtr rPtr = RailRoadFactory::Arc(Point3d(std::sqrt(2)/2, std::sqrt(2)/2, 0.0), Point3d(1, 1, 0), Point2d(-std::sqrt(2)/2, std::sqrt(2)/2));
	
	auto aPtr = std::dynamic_pointer_cast<RailRoadArc>(rPtr);
	
	BOOST_CHECK_EQUAL(aPtr->GetCenter(), Point2d(0, std::sqrt(2)));
	BOOST_CHECK_EQUAL(aPtr->GetAngle(), Degrees(270));
	BOOST_CHECK_EQUAL(aPtr->GetRotation(), Rotation::AntiClockwise);
}

BOOST_AUTO_TEST_CASE(RailROadFactoryTest6)
{
	RailRoadPtr rPtr = RailRoadFactory::Arc(Point3d(std::sqrt(2)/2, std::sqrt(2)/2, 0.0f), Point3d(-1, -1, 0), Point2d(-std::sqrt(2)/2, std::sqrt(2)/2));
	
	auto aPtr = std::dynamic_pointer_cast<RailRoadArc>(rPtr);
	
	BOOST_CHECK_EQUAL(aPtr->GetCenter(), Point2d(0, std::sqrt(2)));
	BOOST_CHECK_EQUAL(aPtr->GetAngle(), Degrees(90));
	BOOST_CHECK_EQUAL(aPtr->GetRotation(), Rotation::Clockwise);
}

BOOST_AUTO_TEST_CASE(RailROadFactoryTest7)
{
	RailRoadPtr rPtr = RailRoadFactory::Arc(Point3d(-std::sqrt(2)/2, -std::sqrt(2)/2, 0.0f), Point3d(1, -1, 0), Point2d(-std::sqrt(2)/2, std::sqrt(2)/2));
	
	auto aPtr = std::dynamic_pointer_cast<RailRoadArc>(rPtr);
	
	BOOST_CHECK_EQUAL(aPtr->GetCenter(), Point2d(0, 0));
	BOOST_CHECK_EQUAL(aPtr->GetAngle(), Degrees(270));
	BOOST_CHECK_EQUAL(aPtr->GetRotation(), Rotation::AntiClockwise);
}

BOOST_AUTO_TEST_CASE(RailROadFactoryTest8)
{
	RailRoadPtr rPtr = RailRoadFactory::Arc(Point3d(-std::sqrt(2)/2, -std::sqrt(2)/2, 0.0f), Point3d(-1, 1, 0), Point2d(-std::sqrt(2)/2, std::sqrt(2)/2));
	
	auto aPtr = std::dynamic_pointer_cast<RailRoadArc>(rPtr);
	
	BOOST_CHECK_EQUAL(aPtr->GetCenter(), Point2d(0, 0));
	BOOST_CHECK_EQUAL(aPtr->GetAngle(), Degrees(90));
	BOOST_CHECK_EQUAL(aPtr->GetRotation(), Rotation::Clockwise);
}

BOOST_AUTO_TEST_CASE(RailROadFactoryTest9)
{
	BOOST_CHECK_THROW(RailRoadFactory::Arc(Point3d(-std::sqrt(2)/2, -std::sqrt(2)/2, 0.0f), Point3d(-1, 1, 0), Point2d(-std::sqrt(2)/2, -std::sqrt(2)/2)), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(RailROadFactoryTest10)
{
	BOOST_CHECK_THROW(RailRoadFactory::Arc(Point3d(-std::sqrt(2)/2, -std::sqrt(2)/2, 0.0f), Point3d(), Point2d(-std::sqrt(2)/2, -std::sqrt(2)/2)), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(RailRoadFactoryTest11)
{
	BOOST_CHECK_THROW(RailRoadFactory::Arc(Point3d(), Degrees(0), Point2d(1, 1), Rotation::AntiClockwise), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(RailRoadFactoryTest12)
{
	BOOST_CHECK_THROW(RailRoadFactory::Arc(Point3d(), Degrees(1), Point2d(), Rotation::AntiClockwise), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(RailRoadFactoryTest13)
{
	BOOST_CHECK_THROW(RailRoadFactory::Line(Point3d(), Point3d()), std::runtime_error);
}