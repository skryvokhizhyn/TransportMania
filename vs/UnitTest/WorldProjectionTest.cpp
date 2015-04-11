#include <WorldProjection.h>

#include <boost/test/unit_test.hpp>

using namespace trm;

BOOST_AUTO_TEST_CASE(WorldProjectionTestZeroAngles)
{
	WorldProjection wp;
	wp.SetRatio(4, 3);
	wp.SetAngles(Degrees(0), Degrees(0), Degrees(0));
	wp.SetShift(Point3d(0, 0, 100));

	BOOST_CHECK_EQUAL(Point3d(0, 0, 100), wp.GetCameraPosition());
}

BOOST_AUTO_TEST_CASE(WorldProjectionTestXRotation)
{
	WorldProjection wp;
	wp.SetRatio(4, 3);
	wp.SetAngles(Degrees(90), Degrees(0), Degrees(0));
	wp.SetShift(Point3d(0, 0, 100));

	const Point3d & c = wp.GetCameraPosition();

	//BOOST_CHECK_EQUAL(Point3d(0, -100, 0), wp.GetCameraPosition());
	BOOST_CHECK_EQUAL(c.x(), 0.0f);
	BOOST_CHECK_CLOSE(c.y(), -100.0f, 0.0001f);
	BOOST_CHECK_PREDICATE(std::less_equal<float>(), (c.z())(0.00001f));
}

BOOST_AUTO_TEST_CASE(WorldProjectionTestYRotation)
{
	WorldProjection wp;
	wp.SetRatio(4, 3);
	wp.SetAngles(Degrees(0), Degrees(90), Degrees(0));
	wp.SetShift(Point3d(0, 0, 100));

	const Point3d & c = wp.GetCameraPosition();

	//BOOST_CHECK_EQUAL(Point3d(100, 0, 0), wp.GetCameraPosition());
	BOOST_CHECK_CLOSE(c.x(), 100.0f, 0.0001f);
	BOOST_CHECK_EQUAL(c.y(), 0.0f);
	BOOST_CHECK_PREDICATE(std::less_equal<float>(), (c.z())(0.00001f));
}

BOOST_AUTO_TEST_CASE(WorldProjectionTestZRotation1)
{
	WorldProjection wp;
	wp.SetRatio(4, 3);
	wp.SetAngles(Degrees(0), Degrees(0), Degrees(90));
	wp.SetShift(Point3d(0, 0, 100));

	BOOST_CHECK_EQUAL(Point3d(0, 0, 100), wp.GetCameraPosition());
}

BOOST_AUTO_TEST_CASE(WorldProjectionTestZRotation2)
{
	WorldProjection wp;
	wp.SetRatio(4, 3);
	wp.SetAngles(Degrees(0), Degrees(0), Degrees(90));
	wp.SetShift(Point3d(0, 100, 0));

	const Point3d & c = wp.GetCameraPosition();

	//BOOST_CHECK_EQUAL(Point3d(-100, 0, 0), wp.GetCameraPosition());
	BOOST_CHECK_EQUAL(c.x(), 0.0f);
	BOOST_CHECK_EQUAL(c.y(), 100.0f);
	BOOST_CHECK_EQUAL(c.z(), 0.0f);
}

BOOST_AUTO_TEST_CASE(WorldProjectionTestXZRotation1)
{
	WorldProjection wp;
	wp.SetRatio(4, 3);
	wp.SetAngles(Degrees(0), Degrees(0), Degrees(180));
	wp.SetShift(Point3d(0, 0, 100));

	BOOST_CHECK_EQUAL(Point3d(0, 0, 100), wp.GetCameraPosition());
}

