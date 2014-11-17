#include <WorldProjection.h>

#include <boost/test/unit_test.hpp>

using namespace trm;

BOOST_AUTO_TEST_CASE(WorldProjectionTestZeroAngles)
{
	WorldProjection wp;
	wp.SetRatio(4.0f / 3.0f);
	wp.SetAngles(Degrees(0), Degrees(0), Degrees(0));
	wp.SetShift(Point3d(0, 0, 100));

	BOOST_CHECK_EQUAL(Point3d(0, 0, 100), wp.GetCameraPosition());
}

BOOST_AUTO_TEST_CASE(WorldProjectionTestXRotation)
{
	WorldProjection wp;
	wp.SetRatio(4.0f / 3.0f);
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
	wp.SetRatio(4.0f / 3.0f);
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
	wp.SetRatio(4.0f / 3.0f);
	wp.SetAngles(Degrees(0), Degrees(0), Degrees(90));
	wp.SetShift(Point3d(0, 0, 100));

	BOOST_CHECK_EQUAL(Point3d(0, 0, 100), wp.GetCameraPosition());
}

BOOST_AUTO_TEST_CASE(WorldProjectionTestZRotation2)
{
	WorldProjection wp;
	wp.SetRatio(4.0f / 3.0f);
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
	wp.SetRatio(4.0f / 3.0f);
	wp.SetAngles(Degrees(0), Degrees(0), Degrees(180));
	wp.SetShift(Point3d(0, 0, 100));

	BOOST_CHECK_EQUAL(Point3d(0, 0, 100), wp.GetCameraPosition());
}

BOOST_AUTO_TEST_CASE(WorldProjectionTestXZRotation2)
{
	WorldProjection wp;
	wp.SetRatio(4.0f / 3.0f);
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
	wp.SetRatio(4.0f / 3.0f);
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
	wp.SetRatio(4.0f / 3.0f);
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
	wp.SetRatio(4.0f / 3.0f);
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
	wp.SetRatio(4.0f / 3.0f);
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
	wp.SetRatio(4.0f / 3.0f);
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
	wp.SetRatio(4.0f / 3.0f);
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
	wp.SetRatio(4.0f / 3.0f);
	wp.SetAngles(Degrees(0), Degrees(0), Degrees(-45));
	wp.SetShift(Point3d(100, 100, 100));
	
	BOOST_CHECK_EQUAL(Point3d(100, 100, 100), wp.GetCameraPosition());

	wp.Shift(10.0f, 10.0f);

	BOOST_CHECK_EQUAL(wp.GetCameraPosition().x(), 100.0f);
	BOOST_CHECK_GT(wp.GetCameraPosition().y(), 100.0f);
}
