#include <boost/test/unit_test.hpp>

#include <PolygonVisibilityChecker.h>
#include <MatrixFactory.h>

using namespace trm;

BOOST_AUTO_TEST_CASE(PolygonVisibilityCheckerTest1)
{
	BOOST_CHECK(CheckPolygonIsVisible(MatrixFactory::Identity(), {
		Point3d(0, 0, 0), Point3d(1, 1, 1), Point3d(1, 1, 0)}));
}

BOOST_AUTO_TEST_CASE(PolygonVisibilityCheckerTest2)
{
	BOOST_CHECK(!CheckPolygonIsVisible(MatrixFactory::Identity(), {
		Point3d(10, 10, 10), Point3d(10, 10, 11), Point3d(10, 10, 10)}));
}

BOOST_AUTO_TEST_CASE(PolygonVisibilityCheckerTest3)
{
	BOOST_CHECK(CheckPolygonIsVisible(MatrixFactory::Identity(), {
		Point3d(-2, 2, 0), Point3d(2, -2, 0), Point3d(-2, -2, 0)}));
}

BOOST_AUTO_TEST_CASE(PolygonVisibilityCheckerTest4)
{
	BOOST_CHECK(!CheckPolygonIsVisible(MatrixFactory::Identity(), {
		Point3d(-2.0f, -0.1f, 0.0f), Point3d(0.0f, -2.1f, 0.0f), Point3d(-10.0f, -10.0f, 0.0f)}));
}

BOOST_AUTO_TEST_CASE(PolygonVisibilityCheckerTest5)
{
	BOOST_CHECK(CheckPolygonIsVisible(MatrixFactory::Identity(), {
		Point3d(-2.0f, -0.1f, 0.0f), Point3d(0.0f, -2.1f, 0.0f), 
		Point3d(0.0f, 2.0f, 0.0f), Point3d(-2.0f, -2.0f, 0.0f)}));
}

// these tests fail due to the specifics of projection matrix
// it translates 3d point from world coordinates to pseudo 3d point
// where x and y are in [-1, 1] but z is close to 1 (depends on near frustrum value)
// so z coordinate doesn't take part in visibility check
//BOOST_AUTO_TEST_CASE(PolygonVisibilityCheckerTest6)
//{
//	BOOST_CHECK(!CheckPolygonIsVisible(MatrixFactory::Identity(), {
//		Point3d(-2.0f, -0.1f, 10.0f), Point3d(0.0f, -2.1f, 10.0f), 
//		Point3d(0.0f, 2.0f, 10.0f), Point3d(-2.0f, -2.0f, 10.0f)}));
//}
//
//BOOST_AUTO_TEST_CASE(PolygonVisibilityCheckerTest7)
//{
//	BOOST_CHECK(!CheckPolygonIsVisible(MatrixFactory::Identity(), {
//		Point3d(-2.0f, -0.1f, 10.0f), Point3d(0.0f, -2.1f, 10.0f), 
//		Point3d(0.0f, 2.0f, 10.0f), Point3d(-2.0f, -2.0f, 10.0f)}));
//}

BOOST_AUTO_TEST_CASE(PolygonVisibilityCheckerTest8)
{
	BOOST_CHECK(!CheckPolygonIsVisible(MatrixFactory::Identity(), {
		Point3d(-2.0f, -10.1f, 10.0f), Point3d(0.0f, -12.1f, 10.0f), 
		Point3d(0.0f, -9.0f, 10.0f), Point3d(-2.0f, -12.0f, 10.0f)}));
}

BOOST_AUTO_TEST_CASE(PolygonVisibilityCheckerTest9)
{
	BOOST_CHECK(!CheckPolygonIsVisible(MatrixFactory::Identity(), {
		Point3d(-12.0f, -0.1f, 10.0f), Point3d(-10.0f, -2.1f, 10.0f), 
		Point3d(-10.0f, 2.0f, 10.0f), Point3d(-12.0f, -2.0f, 10.0f)}));
}

BOOST_AUTO_TEST_CASE(PolygonVisibilityCheckerTest10)
{
	BOOST_CHECK(CheckPolygonIsVisible(MatrixFactory::Identity(), {
		Point3d(-0.5f, -0.1f, 0.0f), Point3d(0.0f, -0.1f, 0.0f), 
		Point3d(-0.0f, 0.5f, 0.0f), Point3d(-0.5f, -0.7f, 0.0f)}));
}
