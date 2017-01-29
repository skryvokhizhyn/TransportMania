#include <boost/test/unit_test.hpp>

#include <PolygonVisibilityChecker.h>
#include <MatrixFactory.h>

using namespace trm;

BOOST_AUTO_TEST_CASE(PolygonVisibilityCheckerTest1)
{
	// line isn't treated as visible
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

BOOST_AUTO_TEST_CASE(PolygonVisibilityCheckerTest6)
{
	BOOST_CHECK(CheckPolygonIsVisible(MatrixFactory::Identity(), {
		Point3d(-2.0f, -0.1f, 10.0f), Point3d(0.0f, -2.1f, 10.0f), 
		Point3d(0.0f, 2.0f, 10.0f), Point3d(-2.0f, -2.0f, 10.0f)}));
}

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

BOOST_AUTO_TEST_CASE(PolygonVisibilityCheckerTest11)
{
	/*BOOST_CHECK(CheckPolygonIsVisible(MatrixFactory::Identity(), {
		Point3d(-12.0f, -12.0f, 0.0f), Point3d(-10.0f, 12.0f, 0.0f),
		Point3d(10.0f, 12.0f, 0.0f), Point3d(12.0f, -12.0f, 0.0f) }));*/
}

BOOST_AUTO_TEST_CASE(PolygonVisibilityCheckerTest12)
{
	BOOST_CHECK(CheckPolygonIsVisible(MatrixFactory::Identity(), {
		Point3d(-0.2f, -0.2f, 0.0f), Point3d(-0.2f, 0.2f, 0.0f),
		Point3d(0.2f, 0.2f, 0.0f), Point3d(0.2f, -0.2f, 0.0f) }));
}

namespace
{
	Matrix GetRandomMatrix()
	{
		Matrix m;
		m.at_element(0, 0) = 0;
		m.at_element(0, 1) = 1;
		m.at_element(0, 2) = 2;
		m.at_element(0, 3) = 3;
		m.at_element(1, 0) = 4;
		m.at_element(1, 1) = 5;
		m.at_element(1, 2) = 6;
		m.at_element(1, 3) = 7;
		m.at_element(2, 0) = 8;
		m.at_element(2, 1) = 9;
		m.at_element(2, 2) = 10;
		m.at_element(2, 3) = 11;
		m.at_element(3, 0) = 12;
		m.at_element(3, 1) = 13;
		m.at_element(3, 2) = 14;
		m.at_element(3, 3) = 15;

		return m;
	}

	Matrix RANDOM_MATRIX = GetRandomMatrix();
}

BOOST_AUTO_TEST_CASE(GetConvertedPolygonTest1)
{
	auto res1 = GetConvertedPolygon4PointsAndLess(RANDOM_MATRIX,
	{
		Point3d(-0.2f, -0.2f, 0.0f), Point3d(-0.2f, 0.2f, 0.0f),
		Point3d(0.2f, 0.2f, 0.0f), Point3d(0.2f, -0.2f, 0.0f)
	});

	auto res2 = GetConvertedPolygonGeneric(RANDOM_MATRIX,
	{
		Point3d(-0.2f, -0.2f, 0.0f), Point3d(-0.2f, 0.2f, 0.0f),
		Point3d(0.2f, 0.2f, 0.0f), Point3d(0.2f, -0.2f, 0.0f)
	});

	BOOST_CHECK(boost::equal(res1, res2));
}

BOOST_AUTO_TEST_CASE(GetConvertedPolygonTest2)
{
	auto res1 = GetConvertedPolygon4PointsAndLess(RANDOM_MATRIX,
	{
		Point3d(-0.2f, -0.2f, 0.0f), Point3d(-0.2f, 0.2f, 0.0f),
		Point3d(0.2f, 0.2f, 0.0f)
	});

	auto res2 = GetConvertedPolygonGeneric(RANDOM_MATRIX,
	{
		Point3d(-0.2f, -0.2f, 0.0f), Point3d(-0.2f, 0.2f, 0.0f),
		Point3d(0.2f, 0.2f, 0.0f)
	});

	BOOST_CHECK(boost::equal(res1, res2));
}

BOOST_AUTO_TEST_CASE(GetConvertedPolygonTest3)
{
	auto res1 = GetConvertedPolygon4PointsAndLess(RANDOM_MATRIX,
	{
		Point3d(-0.2f, -0.2f, 0.0f), Point3d(-0.2f, 0.2f, 0.0f)
	});

	auto res2 = GetConvertedPolygonGeneric(RANDOM_MATRIX,
	{
		Point3d(-0.2f, -0.2f, 0.0f), Point3d(-0.2f, 0.2f, 0.0f)
	});

	BOOST_CHECK(boost::equal(res1, res2));
}

BOOST_AUTO_TEST_CASE(GetConvertedPolygonTest4)
{
	auto res1 = GetConvertedPolygon4PointsAndLess(RANDOM_MATRIX,
	{
		Point3d(-0.2f, -0.2f, 0.0f)
	});

	auto res2 = GetConvertedPolygonGeneric(RANDOM_MATRIX,
	{
		Point3d(-0.2f, -0.2f, 0.0f)
	});

	BOOST_CHECK(boost::equal(res1, res2));
}