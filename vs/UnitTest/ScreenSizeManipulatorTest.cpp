#include <boost/test/unit_test.hpp>

#include <ScreenSizeManipulator.h>

using namespace trm;

BOOST_AUTO_TEST_CASE(ScreenPointToOglCoordinatesTest1)
{
	ScreenSizeManipulator s;
	s.SetScreenSize(Size2d(200, 100));

	BOOST_CHECK_EQUAL(s.ConvertToOgl(Point2d(0, 0)), Point2d(-1, -1));
	BOOST_CHECK_EQUAL(s.ConvertToOgl(Point2d(200, 100)), Point2d(1, 1));
	BOOST_CHECK_EQUAL(s.ConvertToOgl(Point2d(0, 100)), Point2d(-1, 1));
	BOOST_CHECK_EQUAL(s.ConvertToOgl(Point2d(200, 0)), Point2d(1, -1));
	BOOST_CHECK_EQUAL(s.ConvertToOgl(Point2d(100, 50)), Point2d(0, 0));
}
