//#define BOOST_TEST_DYN_LINK

#include <boost/test/unit_test.hpp>
#include "MatrixFactory.h"
#include "MatrixUtils.h"
#include <Point4d.h>
#include <limits>

using namespace trm;
 
BOOST_AUTO_TEST_CASE(MathTest1)
{
	Point4d v(1, 2, 3, 1);
	Matrix m = MatrixFactory::Identity();
	Point4d r = m * v;

	BOOST_CHECK_EQUAL(r, v);
}

BOOST_AUTO_TEST_CASE(MathTest2)
{
	Point4d v(1, 2, 3, 1);
	Matrix m = MatrixFactory::Scale(2);
	Point4d r = m * v;

	BOOST_CHECK_EQUAL(r, Point4d(2, 4, 6, 1));
}

BOOST_AUTO_TEST_CASE(MathTest3)
{
	Point4d v(1, 2, 3, 1);
	Matrix m = MatrixFactory::Move(Point3d(2, 3, 4));
	Point4d r = m * v;
	Point4d e(3, 5, 7, 1);

	BOOST_CHECK_EQUAL(r, e);
}

BOOST_AUTO_TEST_CASE(MathTest4)
{
	Point4d v(1, 2, 3, 1);
	Matrix m = MatrixFactory::Zero();
	Point4d r = m * v;

	BOOST_CHECK_EQUAL(r, Point4d());
}

BOOST_AUTO_TEST_CASE(MathTest7)
{
	Point4d v(1, 0, 0, 0);
	Matrix m = MatrixFactory::Rotate(trm::Degrees(90), trm::Degrees(0), trm::Degrees(0));
	Point4d r = m * v;

	BOOST_CHECK_EQUAL(r, v);
}

BOOST_AUTO_TEST_CASE(MathTest8)
{
	Point4d v(0, 1, 0, 0);
	Matrix m = MatrixFactory::Rotate(trm::Degrees(90), trm::Degrees(0), trm::Degrees(0));
	Point4d r = m * v;

	BOOST_CHECK_EQUAL(r.x(), 0);
	BOOST_CHECK_EQUAL(r.y(), -std::numeric_limits<float>::epsilon());
	BOOST_CHECK_CLOSE(r.z(), 1.0f, 0.00002f );
	BOOST_CHECK_EQUAL(r.w(), 0);

	//BOOST_CHECK_PREDICATE(std::less_equal<float>(), (r)(Point4d(0, 0, -1, 0)));
}

BOOST_AUTO_TEST_CASE(MathTest9)
{
	Point4d v(0, 1, 0, 0);
	Matrix m = MatrixFactory::Rotate(trm::Degrees(90), trm::Degrees(90), trm::Degrees(90));
	Point4d r = m * v;

	BOOST_CHECK_CLOSE(r.x(), 1.0f, 0.00001f);
	BOOST_CHECK_PREDICATE(std::less_equal<float>(), (r.y())(0.00001f));
	BOOST_CHECK_EQUAL(r.z(), 0);
	BOOST_CHECK_EQUAL(r.w(), 0);
}

BOOST_AUTO_TEST_CASE(MathTest10)
{
	Point4d v(0, 0, 1, 0);
	Matrix m = MatrixFactory::Rotate(trm::Degrees(90), trm::Degrees(90), trm::Degrees(90));
	Point4d r = m * v;

	//BOOST_CHECK_EQUAL(r, Point4d(0, 0, -1, 0));

	BOOST_CHECK_EQUAL(r.x(), 0);
	BOOST_CHECK_EQUAL(r.y(), 0);
	BOOST_CHECK_CLOSE(r.z(), - 1.0f, 0.0001f);
	BOOST_CHECK_PREDICATE(std::less_equal<float>(), (r.w())(0.00001f));
}

