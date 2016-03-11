#include <boost/test/unit_test.hpp>

#include <RailRoadFactory.h>
#include <Point2d.h>
#include <RailRoadArc.h>

using namespace trm;

BOOST_AUTO_TEST_CASE(RailRoadFactoryTest1)
{
	RailRoadPtr rPtr = RailRoadFactory::Arc(Point3d(), Point3d(1, 0, 0), Point2d(0, 4));
	
	auto aPtr = std::dynamic_pointer_cast<RailRoadArc>(rPtr);
	
	BOOST_CHECK_EQUAL(aPtr->GetStart(), Point3d());
	BOOST_CHECK_EQUAL(aPtr->GetCenter(), Point2d(0, 2));
	BOOST_CHECK_EQUAL(aPtr->GetAngle(), Degrees(180));
}

BOOST_AUTO_TEST_CASE(RailRoadFactoryTest2)
{
	RailRoadPtr rPtr = RailRoadFactory::Arc(Point3d(), Point3d(0, 1, 0), Point2d(-4, 0));
	
	auto aPtr = std::dynamic_pointer_cast<RailRoadArc>(rPtr);
	
	BOOST_CHECK_EQUAL(aPtr->GetStart(), Point3d());
	BOOST_CHECK_EQUAL(aPtr->GetCenter(), Point2d(-2, 0));
	BOOST_CHECK_EQUAL(aPtr->GetAngle(), Degrees(180));
}

BOOST_AUTO_TEST_CASE(RailRoadFactoryTest3)
{
	RailRoadPtr rPtr = RailRoadFactory::Arc(Point3d(0, 0, 0), Point3d(-1, 0, 0), Point2d(0, 4));
	
	auto aPtr = std::dynamic_pointer_cast<RailRoadArc>(rPtr);
	
	BOOST_CHECK_EQUAL(aPtr->GetStart(), Point3d());
	BOOST_CHECK_EQUAL(aPtr->GetCenter(), Point2d(0, 2));
	BOOST_CHECK_EQUAL(aPtr->GetAngle(), Degrees(-180));
}

BOOST_AUTO_TEST_CASE(RailRoadFactoryTest4)
{
	RailRoadPtr rPtr = RailRoadFactory::Arc(Point3d(1, 1, 0), Point3d(1, 1, 0), Point2d(-3, 5));
	
	auto aPtr = std::dynamic_pointer_cast<RailRoadArc>(rPtr);
	
	BOOST_CHECK_EQUAL(aPtr->GetStart(), Point3d(1, 1, 0));
	BOOST_CHECK_EQUAL(aPtr->GetCenter(), Point2d(-1, 3));
	BOOST_CHECK_EQUAL(aPtr->GetAngle(), Degrees(180));
}

// conversion from 'double' to 'const float'
#pragma warning(disable: 4244)

BOOST_AUTO_TEST_CASE(RailRoadFactoryTest5)
{
	RailRoadPtr rPtr = RailRoadFactory::Arc(Point3d(std::sqrt(2)/2, std::sqrt(2)/2, 0.0), Point3d(1, 1, 0), Point2d(-std::sqrt(2)/2, std::sqrt(2)/2));
	
	auto aPtr = std::dynamic_pointer_cast<RailRoadArc>(rPtr);
	
	BOOST_CHECK_EQUAL(aPtr->GetCenter(), Point2d(0, std::sqrt(2)));
	BOOST_CHECK_EQUAL(aPtr->GetAngle(), Degrees(270));
}

BOOST_AUTO_TEST_CASE(RailRoadFactoryTest6)
{
	RailRoadPtr rPtr = RailRoadFactory::Arc(Point3d(std::sqrt(2)/2, std::sqrt(2)/2, 0.0f), Point3d(-1, -1, 0), Point2d(-std::sqrt(2)/2, std::sqrt(2)/2));
	
	auto aPtr = std::dynamic_pointer_cast<RailRoadArc>(rPtr);
	
	BOOST_CHECK_EQUAL(aPtr->GetCenter(), Point2d(0, std::sqrt(2)));
	BOOST_CHECK_EQUAL(aPtr->GetAngle(), Degrees(-90));
}

BOOST_AUTO_TEST_CASE(RailRoadFactoryTest7)
{
	RailRoadPtr rPtr = RailRoadFactory::Arc(Point3d(-std::sqrt(2)/2, -std::sqrt(2)/2, 0.0f), Point3d(1, -1, 0), Point2d(-std::sqrt(2)/2, std::sqrt(2)/2));
	
	auto aPtr = std::dynamic_pointer_cast<RailRoadArc>(rPtr);
	
	BOOST_CHECK_EQUAL(aPtr->GetCenter(), Point2d(0, 0));
	BOOST_CHECK_EQUAL(aPtr->GetAngle(), Degrees(270));
}

BOOST_AUTO_TEST_CASE(RailRoadFactoryTest8)
{
	RailRoadPtr rPtr = RailRoadFactory::Arc(Point3d(-std::sqrt(2)/2, -std::sqrt(2)/2, 0.0f), Point3d(-1, 1, 0), Point2d(-std::sqrt(2)/2, std::sqrt(2)/2));
	
	auto aPtr = std::dynamic_pointer_cast<RailRoadArc>(rPtr);
	
	BOOST_CHECK_EQUAL(aPtr->GetCenter(), Point2d(0, 0));
	BOOST_CHECK_EQUAL(aPtr->GetAngle(), Degrees(-90));
}

BOOST_AUTO_TEST_CASE(RailRoadFactoryTest9)
{
	BOOST_CHECK_THROW(RailRoadFactory::Arc(Point3d(-std::sqrt(2)/2, -std::sqrt(2)/2, 0.0f), Point3d(-1, 1, 0), Point2d(-std::sqrt(2)/2, -std::sqrt(2)/2)), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(RailRoadFactoryTest10)
{
	BOOST_CHECK_THROW(RailRoadFactory::Arc(Point3d(-std::sqrt(2)/2, -std::sqrt(2)/2, 0.0f), Point3d(), Point2d(-std::sqrt(2)/2, -std::sqrt(2)/2)), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(RailRoadFactoryTest11)
{
	BOOST_CHECK_THROW(RailRoadFactory::Arc(Point3d(), Degrees(0), Point2d(1, 1)), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(RailRoadFactoryTest12)
{
	BOOST_CHECK_THROW(RailRoadFactory::Arc(Point3d(), Degrees(1), Point2d()), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(RailRoadFactoryTest13)
{
	BOOST_CHECK_THROW(RailRoadFactory::Line(Point3d(), Point3d()), std::runtime_error);
}
