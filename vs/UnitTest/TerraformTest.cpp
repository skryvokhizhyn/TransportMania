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

	TerraformFunction func = TerraformFunctionFactory::GetSpherical(p1, 10, 10);

	const AxisType h1 = func(p1, 0);
	const AxisType h2 = func(p2, 0);

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

	TerraformFunction func = TerraformFunctionFactory::GetSpherical(center, r, h);

	const AxisType z = func(p, 0);

	BOOST_CHECK(utils::CheckEqual(z, -h / (r * r) * d * d + h));
}

BOOST_AUTO_TEST_CASE(TerraformTest3)
{
	const float h = 1.0f;
	const float c = 2.0f;

	TerraformFunction func = TerraformFunctionFactory::GetAngleLinear(Point2d(0, 0), Point2d(2, 0), h, c);

	const AxisType z1 = func(Point2d(0, 0), 0);
	const AxisType z2 = func(Point2d(0, 1), 0);
	const AxisType z3 = func(Point2d(0, -1), 0);
	const AxisType z4 = func(Point2d(2, 0), 0);
	const AxisType z5 = func(Point2d(2, 1), 0);
	const AxisType z6 = func(Point2d(2, -1), 0);
	const AxisType z7 = func(Point2d(4, 1), 0);

	BOOST_CHECK_EQUAL(z1, h);
	BOOST_CHECK_EQUAL(z2, h);
	BOOST_CHECK_EQUAL(z3, h);

	BOOST_CHECK_EQUAL(z4, 2 * c + h);
	BOOST_CHECK_EQUAL(z5, 2 * c + h);
	BOOST_CHECK_EQUAL(z6, 2 * c + h);

	BOOST_CHECK_EQUAL(z7, 4 * c + h);
}

BOOST_AUTO_TEST_CASE(TerraformTest4)
{
	const float h = 1.0f;
	const float c = 2.0f;

	TerraformFunction func = TerraformFunctionFactory::GetAngleLinear(Point2d(0, 0), Point2d(0, 2), h, c);

	const AxisType z1 = func(Point2d(0, 0), 0);
	const AxisType z2 = func(Point2d(1, 0), 0);
	const AxisType z3 = func(Point2d(-1, 0), 0);

	const AxisType z4 = func(Point2d(0, 2), 0);
	const AxisType z5 = func(Point2d(1, 2), 0);
	const AxisType z6 = func(Point2d(-1, 2), 0);

	const AxisType z7 = func(Point2d(1, 4), 0);

	BOOST_CHECK_EQUAL(z1, h);
	BOOST_CHECK_EQUAL(z2, h);
	BOOST_CHECK_EQUAL(z3, h);

	BOOST_CHECK_EQUAL(z4, 2 * c + h);
	BOOST_CHECK_EQUAL(z5, 2 * c + h);
	BOOST_CHECK_EQUAL(z6, 2 * c + h);

	BOOST_CHECK_EQUAL(z7, 4 * c + h);
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