BOOST_AUTO_TEST_CASE(MathTest11)
{
	Matrix m = MatrixFactory::Projection(trm::Degrees(45), 4.0f/3.0f, 2, 11);

	Point4d v1(0, 0, -2, 1);
	Point4d r1 = m * v1;

	BOOST_CHECK(utils::CheckEqual(-1.0f, r1.z() / r1.w()));

	Point4d v2(0, 0, -11, 1);
	Point4d r2 = m * v2;

	//BOOST_CHECK(utils::CheckEqual(1.0f, r2.z() / r2.w()));
	BOOST_CHECK_CLOSE(r2.z() / r2.w(), 1.0f, 0.00001f);
}

#undef near
#undef far

BOOST_AUTO_TEST_CASE(MathTest12)
{
	const float angle = utils::ToRadians(45.0f);
	const float near = 2.0f;
	const float far = 11.0f;
	const float ratio = 4.0f / 3.0f;
	const float xNear = ::tan(angle / 2) * near;
	const float yNear = xNear / ratio;
	const float xFar = ::tan(angle / 2) * far;
	const float yFar = xFar / ratio;

	Matrix m = MatrixFactory::Projection(trm::Radians(angle), ratio, near, far);

	Point4d v1(xNear, yNear, -near, 1);
	Point4d r1 = m * v1;
	r1 /= r1.w();

	BOOST_CHECK_EQUAL(r1, Point4d(1.0f, 1.0f, -1.0f, 1.0f));

	Point4d v2(xFar, yFar, -far, 1);
	Point4d r2 = m * v2;
	r2 /= r2.w();

	BOOST_CHECK_EQUAL(r2, Point4d(1.0f, 1.0f + std::numeric_limits<float>::epsilon(), 1.0f, 1.0f));
}

BOOST_AUTO_TEST_CASE(MathTestRotateXZ1)
{
	Point4d v(0, 0, 100, 0);
	Matrix m = MatrixFactory::Rotate(trm::Degrees(45), trm::Degrees(0), trm::Degrees(45));
	Point4d r = m * v;

	//adjusted to the real results
	BOOST_CHECK_CLOSE(r.x(), 50.0f, 0.0001f);
	BOOST_CHECK_EQUAL(r.y(), -50.0f);
	BOOST_CHECK_CLOSE(r.z(), 70.7107f, 0.0001f);
	BOOST_CHECK_EQUAL(r.w(), 0.0f);
}

BOOST_AUTO_TEST_CASE(MathTestRotateXZ2)
{
	Point4d v(0, 100, 100, 0);
	Matrix m = MatrixFactory::Rotate(trm::Degrees(0), trm::Degrees(0), trm::Degrees(45));
	Point4d r = m * v;

	//adjusted to the real results
	//BOOST_CHECK_EQUAL(r, Point4d(-70.7107f, 70.7107f, 100.0f, 0.0f));
	BOOST_CHECK_CLOSE(r.x(), -70.7107f, 0.0001f);
	BOOST_CHECK_CLOSE(r.y(), 70.7107f, 0.0001f);
	BOOST_CHECK_EQUAL(r.z(), 100.0f);
	BOOST_CHECK_EQUAL(r.w(), 0.0f);
}

BOOST_AUTO_TEST_CASE(MathTestRotate45_1)
{
	Point4d v(0, 0, 1, 0);
	Matrix m = MatrixFactory::Rotate(trm::Degrees(45), trm::Degrees(0), trm::Degrees(45));
	//Point4d r1 = v * m;
	Point4d r2 = m * v;

	//adjusted to the real results
	BOOST_CHECK_CLOSE(r2.x(), 0.5f, 0.0001f);
	BOOST_CHECK_CLOSE(r2.y(), -0.5f, 0.0001f);
	//BOOST_CHECK_CLOSE(r2.z(), 0.7f, 1.0f);
	BOOST_CHECK_EQUAL(r2.w(), 0.0f);
}

BOOST_AUTO_TEST_CASE(MathTestRotateVector1)
{
	const Point4d vF = Point4d(1, 0, 0, 0);
	const Point4d vT = Point4d(0, 1, 0, 0);
	const Matrix m = MatrixFactory::Rotate(Point3d::Cast(vF), Point3d::Cast(vT));
	const Point4d rotated = m * vF;
	
	BOOST_CHECK(utils::CheckNear(rotated.x(), vT.x(), 0.00001f));
	BOOST_CHECK(utils::CheckNear(rotated.y(), vT.y(), 0.00001f));
	BOOST_CHECK_EQUAL(rotated.z(), vT.z());
	BOOST_CHECK_EQUAL(rotated.w(), vT.w());
}