BOOST_AUTO_TEST_CASE(WorldProjectionTestXZRotation2)
{
	WorldProjection wp;
	wp.SetRatio(4, 3);
	wp.SetAngles(Degrees(90), Degrees(0), Degrees(180));
	wp.SetShift(Point3d(0, 0, 100));

	const Point3d & c = wp.GetCameraPosition();

	//BOOST_CHECK_EQUAL(Point3d(0, -100, 0), wp.GetCameraPosition());
	BOOST_CHECK_PREDICATE(std::less_equal<float>(), (c.x())(0.00001f));
	BOOST_CHECK_CLOSE(c.y(), 100.0f, 0.0001f);
	BOOST_CHECK_PREDICATE(std::less_equal<float>(), (c.z())(0.00001f));
}

BOOST_AUTO_TEST_CASE(WorldProjectionTestXZRotation3)
{
	WorldProjection wp;
	wp.SetRatio(4, 3);
	wp.SetAngles(Degrees(90), Degrees(0), Degrees(180));
	wp.SetShift(Point3d(100, 100, 100));
	
	const Point3d & c = wp.GetCameraPosition();

	//BOOST_CHECK_EQUAL(Point3d(0, 100, 0), wp.GetCameraPosition());
	BOOST_CHECK_CLOSE(c.x(), 100.0f, 0.0001f);
	BOOST_CHECK_CLOSE(c.y(), 200.0f, 0.0001f);
	BOOST_CHECK_PREDICATE(std::less_equal<float>(), (c.z())(0.00001f));
}

BOOST_AUTO_TEST_CASE(WorldProjectionReversedMoveTest1)
{
	WorldProjection wp;
	wp.SetRatio(4, 3);
	wp.SetAngles(Degrees(0), Degrees(0), Degrees(0));
	wp.SetShift(Point3d(100, 100, 100));
	
	BOOST_CHECK_EQUAL(Point3d(100, 100, 100), wp.GetCameraPosition());

	wp.Shift(10.0f, 10.0f);

	BOOST_CHECK_GT(wp.GetCameraPosition().x(), 100.0f);
	BOOST_CHECK_GT(wp.GetCameraPosition().y(), 100.0f);
}

BOOST_AUTO_TEST_CASE(WorldProjectionReversedMoveTest2)
{
	WorldProjection wp;
	wp.SetRatio(4, 3);
	wp.SetAngles(Degrees(0), Degrees(0), Degrees(180));
	wp.SetShift(Point3d(100, 100, 100));
	
	BOOST_CHECK_EQUAL(Point3d(100, 100, 100), wp.GetCameraPosition());

	wp.Shift(10.0f, 10.0f);

	BOOST_CHECK_LT(wp.GetCameraPosition().x(), 100.0f);
	BOOST_CHECK_LT(wp.GetCameraPosition().y(), 100.0f);
}

BOOST_AUTO_TEST_CASE(WorldProjectionReversedMoveTest3)
{
	WorldProjection wp;
	wp.SetRatio(4, 3);
	wp.SetAngles(Degrees(0), Degrees(0), Degrees(90));
	wp.SetShift(Point3d(100, 100, 100));
	
	BOOST_CHECK_EQUAL(Point3d(100, 100, 100), wp.GetCameraPosition());

	wp.Shift(10.0f, 10.0f);

	BOOST_CHECK_LT(wp.GetCameraPosition().x(), 100.0f);
	BOOST_CHECK_GT(wp.GetCameraPosition().y(), 100.0f);
}

BOOST_AUTO_TEST_CASE(WorldProjectionReversedMoveTest4)
{
	WorldProjection wp;
	wp.SetRatio(4, 3);
	wp.SetAngles(Degrees(0), Degrees(0), Degrees(270));
	wp.SetShift(Point3d(100, 100, 100));
	
	BOOST_CHECK_EQUAL(Point3d(100, 100, 100), wp.GetCameraPosition());

	wp.Shift(10.0f, 10.0f);

	BOOST_CHECK_GT(wp.GetCameraPosition().x(), 100.0f);
	BOOST_CHECK_LT(wp.GetCameraPosition().y(), 100.0f);
}


