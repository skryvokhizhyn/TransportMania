#include <boost/test/unit_test.hpp>

#include <WindowPosition.h>

using namespace trm;

BOOST_AUTO_TEST_CASE(WindowPositionTest1)
{
	const Size2d windowSize(10, 14);
	const Size2d screenSize(140, 38);

	BOOST_CHECK_EQUAL(GetWindowPosition(screenSize, windowSize, WindowPosition::p0, WindowPosition::p0), Point2d(0, 0));
	BOOST_CHECK_EQUAL(GetWindowPosition(screenSize, windowSize, WindowPosition::p25, WindowPosition::p25), Point2d(30.0f, 2.5f));
	BOOST_CHECK_EQUAL(GetWindowPosition(screenSize, windowSize, WindowPosition::p50, WindowPosition::p50), Point2d(65, 12));
	BOOST_CHECK_EQUAL(GetWindowPosition(screenSize, windowSize, WindowPosition::p75, WindowPosition::p75), Point2d(100.0f, 21.5f));
	BOOST_CHECK_EQUAL(GetWindowPosition(screenSize, windowSize, WindowPosition::p100, WindowPosition::p100), Point2d(130, 24));
}

BOOST_AUTO_TEST_CASE(WindowPositionTest2)
{
	const Size2d windowSize(10, 10);

	BOOST_CHECK_EQUAL(GetWindowPosition(Size2d(5, 5), windowSize, WindowPosition::p100, WindowPosition::p100), Point2d(0, 0));
	BOOST_CHECK_EQUAL(GetWindowPosition(Size2d(4, 4), windowSize, WindowPosition::p100, WindowPosition::p100), Point2d(0, 0));
}

BOOST_AUTO_TEST_CASE(WindowPositionTest3)
{
	const Size2d windowSize(10, 10);
	const Size2d screenSize(16, 16);

	BOOST_CHECK_EQUAL(GetWindowPosition(screenSize, windowSize, WindowPosition::p25, WindowPosition::p25), Point2d(0, 0));
	BOOST_CHECK_EQUAL(GetWindowPosition(screenSize, windowSize, WindowPosition::p75, WindowPosition::p75), Point2d(6, 6));
}