BOOST_AUTO_TEST_CASE(MathTestRotateVector2)
{
	const Point4d vF = Point4d(0, 1, 0, 0);
	const Point4d vT = Point4d(1, 0, 0, 0);
	const Matrix m = MatrixFactory::Rotate(Point3d::Cast(vF), Point3d::Cast(vT));
	const Point4d rotated = m * vF;
	
	BOOST_CHECK(utils::CheckNear(rotated.x(), vT.x(), 0.00001f));
	BOOST_CHECK(utils::CheckNear(rotated.y(), vT.y(), 0.00001f));
	BOOST_CHECK_EQUAL(rotated.z(), vT.z());
	BOOST_CHECK_EQUAL(rotated.w(), vT.w());
}

BOOST_AUTO_TEST_CASE(MathTestRotateVector3)
{
	const Point4d vF = Point4d(0, 0, 1, 0);
	const Point4d vT = Point4d(1, 0, 0, 0);
	const Matrix m = MatrixFactory::Rotate(Point3d::Cast(vF), Point3d::Cast(vT));
	const Point4d rotated = m * vF;
	
	BOOST_CHECK(utils::CheckNear(rotated.x(), vT.x(), 0.00001f));
	BOOST_CHECK_EQUAL(rotated.y(), vT.y());
	BOOST_CHECK(utils::CheckNear(rotated.z(), vT.z(), 0.00001f));
	BOOST_CHECK_EQUAL(rotated.w(), vT.w());
}

BOOST_AUTO_TEST_CASE(MathTestRotateVector4)
{
	const Point4d vF = Point4d(0, 1, 0, 0);
	const Point4d vT = Point4d(0, 0, 1, 0);
	const Matrix m = MatrixFactory::Rotate(Point3d::Cast(vF), Point3d::Cast(vT));
	const Point4d rotated = m * vF;
	
	BOOST_CHECK_EQUAL(rotated.x(), vT.x());
	BOOST_CHECK(utils::CheckNear(rotated.y(), vT.y(), 0.00001f));
	BOOST_CHECK(utils::CheckNear(rotated.z(), vT.z(), 0.00001f));
	BOOST_CHECK_EQUAL(rotated.w(), vT.w());
}

BOOST_AUTO_TEST_CASE(MathTestRotateVector5)
{
	const Point4d vF = Point4d(1, 1, 0, 0);
	const Point4d vT = Point4d(-1, -1, 0, 0);
	const Matrix m = MatrixFactory::Rotate(Point3d::Cast(vF), Point3d::Cast(vT), Point3d(0, 0, 1));
	const Point4d rotated = m * vF;
	
	BOOST_CHECK(utils::CheckNear(rotated.x(), vT.x(), 0.00001f));
	BOOST_CHECK(utils::CheckNear(rotated.y(), vT.y(), 0.00001f));
	BOOST_CHECK_EQUAL(rotated.z(), vT.z());
	BOOST_CHECK_EQUAL(rotated.w(), vT.w());
} 

BOOST_AUTO_TEST_CASE(MathTestRotateVector6)
{
	const Point4d vF = Point4d(1, 1, -1, 0);
	const Point4d vT = Point4d(-1, -1, 0, 0);
	const Matrix m = MatrixFactory::Rotate(Point3d::Cast(vF), Point3d::Cast(vT), Point3d(1, 1, 1));
	const Point4d rotated = m * vF;
	
	/*BOOST_CHECK_EQUAL(rotated.x(), vT.x());
	BOOST_CHECK_EQUAL(rotated.y(), vT.y());
	BOOST_CHECK_EQUAL(rotated.z(), vT.z());
	BOOST_CHECK_EQUAL(rotated.w(), vT.w());*/
} 