BOOST_AUTO_TEST_CASE(WorldProjectionReversedMoveTest5)
{
	WorldProjection wp;
	wp.SetRatio(4, 3);
	wp.SetAngles(Degrees(0), Degrees(0), Degrees(45));
	wp.SetShift(Point3d(100, 100, 100));
	
	BOOST_CHECK_EQUAL(Point3d(100, 100, 100), wp.GetCameraPosition());

	wp.Shift(10.0f, 10.0f);

	BOOST_CHECK_GE(wp.GetCameraPosition().x(), 100.0f);
	BOOST_CHECK_GE(wp.GetCameraPosition().y(), 100.0f);
}

BOOST_AUTO_TEST_CASE(WorldProjectionReversedMoveTest6)
{
	WorldProjection wp;
	wp.SetRatio(4, 3);
	wp.SetAngles(Degrees(0), Degrees(0), Degrees(-45));
	wp.SetShift(Point3d(100, 100, 100));
	
	BOOST_CHECK_EQUAL(Point3d(100, 100, 100), wp.GetCameraPosition());

	wp.Shift(10.0f, 10.0f);

	BOOST_CHECK_GT(wp.GetCameraPosition().x(), 100.0f);
	BOOST_CHECK_EQUAL(wp.GetCameraPosition().y(), 100.0f);
}

#include <MatrixUtils.h>

BOOST_AUTO_TEST_CASE(PointIn3DTest1)
{
	WorldProjection wp;
	wp.SetRatio(4, 3);
	wp.SetAngles(Degrees(20), Degrees(30), Degrees(-45));
	wp.SetShift(Point3d(100, 100, 100));

	const Point3d p(200, 300, 400);

	const Matrix & pvm = wp.GetProjectionViewMatrix();

	const Point3d projectedP = pvm * p;

	const Matrix inverted = trm::Inverse(pvm);
 
	const Point3d invertedP = inverted * projectedP;

	BOOST_CHECK_CLOSE(p.x(), invertedP.x(), 0.001f);
	BOOST_CHECK_CLOSE(p.y(), invertedP.y(), 0.001f);
	BOOST_CHECK_CLOSE(p.z(), invertedP.z(), 0.001f);
}

#include "Point2d.h"

BOOST_AUTO_TEST_CASE(PointIn3DTest2)
{
	WorldProjection wp;
	wp.SetRatio(40, 3);
	wp.SetShift(Point3d(0, 0, 100));

	const Point3d p = wp.ToWorldCoordinates(Point2d(0, 0));

	const Point3d & c = wp.GetCameraPosition();

	BOOST_CHECK_EQUAL(p, Point3d(0, 0, 99));
	BOOST_CHECK_EQUAL(c, Point3d(0, 0, 100));
}

#include <boost/units/cmath.hpp>

BOOST_AUTO_TEST_CASE(PointIn3DTest3)
{
	WorldProjection wp;
	size_t x_s = 4;
	size_t y_s = 3;
	wp.SetRatio(x_s, y_s);
	wp.SetShift(Point3d(0, 0, 100));

	const Point3d p1 = wp.ToWorldCoordinates(Point2d(1, 0));
	const Point3d p2 = wp.ToWorldCoordinates(Point2d(0, 1));

	const Angle a = Degrees(45);
	const float x = boost::units::tan(a / 2.0f);

	//-1000-1.00018
	//-900- 1
	//-500- 0.998665
	//-100- 0.991992
	//-10 - 0.983802
	// 00 - 0.981982
	// 10 - 0.979758
	// 20 - 0.976977
	// 30 - 0.973402
	// 99 --0.999992

	// 0.414212, 0.310659

	BOOST_CHECK_EQUAL(p1.z(), 99.0f);
	BOOST_CHECK_CLOSE(p1.x(), x, 0.001f);
	BOOST_CHECK_CLOSE(p2.y(), x / x_s * y_s, 0.001f);
}

