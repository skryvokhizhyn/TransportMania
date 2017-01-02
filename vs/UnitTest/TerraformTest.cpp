//#define BOOST_TEST_DYN_LINK

#include <TerraformFunction.h>
#include <Point3d.h>
#include <Point2d.h>
#include <Size2d.h>
#include <CommonUtils.h>
#include <Terraformer.h>
#include <boost/test/unit_test.hpp>

using namespace trm;

namespace trm
{
	bool operator == (const TerrainRange::Range & r1, const TerrainRange::Range & r2)
	{
		return
			r1.y == r2.y &&
			r1.xBegin == r2.xBegin &&
			r1.xEnd == r2.xEnd;
	}
}

BOOST_AUTO_TEST_CASE(TerraformTest1)
{
	const Point2d p1(0, 0);
	const Point2d p2(0, 100);

	TerraformFunctionPtr func = TerraformFunctionFactory::GetSpherical(p1, 10, 10);

	AxisType h1 = 0.0f;
	(*func)(p1, h1);
	AxisType h2 = 0.0f;
	(*func)(p2, h2);

	BOOST_CHECK(utils::CheckEqual(h1, 10.0f));
	BOOST_CHECK(utils::CheckEqual(h2, 0.0f));
}

BOOST_AUTO_TEST_CASE(TerraformTest2)
{
	const float h = 10.0f;
	const AxisType r = 10;
	const Point2d center(0, 0);
	const AxisType d = 5;
	const Point2d p(0, d);

	auto func = TerraformFunctionFactory::GetSpherical(center, r, h);

	AxisType z = 0.0f;
	(*func)(p, z);

	BOOST_CHECK(utils::CheckEqual(z, -h / (r * r) * d * d + h));
}

BOOST_AUTO_TEST_CASE(TerraformTest3)
{
	auto func = TerraformFunctionFactory::GetLinear(Point3d(0, 0, 1), Point3d(2, 0, 2));

	AxisType z1 = 0.0f;
	(*func)(Point2d(0, 0), z1);
	AxisType z2 = 0.0f;
	(*func)(Point2d(0, 1), z2);
	AxisType z3 = 0.0f;
	(*func)(Point2d(0, -1), z3);

	AxisType z4 = 2.0f;
	(*func)(Point2d(2, 0), z4);
	AxisType z5 = 2.0f;
	(*func)(Point2d(2, 1), z5);
	AxisType z6 = 2.0f;
	(*func)(Point2d(2, -1), z6);

	AxisType z7 = 3.0f;
	(*func)(Point2d(4, 1), z7);

	BOOST_CHECK_EQUAL(z1, 1);
	BOOST_CHECK_EQUAL(z2, 1);
	BOOST_CHECK_EQUAL(z3, 1);

	BOOST_CHECK_EQUAL(z4, 2);
	BOOST_CHECK_EQUAL(z5, 2);
	BOOST_CHECK_EQUAL(z6, 2);

	BOOST_CHECK_EQUAL(z7, 3);
}

BOOST_AUTO_TEST_CASE(TerraformTest4)
{
	auto func = TerraformFunctionFactory::GetLinear(Point3d(0, 0, 1), Point3d(0, 2, 2));

	AxisType z1 = 0.0f;
	(*func)(Point2d(0, 0), z1);
	AxisType z2 = 0.0f;
	(*func)(Point2d(1, 0), z2);
	AxisType z3 = 0.0f;
	(*func)(Point2d(-1, 0), z3);

	AxisType z4 = 0.0f;
	(*func)(Point2d(0, 2), z4);
	AxisType z5 = 0.0f;
	(*func)(Point2d(1, 2), z5);
	AxisType z6 = 0.0f;
	(*func)(Point2d(-1, 2), z6);

	AxisType z7 = 0.0f;
	(*func)(Point2d(1, 4), z7);

	BOOST_CHECK_EQUAL(z1, 1);
	BOOST_CHECK_EQUAL(z2, 1);
	BOOST_CHECK_EQUAL(z3, 1);

	BOOST_CHECK_EQUAL(z4, 2);
	BOOST_CHECK_EQUAL(z5, 2);
	BOOST_CHECK_EQUAL(z6, 2);

	BOOST_CHECK_EQUAL(z7, 3);
}

BOOST_AUTO_TEST_CASE(TerraformNormalizeTest1)
{
	TerrainRange::Range r(0, 0, 1);

	BOOST_CHECK(Terraformer::Normalized(Size2d(10, 10), r));
	BOOST_CHECK_EQUAL(r, TerrainRange::Range(0, 0, 1));
}

BOOST_AUTO_TEST_CASE(TerraformNormalizeTest2)
{
	TerrainRange::Range r(0, 11, 12);

	BOOST_CHECK(!Terraformer::Normalized(Size2d(10, 10), r));
}

BOOST_AUTO_TEST_CASE(TerraformNormalizeTest3)
{
	TerrainRange::Range r(10, 0, 1);

	BOOST_CHECK(!Terraformer::Normalized(Size2d(10, 10), r));
}

BOOST_AUTO_TEST_CASE(TerraformNormalizeTest4)
{
	TerrainRange::Range r(10, 10, 11);

	BOOST_CHECK(!Terraformer::Normalized(Size2d(10, 10), r));
}

BOOST_AUTO_TEST_CASE(TerraformNormalizeTest5)
{
	TerrainRange::Range r(3, 0, 12);

	BOOST_CHECK(Terraformer::Normalized(Size2d(10, 10), r));
	BOOST_CHECK_EQUAL(r, TerrainRange::Range(3, 0, 9));
}

BOOST_AUTO_TEST_CASE(TerraformNormalizeTest6)
{
	TerrainRange::Range r(3, -2, 12);

	BOOST_CHECK(Terraformer::Normalized(Size2d(10, 10), r));
	BOOST_CHECK_EQUAL(r, TerrainRange::Range(3, 0, 9));
}

BOOST_AUTO_TEST_CASE(TerraformFunctionSpiralStartTest1)
{
	auto tf = TerraformFunctionFactory::GetSpiral(Spiral3d(Point3d(1, 1, 1), Degrees(90), Point2d(0, 0), 1.0f));
	float z = 0.0f;
	BOOST_CHECK((*tf)(Point2d(1, 1), z));
	BOOST_CHECK_EQUAL(z, 1.0f);
}

BOOST_AUTO_TEST_CASE(TerraformFunctionSpiralEndTest1)
{
	auto tf = TerraformFunctionFactory::GetSpiral(Spiral3d(Point3d(1, 0, 1), Degrees(90), Point2d(0, 0), 1.0f));
	float z = 0.0f;
	BOOST_CHECK((*tf)(Point2d(0, 1), z));
	BOOST_CHECK_EQUAL(z, 2.0f);
}

BOOST_AUTO_TEST_CASE(TerraformFunctionSpiralEndNegativeAngleTest1)
{
	auto tf = TerraformFunctionFactory::GetSpiral(Spiral3d(Point3d(1, 0, 1), Degrees(-90), Point2d(0, 0), 1.0f));
	float z = 0.0f;
	BOOST_CHECK((*tf)(Point2d(0, -1), z));
	BOOST_CHECK_EQUAL(z, 2.0f);
}

BOOST_AUTO_TEST_CASE(TerraformFunctionSpiralEndNegativeAngleTest2)
{
	auto tf = TerraformFunctionFactory::GetSpiral(Spiral3d(Point3d(1, 0, 1), Degrees(-90), Point2d(0, 0), -1.0f));
	float z = 0.0f;
	BOOST_CHECK((*tf)(Point2d(0, -1), z));
	BOOST_CHECK_EQUAL(z, 0.0f);
}

BOOST_AUTO_TEST_CASE(TerraformFunctionSpiralMiddleTest1)
{
	auto tf = TerraformFunctionFactory::GetSpiral(Spiral3d(Point3d(1, 0, 1), Degrees(180), Point2d(0, 0), 1.0f));
	float z = 0.0f;
	BOOST_CHECK((*tf)(Point2d(0, 1), z));
	BOOST_CHECK_EQUAL(z, 1.5f);
}

BOOST_AUTO_TEST_CASE(TerraformFunctionSpiralBugTest1)
{
	auto tf = TerraformFunctionFactory::GetSpiral(Spiral3d(Point3d(31.0f, 33.0f, 7.039f), Degrees(-61), Point2d(29.77f, 5.98f), 3.414f));
	float z = 0.0f;
	BOOST_CHECK((*tf)(Point2d(31, 33), z));
	BOOST_CHECK_LT(z, 8.0f);
	BOOST_CHECK((*tf)(Point2d(31, 34), z));
	BOOST_CHECK_LT(z, 8.0f);
	BOOST_CHECK((*tf)(Point2d(31, 32), z));
	BOOST_CHECK_LT(z, 8.0f);
}
