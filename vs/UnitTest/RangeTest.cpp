#include <boost/test/unit_test.hpp>

#include <TerrainRangeCircle.h>
#include <TerrainRangeLine.h>
#include <TerrainRangeArc.h>
#include <GeometryUtils.h>
#include <RangeRectangle.h>

using namespace trm;
BOOST_AUTO_TEST_CASE(TerrainRangeTest1)
{
	TerrainRangeCircle c(trm::Point2d(0, 0), 0);
	const TerrainRange::Ranges & r = c.GetRanges();

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r.front().y, 0);
	BOOST_CHECK_EQUAL(r.front().xBegin, 0);
	BOOST_CHECK_EQUAL(r.front().xEnd, 0);
}

BOOST_AUTO_TEST_CASE(TerrainRangeTest2)
{
	TerrainRangeCircle c(trm::Point2d(0, 0), 1);
	const TerrainRange::Ranges & r = c.GetRanges();

	TerrainRange::Ranges::const_iterator it = r.begin();

	BOOST_CHECK_EQUAL(r.size(), 3u);

	BOOST_CHECK_EQUAL(it->y, -1);
	BOOST_CHECK_EQUAL(it->xBegin, -1);
	BOOST_CHECK_EQUAL(it->xEnd, 1);

	BOOST_CHECK_EQUAL((++it)->y, 0);
	BOOST_CHECK_EQUAL(it->xBegin, -1);
	BOOST_CHECK_EQUAL(it->xEnd, 1);

	BOOST_CHECK_EQUAL((++it)->y, 1);
	BOOST_CHECK_EQUAL(it->xBegin, -1);
	BOOST_CHECK_EQUAL(it->xEnd, 1);

	BOOST_CHECK(++it == r.end());
}

BOOST_AUTO_TEST_CASE(TerrainRangeTest3)
{
	TerrainRangeCircle c(trm::Point2d(0, 0), 2);
	const TerrainRange::Ranges & r = c.GetRanges();

	TerrainRange::Ranges::const_iterator it = r.begin();

	BOOST_CHECK_EQUAL(r.size(), 5u);

	BOOST_CHECK_EQUAL(it->y, -2);
	BOOST_CHECK_EQUAL(it->xBegin, -1);
	BOOST_CHECK_EQUAL(it->xEnd, 1);

	BOOST_CHECK_EQUAL((++it)->y, -1);
	BOOST_CHECK_EQUAL(it->xBegin, -2);
	BOOST_CHECK_EQUAL(it->xEnd, 2);

	BOOST_CHECK_EQUAL((++it)->y, 0);
	BOOST_CHECK_EQUAL(it->xBegin, -2);
	BOOST_CHECK_EQUAL(it->xEnd, 2);

	BOOST_CHECK_EQUAL((++it)->y, 1);
	BOOST_CHECK_EQUAL(it->xBegin, -2);
	BOOST_CHECK_EQUAL(it->xEnd, 2);

	BOOST_CHECK_EQUAL((++it)->y, 2);
	BOOST_CHECK_EQUAL(it->xBegin, -1);
	BOOST_CHECK_EQUAL(it->xEnd, 1);
}

BOOST_AUTO_TEST_CASE(TerrainRangeTest4)
{
	TerrainRangeLine c(trm::Point2d(0, 1), trm::Point2d(2, 1), 1);
	const TerrainRange::Ranges & r = c.GetRanges();

	BOOST_CHECK_EQUAL(r.size(), 3u);

	TerrainRange::Ranges::const_iterator it = r.begin();

	BOOST_CHECK_EQUAL(it->y, 0);
	BOOST_CHECK_EQUAL(it->xBegin, 0);
	BOOST_CHECK_EQUAL(it->xEnd, 2);

	BOOST_CHECK_EQUAL((++it)->y, 1);
	BOOST_CHECK_EQUAL(it->xBegin, 0);
	BOOST_CHECK_EQUAL(it->xEnd, 2);

	BOOST_CHECK_EQUAL((++it)->y, 2);
	BOOST_CHECK_EQUAL(it->xBegin, 0);
	BOOST_CHECK_EQUAL(it->xEnd, 2);
}

BOOST_AUTO_TEST_CASE(TerrainRangeTest6)
{
	TerrainRangeLine c(trm::Point2d(0, 3), trm::Point2d(2, 3), 3);
	const TerrainRange::Ranges & r = c.GetRanges();

	BOOST_CHECK_EQUAL(r.size(), 5u);

	TerrainRange::Ranges::const_iterator it = r.begin();

	BOOST_CHECK_EQUAL(it->y, 1);
	BOOST_CHECK_EQUAL(it->xBegin, 0);
	BOOST_CHECK_EQUAL(it->xEnd, 2);

	BOOST_CHECK_EQUAL((++it)->y, 2);
	BOOST_CHECK_EQUAL(it->xBegin, 0);
	BOOST_CHECK_EQUAL(it->xEnd, 2);

	BOOST_CHECK_EQUAL((++it)->y, 3);
	BOOST_CHECK_EQUAL(it->xBegin, 0);
	BOOST_CHECK_EQUAL(it->xEnd, 2);

	BOOST_CHECK_EQUAL((++it)->y, 4);
	BOOST_CHECK_EQUAL(it->xBegin, 0);
	BOOST_CHECK_EQUAL(it->xEnd, 2);

	BOOST_CHECK_EQUAL((++it)->y, 5);
	BOOST_CHECK_EQUAL(it->xBegin, 0);
	BOOST_CHECK_EQUAL(it->xEnd, 2);
}

BOOST_AUTO_TEST_CASE(TerrainRangeTest7)
{
	TerrainRangeLine c(trm::Point2d(0, 3), trm::Point2d(0, 6), 3);
	const TerrainRange::Ranges & r = c.GetRanges();

	BOOST_CHECK_EQUAL(r.size(), 4u);

	TerrainRange::Ranges::const_iterator it = r.begin();

	BOOST_CHECK_EQUAL(it->y, 3);
	BOOST_CHECK_EQUAL(it->xBegin, -2);
	BOOST_CHECK_EQUAL(it->xEnd, 2);

	BOOST_CHECK_EQUAL((++it)->y, 4);
	BOOST_CHECK_EQUAL(it->xBegin, -2);
	BOOST_CHECK_EQUAL(it->xEnd, 2);

	BOOST_CHECK_EQUAL((++it)->y, 5);
	BOOST_CHECK_EQUAL(it->xBegin, -2);
	BOOST_CHECK_EQUAL(it->xEnd, 2);

	BOOST_CHECK_EQUAL((++it)->y, 6);
	BOOST_CHECK_EQUAL(it->xBegin, -2);
	BOOST_CHECK_EQUAL(it->xEnd, 2);
}

BOOST_AUTO_TEST_CASE(TerrainRangeTest8)
{
	TerrainRangeLine c(trm::Point2d(0, 6), trm::Point2d(0, 3), 3);
	const TerrainRange::Ranges & r = c.GetRanges();

	BOOST_CHECK_EQUAL(r.size(), 4u);

	TerrainRange::Ranges::const_iterator it = r.begin();

	BOOST_CHECK_EQUAL(it->y, 3);
	BOOST_CHECK_EQUAL(it->xBegin, -2);
	BOOST_CHECK_EQUAL(it->xEnd, 2);

	BOOST_CHECK_EQUAL((++it)->y, 4);
	BOOST_CHECK_EQUAL(it->xBegin, -2);
	BOOST_CHECK_EQUAL(it->xEnd, 2);

	BOOST_CHECK_EQUAL((++it)->y, 5);
	BOOST_CHECK_EQUAL(it->xBegin, -2);
	BOOST_CHECK_EQUAL(it->xEnd, 2);

	BOOST_CHECK_EQUAL((++it)->y, 6);
	BOOST_CHECK_EQUAL(it->xBegin, -2);
	BOOST_CHECK_EQUAL(it->xEnd, 2);
}

BOOST_AUTO_TEST_CASE(TerrainRangeTest9)
{
	TerrainRangeLine c(trm::Point2d(2, 3), trm::Point2d(0, 3), 3);
	const TerrainRange::Ranges & r = c.GetRanges();

	BOOST_CHECK_EQUAL(r.size(), 5u);

	TerrainRange::Ranges::const_iterator it = r.begin();

	BOOST_CHECK_EQUAL(it->y, 1);
	BOOST_CHECK_EQUAL(it->xBegin, 0);
	BOOST_CHECK_EQUAL(it->xEnd, 2);

	BOOST_CHECK_EQUAL((++it)->y, 2);
	BOOST_CHECK_EQUAL(it->xBegin, 0);
	BOOST_CHECK_EQUAL(it->xEnd, 2);

	BOOST_CHECK_EQUAL((++it)->y, 3);
	BOOST_CHECK_EQUAL(it->xBegin, 0);
	BOOST_CHECK_EQUAL(it->xEnd, 2);

	BOOST_CHECK_EQUAL((++it)->y, 4);
	BOOST_CHECK_EQUAL(it->xBegin, 0);
	BOOST_CHECK_EQUAL(it->xEnd, 2);

	BOOST_CHECK_EQUAL((++it)->y, 5);
	BOOST_CHECK_EQUAL(it->xBegin, 0);
	BOOST_CHECK_EQUAL(it->xEnd, 2);
}

BOOST_AUTO_TEST_CASE(TerrainRangeTest10)
{
	TerrainRangeLine c(trm::Point2d(0, 0), trm::Point2d(3, 3), 1);
	const TerrainRange::Ranges & r = c.GetRanges();

	BOOST_CHECK_EQUAL(r.size(), 6u);

	const TerrainRange::Range & tr0 = r[0];
	BOOST_CHECK_EQUAL(tr0.y, -1);
	BOOST_CHECK_EQUAL(tr0.xBegin, -1);
	BOOST_CHECK_EQUAL(tr0.xEnd, 2);

	const TerrainRange::Range & tr1 = r[1];
	BOOST_CHECK_EQUAL(tr1.y, 0);
	BOOST_CHECK_EQUAL(tr1.xBegin, -1);
	BOOST_CHECK_EQUAL(tr1.xEnd, 2);

	const TerrainRange::Range & tr2 = r[2];
	BOOST_CHECK_EQUAL(tr2.y, 1);
	BOOST_CHECK_EQUAL(tr2.xBegin, -1);
	BOOST_CHECK_EQUAL(tr2.xEnd, 3);

	const TerrainRange::Range & tr3 = r[3];
	BOOST_CHECK_EQUAL(tr3.y, 2);
	BOOST_CHECK_EQUAL(tr3.xBegin, 0);
	BOOST_CHECK_EQUAL(tr3.xEnd, 4);

	const TerrainRange::Range & tr4 = r[4];
	BOOST_CHECK_EQUAL(tr4.y, 3);
	BOOST_CHECK_EQUAL(tr4.xBegin, 1);
	BOOST_CHECK_EQUAL(tr4.xEnd, 4);

	const TerrainRange::Range & tr5 = r[5];
	BOOST_CHECK_EQUAL(tr5.y, 4);
	BOOST_CHECK_EQUAL(tr5.xBegin, 1);
	BOOST_CHECK_EQUAL(tr5.xEnd, 4);
}

BOOST_AUTO_TEST_CASE(TerrainRangeTest11)
{
	TerrainRangeLine c(trm::Point2d(0, 0), trm::Point2d(-3, 3), 1);
	const TerrainRange::Ranges & r = c.GetRanges();

	BOOST_CHECK_EQUAL(r.size(), 6u);

	const TerrainRange::Range & tr0 = r[0];
	BOOST_CHECK_EQUAL(tr0.y, -1);
	BOOST_CHECK_EQUAL(tr0.xBegin, -2);
	BOOST_CHECK_EQUAL(tr0.xEnd, 1);

	const TerrainRange::Range & tr1 = r[1];
	BOOST_CHECK_EQUAL(tr1.y, 0);
	BOOST_CHECK_EQUAL(tr1.xBegin, -2);
	BOOST_CHECK_EQUAL(tr1.xEnd, 1);

	const TerrainRange::Range & tr2 = r[2];
	BOOST_CHECK_EQUAL(tr2.y, 1);
	BOOST_CHECK_EQUAL(tr2.xBegin, -3);
	BOOST_CHECK_EQUAL(tr2.xEnd, 1);

	const TerrainRange::Range & tr3 = r[3];
	BOOST_CHECK_EQUAL(tr3.y, 2);
	BOOST_CHECK_EQUAL(tr3.xBegin, -4);
	BOOST_CHECK_EQUAL(tr3.xEnd, 0);

	const TerrainRange::Range & tr4 = r[4];
	BOOST_CHECK_EQUAL(tr4.y, 3);
	BOOST_CHECK_EQUAL(tr4.xBegin, -4);
	BOOST_CHECK_EQUAL(tr4.xEnd, -1);

	const TerrainRange::Range & tr5 = r[5];
	BOOST_CHECK_EQUAL(tr5.y, 4);
	BOOST_CHECK_EQUAL(tr5.xBegin, -4);
	BOOST_CHECK_EQUAL(tr5.xEnd, -1);
}

BOOST_AUTO_TEST_CASE(TerrainRangeTest12)
{
	TerrainRangeLine c(trm::Point2d(0, 0), trm::Point2d(-3, -3), 1);
	const TerrainRange::Ranges & r = c.GetRanges();

	BOOST_CHECK_EQUAL(r.size(), 6u);

	const TerrainRange::Range & tr0 = r[0];
	BOOST_CHECK_EQUAL(tr0.y, -4);
	BOOST_CHECK_EQUAL(tr0.xBegin, -4);
	BOOST_CHECK_EQUAL(tr0.xEnd, -1);

	const TerrainRange::Range & tr1 = r[1];
	BOOST_CHECK_EQUAL(tr1.y, -3);
	BOOST_CHECK_EQUAL(tr1.xBegin, -4);
	BOOST_CHECK_EQUAL(tr1.xEnd, -1);

	const TerrainRange::Range & tr2 = r[2];
	BOOST_CHECK_EQUAL(tr2.y, -2);
	BOOST_CHECK_EQUAL(tr2.xBegin, -4);
	BOOST_CHECK_EQUAL(tr2.xEnd, 0);

	const TerrainRange::Range & tr3 = r[3];
	BOOST_CHECK_EQUAL(tr3.y, -1);
	BOOST_CHECK_EQUAL(tr3.xBegin, -3);
	BOOST_CHECK_EQUAL(tr3.xEnd, 1);

	const TerrainRange::Range & tr4 = r[4];
	BOOST_CHECK_EQUAL(tr4.y, 0);
	BOOST_CHECK_EQUAL(tr4.xBegin, -2);
	BOOST_CHECK_EQUAL(tr4.xEnd, 1);

	const TerrainRange::Range & tr5 = r[5];
	BOOST_CHECK_EQUAL(tr5.y, 1);
	BOOST_CHECK_EQUAL(tr5.xBegin, -2);
	BOOST_CHECK_EQUAL(tr5.xEnd, 1);
}

BOOST_AUTO_TEST_CASE(TerrainRangeTest13)
{
	TerrainRangeLine c(trm::Point2d(0, 0), trm::Point2d(5, 1), 1);
	const TerrainRange::Ranges & r = c.GetRanges();

	BOOST_CHECK_EQUAL(r.size(), 4u);

	const TerrainRange::Range & tr0 = r[0];
	BOOST_CHECK_EQUAL(tr0.y, -1);
	BOOST_CHECK_EQUAL(tr0.xBegin, -1);
	BOOST_CHECK_EQUAL(tr0.xEnd, 6);

	const TerrainRange::Range & tr1 = r[1];
	BOOST_CHECK_EQUAL(tr1.y, 0);
	BOOST_CHECK_EQUAL(tr1.xBegin, -1);
	BOOST_CHECK_EQUAL(tr1.xEnd, 6);

	const TerrainRange::Range & tr2 = r[2];
	BOOST_CHECK_EQUAL(tr2.y, 1);
	BOOST_CHECK_EQUAL(tr2.xBegin, -1);
	BOOST_CHECK_EQUAL(tr2.xEnd, 6);

	const TerrainRange::Range & tr3 = r[3];
	BOOST_CHECK_EQUAL(tr3.y, 2);
	BOOST_CHECK_EQUAL(tr3.xBegin, -1);
	BOOST_CHECK_EQUAL(tr3.xEnd, 6);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest1)
{
	Arc2d d{ trm::Point2d(0, -5), Degrees(-360), trm::Point2d(0, 0) };

	TerrainRangeArc c(d, 0.5f + 2.0f);
	const TerrainRange::Ranges & r = c.GetRanges();

	BOOST_CHECK_EQUAL(r.size(), 20u);

	const TerrainRange::Range & tr0 = r[0];
	BOOST_CHECK_EQUAL(tr0.y, -7);
	BOOST_CHECK_EQUAL(tr0.xBegin, 0);
	BOOST_CHECK_EQUAL(tr0.xEnd, 0);

	const TerrainRange::Range & tr1 = r[1];
	BOOST_CHECK_EQUAL(tr1.y, -6);
	BOOST_CHECK_EQUAL(tr1.xBegin, -4);
	BOOST_CHECK_EQUAL(tr1.xEnd, 4);

	const TerrainRange::Range & tr2 = r[2];
	BOOST_CHECK_EQUAL(tr2.y, -5);
	BOOST_CHECK_EQUAL(tr2.xBegin, -5);
	BOOST_CHECK_EQUAL(tr2.xEnd, 5);
	
	const TerrainRange::Range & tr3 = r[3];
	BOOST_CHECK_EQUAL(tr3.y, -4);
	BOOST_CHECK_EQUAL(tr3.xBegin, -6);
	BOOST_CHECK_EQUAL(tr3.xEnd, 6);

	const TerrainRange::Range & tr4 = r[4];
	BOOST_CHECK_EQUAL(tr4.y, -3);
	BOOST_CHECK_EQUAL(tr4.xBegin, -7);
	BOOST_CHECK_EQUAL(tr4.xEnd, 7);

	const TerrainRange::Range & tr5 = r[5];
	BOOST_CHECK_EQUAL(tr5.y, -2);
	BOOST_CHECK_EQUAL(tr5.xBegin, -7);
	BOOST_CHECK_EQUAL(tr5.xEnd, -2);

	const TerrainRange::Range & tr6 = r[6];
	BOOST_CHECK_EQUAL(tr6.y, -2);
	BOOST_CHECK_EQUAL(tr6.xBegin, 2);
	BOOST_CHECK_EQUAL(tr6.xEnd, 7);

	const TerrainRange::Range & tr7 = r[7];
	BOOST_CHECK_EQUAL(tr7.y, -1);
	BOOST_CHECK_EQUAL(tr7.xBegin, -7);
	BOOST_CHECK_EQUAL(tr7.xEnd, -2);

	const TerrainRange::Range & tr8 = r[8];
	BOOST_CHECK_EQUAL(tr8.y, -1);
	BOOST_CHECK_EQUAL(tr8.xBegin, 2);
	BOOST_CHECK_EQUAL(tr8.xEnd, 7);

	const TerrainRange::Range & tr9 = r[9];
	BOOST_CHECK_EQUAL(tr9.y, 0);
	BOOST_CHECK_EQUAL(tr9.xBegin, -7);
	BOOST_CHECK_EQUAL(tr9.xEnd, -3);

	const TerrainRange::Range & tr10 = r[10];
	BOOST_CHECK_EQUAL(tr10.y, 0);
	BOOST_CHECK_EQUAL(tr10.xBegin, 3);
	BOOST_CHECK_EQUAL(tr10.xEnd, 7);

	const TerrainRange::Range & tr11 = r[11];
	BOOST_CHECK_EQUAL(tr11.y, 1);
	BOOST_CHECK_EQUAL(tr11.xBegin, -7);
	BOOST_CHECK_EQUAL(tr11.xEnd, -2);

	const TerrainRange::Range & tr12 = r[12];
	BOOST_CHECK_EQUAL(tr12.y, 1);
	BOOST_CHECK_EQUAL(tr12.xBegin, 2);
	BOOST_CHECK_EQUAL(tr12.xEnd, 7);

	const TerrainRange::Range & tr13 = r[13];
	BOOST_CHECK_EQUAL(tr13.y, 2);
	BOOST_CHECK_EQUAL(tr13.xBegin, -7);
	BOOST_CHECK_EQUAL(tr13.xEnd, -2);

	const TerrainRange::Range & tr14 = r[14];
	BOOST_CHECK_EQUAL(tr14.y, 2);
	BOOST_CHECK_EQUAL(tr14.xBegin, 2);
	BOOST_CHECK_EQUAL(tr14.xEnd, 7);

	const TerrainRange::Range & tr15 = r[15];
	BOOST_CHECK_EQUAL(tr15.y, 3);
	BOOST_CHECK_EQUAL(tr15.xBegin, -7);
	BOOST_CHECK_EQUAL(tr15.xEnd, 7);

	const TerrainRange::Range & tr16 = r[16];
	BOOST_CHECK_EQUAL(tr16.y, 4);
	BOOST_CHECK_EQUAL(tr16.xBegin, -6);
	BOOST_CHECK_EQUAL(tr16.xEnd, 6);

	const TerrainRange::Range & tr17 = r[17];
	BOOST_CHECK_EQUAL(tr17.y, 5);
	BOOST_CHECK_EQUAL(tr17.xBegin, -5);
	BOOST_CHECK_EQUAL(tr17.xEnd, 5);

	const TerrainRange::Range & tr18 = r[18];
	BOOST_CHECK_EQUAL(tr18.y, 6);
	BOOST_CHECK_EQUAL(tr18.xBegin, -4);
	BOOST_CHECK_EQUAL(tr18.xEnd, 4);

	const TerrainRange::Range & tr19 = r[19];
	BOOST_CHECK_EQUAL(tr19.y, 7);
	BOOST_CHECK_EQUAL(tr19.xBegin, 0);
	BOOST_CHECK_EQUAL(tr19.xEnd, 0);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest20)
{
	Arc2d d{ trm::Point2d(0, -2), Degrees(-180), trm::Point2d(0, 0) };

	TerrainRangeArc c(d, 1.0f + 2.0f);
	const TerrainRange::Ranges & r = c.GetRanges();
	 
	BOOST_CHECK_EQUAL(r.size(), 9u);

	const TerrainRange::Range & tr0 = r[0];
	BOOST_CHECK_EQUAL(tr0.y, -4);
	BOOST_CHECK_EQUAL(tr0.xBegin, 0);
	BOOST_CHECK_EQUAL(tr0.xEnd, 0);

	const TerrainRange::Range & tr1 = r[1];
	BOOST_CHECK_EQUAL(tr1.y, -3);
	BOOST_CHECK_EQUAL(tr1.xBegin, -3);
	BOOST_CHECK_EQUAL(tr1.xEnd, 0);

	const TerrainRange::Range & tr2 = r[2];
	BOOST_CHECK_EQUAL(tr2.y, -2);
	BOOST_CHECK_EQUAL(tr2.xBegin, -4);
	BOOST_CHECK_EQUAL(tr2.xEnd, 0);
	
	const TerrainRange::Range & tr4 = r[3];
	BOOST_CHECK_EQUAL(tr4.y, -1);
	BOOST_CHECK_EQUAL(tr4.xBegin, -4);
	BOOST_CHECK_EQUAL(tr4.xEnd, 0);

	const TerrainRange::Range & tr6 = r[4];
	BOOST_CHECK_EQUAL(tr6.y, 0);
	BOOST_CHECK_EQUAL(tr6.xBegin, -4);
	BOOST_CHECK_EQUAL(tr6.xEnd, 0); // 1 due to range [0, 1e10-8]

	const TerrainRange::Range & tr8 = r[5];
	BOOST_CHECK_EQUAL(tr8.y, 1);
	BOOST_CHECK_EQUAL(tr8.xBegin, -4);
	BOOST_CHECK_EQUAL(tr8.xEnd, 1); // 1 due to range [0, 1e10-8]

	const TerrainRange::Range & tr9 = r[6];
	BOOST_CHECK_EQUAL(tr9.y, 2);
	BOOST_CHECK_EQUAL(tr9.xBegin, -4);
	BOOST_CHECK_EQUAL(tr9.xEnd, 1);

	const TerrainRange::Range & tr10 = r[7];
	BOOST_CHECK_EQUAL(tr10.y, 3);
	BOOST_CHECK_EQUAL(tr10.xBegin, -3);
	BOOST_CHECK_EQUAL(tr10.xEnd, 1);

	const TerrainRange::Range & tr11 = r[8];
	BOOST_CHECK_EQUAL(tr11.y, 4);
	BOOST_CHECK_EQUAL(tr11.xBegin, 0);
	BOOST_CHECK_EQUAL(tr11.xEnd, 0);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest80)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-1, 1), Degrees(-90), 2, AxisPairType(-6.0f, 6.0f));
	
	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -2);
	BOOST_CHECK_CLOSE(r[0].second, 2.0f, 0.0001f);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest81)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-1, 1), Degrees(-180), 2, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -2);
	BOOST_CHECK_EQUAL(r[0].second, 6);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest82)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, -1), Degrees(180), 2, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_CLOSE(r[0].first, 4.0f, 0.0001f);
	BOOST_CHECK_EQUAL(r[0].second, 6);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest83)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, 1), Degrees(0), 2, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -4);
	BOOST_CHECK_EQUAL(r[0].second, -4);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest84)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-1, 1), Degrees(270), 2, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK_EQUAL(r.size(), 2u);
	BOOST_CHECK_EQUAL(r[0].first, -6);
	BOOST_CHECK_EQUAL(r[0].second, -2);
	BOOST_CHECK_EQUAL(r[1].first, 2);
	BOOST_CHECK_EQUAL(r[1].second, 6);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest85)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, -1), Degrees(-180), 2, AxisPairType(-6.0f, 6.0f));
	 
	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -6);
	BOOST_CHECK_CLOSE(r[0].second, 4.0f, 0.0001f);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest86)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, -1), Degrees(180), 2, AxisPairType(-1.0f, 1.0f));

	BOOST_CHECK(r.empty());
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest87)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, -1), Degrees(170), 2, AxisPairType(-1.0f, 1.0f));

	BOOST_CHECK(r.empty());
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest88)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-3, -1), Degrees(190), 2, AxisPairType(-1.0f, 1.0f));

	BOOST_CHECK(r.empty());
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest89)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-3, -1), Degrees(190), -2, AxisPairType(-1.0f, 1.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -1);
	BOOST_CHECK_EQUAL(r[0].second, 1);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest90)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, -1), Degrees(90), -2, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -4);
	BOOST_CHECK_CLOSE(r[0].second, 1.0f, 0.0001f);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest91)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, -1), Degrees(-270), -4, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, 2);
	BOOST_CHECK_EQUAL(r[0].second, 6);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest92)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, 0), Degrees(-180), -2, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK(r.empty());
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest93)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, -1), Degrees(-230), -4, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK(r.empty());
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest94)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-1, -1), Degrees(-200), -4, AxisPairType(-6.0f, 0.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -6);
	BOOST_CHECK_EQUAL(r[0].second, -4);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest95)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, 0), Degrees(-180), 2, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -6);
	BOOST_CHECK_EQUAL(r[0].second, 6);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest951)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, 0), Degrees(180), -2, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -6);
	BOOST_CHECK_EQUAL(r[0].second, 6);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest952)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(0, -2), Degrees(-90), 0, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -6);
	BOOST_CHECK_EQUAL(r[0].second, 0);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest96)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, 0), Degrees(-180), 0, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -6);
	BOOST_CHECK_EQUAL(r[0].second, 6);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest97)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, 0), Degrees(180), 0, AxisPairType(-6.0f, 6.0f));
	 
	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -6);
	BOOST_CHECK_EQUAL(r[0].second, 6);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest98)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(0, -2), Degrees(180), 0, AxisPairType(-3.0f, -1.0f));

	BOOST_CHECK(r.empty());
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest100)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, 0), Degrees(90), 0, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -6);
	BOOST_CHECK_EQUAL(r[0].second, 0);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest101)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(2, 0), Degrees(-90), 0, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, 0);
	BOOST_CHECK_EQUAL(r[0].second, 6);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest99)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(0, -2), Degrees(180), 0, AxisPairType(1.0f, 3.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, 1);
	BOOST_CHECK_EQUAL(r[0].second, 3);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest102)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(0, 2), Degrees(-90), 0, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, 0);
	BOOST_CHECK_EQUAL(r[0].second, 6);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest103)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(0, 2), Degrees(90), 0, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -6);
	BOOST_CHECK_EQUAL(r[0].second, 0);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest104)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(0, 2), Degrees(91), 0, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -6);
	BOOST_CHECK_EQUAL(r[0].second, 0);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest105)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(0, 2), Degrees(-91), 0, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, 0);
	BOOST_CHECK_EQUAL(r[0].second, 6);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest106)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, 0), Degrees(-181), 0, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -6);
	BOOST_CHECK_EQUAL(r[0].second, 6);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest107)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, 0), Degrees(-179), 0, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -6);
	BOOST_CHECK_EQUAL(r[0].second, 0);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest108)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, 0), Degrees(-181), 0, AxisPairType(-6.0f, 6.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -6);
	BOOST_CHECK_EQUAL(r[0].second, 6);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest109)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, 0), Degrees(-181), 0, AxisPairType(-6.0f, -6.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -6);
	BOOST_CHECK_EQUAL(r[0].second, -6);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest110)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, 0), Degrees(-181), 1, AxisPairType(-6.0f, -6.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -6);
	BOOST_CHECK_EQUAL(r[0].second, -6);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest111)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(0, 2), Degrees(-90), 0, AxisPairType(-3.0f, -1.0f));

	BOOST_CHECK(r.empty());
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest112)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, 0), Degrees(-0.0f), 0, AxisPairType(-3.0f, -1.0f));

	BOOST_CHECK_EQUAL(r.size(), 1u);
	BOOST_CHECK_EQUAL(r[0].first, -3);
	BOOST_CHECK_EQUAL(r[0].second, -1);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest113)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(2, 0), Degrees(-0.0f), 0, AxisPairType(-3.0f, -1.0f));

	BOOST_CHECK(r.empty());
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcTest114)
{
	const TerrainRangeArc::IntersectionType r = TerrainRangeArc::GetIntersection(
		Point2d(-2, 0), Degrees(-0.0f), 0, AxisPairType(1.0f, 3.0f));

	BOOST_CHECK(r.empty());
}

namespace trm
{
	bool operator == (const Line & l1, const Line & l2)
	{
		return utils::CheckEqual(l1.a, l2.a)
			&& utils::CheckEqual(l1.b, l2.b)
			&& utils::CheckEqual(l1.c, l2.c);
	}

	std::ostream & operator << (std::ostream & o, const Line & l)
	{
		o << "(" << l.a << ", " << l.b << ", " << l.c << ")";

		return o;
	}

} // namespace trm

BOOST_AUTO_TEST_CASE(TerrainRangeGetRangeRectangleTest1)
{
	const RangeRectangle res = GetRangeRectangle(Point2d(0, 0), Point2d(1, 1), 1);

	BOOST_CHECK_EQUAL(res.ab, Line(1, 1, 0));
	BOOST_CHECK_EQUAL(res.bc, Line(1, -1, sqrtf(2) / 2));
	BOOST_CHECK_EQUAL(res.cd, Line(1, 1, -2));
	BOOST_CHECK_EQUAL(res.da, Line(1, -1, -sqrtf(2) / 2));
}

#include <boost/units/cmath.hpp>

BOOST_AUTO_TEST_CASE(TerrainRangeGetRangeRectangleTest2)
{
	const Angle a = Degrees(30);

	const RangeRectangle res = GetRangeRectangle(Point2d(0, 0), Point2d(boost::units::cos(a), boost::units::sin(a)), 1);
	const float w = 1 / static_cast<float>(boost::units::cos(a));

	BOOST_CHECK_EQUAL(res.ab, Line(boost::units::cos(a), boost::units::sin(a), 0));
	BOOST_CHECK_EQUAL(res.bc, Line(boost::units::sin(a), -boost::units::cos(a), 0.5f));
	BOOST_CHECK_EQUAL(res.cd, Line(boost::units::cos(a), boost::units::sin(a), w * (- res.cd.a)));
	BOOST_CHECK_EQUAL(res.da, Line(boost::units::sin(a), -boost::units::cos(a), -0.5f));
}

BOOST_AUTO_TEST_CASE(TerrainRangeGetRangeRectangleTest3)
{
	const Angle a = Degrees(60);

	const RangeRectangle res = GetRangeRectangle(Point2d(0, 0), Point2d(boost::units::cos(a), boost::units::sin(a)), 1);
	const float w = 1 / static_cast<float>(boost::units::cos(a));

	BOOST_CHECK_EQUAL(res.ab.c, 0);
	BOOST_CHECK_EQUAL(res.bc.c, 0.5f);
	BOOST_CHECK_EQUAL(res.cd.c / (- res.cd.a), w);
	BOOST_CHECK_EQUAL(res.da.c, -0.5f);
}

BOOST_AUTO_TEST_CASE(TerrainRangeGetRangeRectangleTest4)
{
	const Angle a = Degrees(60);

	const RangeRectangle res = GetRangeRectangle(Point2d(boost::units::cos(a), boost::units::sin(a)), Point2d(0, 0), 1);
	const float w = 1 / static_cast<float>(boost::units::cos(a));

	BOOST_CHECK_EQUAL(res.ab, Line(-boost::units::cos(a), -boost::units::sin(a), w * (- res.cd.a)));
	BOOST_CHECK_EQUAL(res.bc, Line(-boost::units::sin(a), boost::units::cos(a), 0.5f));
	BOOST_CHECK_EQUAL(res.cd, Line(-boost::units::cos(a), -boost::units::sin(a), 0));
	BOOST_CHECK_EQUAL(res.da, Line(-boost::units::sin(a), boost::units::cos(a), -0.5f));
}

BOOST_AUTO_TEST_CASE(TerrainRangeGetRangeRectangleTest5)
{
	const RangeRectangle res = GetRangeRectangle(Point2d(0, 0), Point2d(1, 0), 1);

	BOOST_CHECK_EQUAL(res.ab, Line(-1, 0, 0));
	BOOST_CHECK_EQUAL(res.bc, Line(0, -1, 0.5f));
	BOOST_CHECK_EQUAL(res.cd, Line(-1, 0, 1));
	BOOST_CHECK_EQUAL(res.da, Line(0, -1, -0.5f));
}

BOOST_AUTO_TEST_CASE(TerrainRangeGetRangeRectangleTest6)
{
	const RangeRectangle res = GetRangeRectangle(Point2d(0, 0), Point2d(0, 1), 1);

	BOOST_CHECK_EQUAL(res.ab, Line(0, 1, 0));
	BOOST_CHECK_EQUAL(res.bc, Line(1, 0, 0.5f));
	BOOST_CHECK_EQUAL(res.cd, Line(0, 1, -1));
	BOOST_CHECK_EQUAL(res.da, Line(1, 0, -0.5f));
}

BOOST_AUTO_TEST_CASE(TerrainRangeGetRangeRectangleTest7)
{
	const RangeRectangle res = GetRangeRectangle(Point2d(0, 1), Point2d(0, 0), 1);

	BOOST_CHECK_EQUAL(res.ab, Line(0, -1, 1));
	BOOST_CHECK_EQUAL(res.bc, Line(-1, 0, 0.5f));
	BOOST_CHECK_EQUAL(res.cd, Line(0, -1, 0));
	BOOST_CHECK_EQUAL(res.da, Line(-1, 0, -0.5f));
}

BOOST_AUTO_TEST_CASE(TerrainRangeGetRangeRectangleTest8)
{
	const RangeRectangle res = GetRangeRectangle(Point2d(1, 0), Point2d(0, 0), 1);

	BOOST_CHECK_EQUAL(res.ab, Line(1, 0, -1));
	BOOST_CHECK_EQUAL(res.bc, Line(0, 1, 0.5f));
	BOOST_CHECK_EQUAL(res.cd, Line(1, 0, 0));
	BOOST_CHECK_EQUAL(res.da, Line(0, 1, -0.5f));
}

BOOST_AUTO_TEST_CASE(TerrainRangeGetRangeRectangleTest9)
{
	const RangeRectangle res = GetRangeRectangle(Point2d(0, 1), Point2d(2, 1), 1);

	const Line etalonAB = utils::GetLine(Point2d(0, 1.5), Point2d(0, 0.5));

	BOOST_CHECK_EQUAL(res.ab.a, 2 * etalonAB.a);
	BOOST_CHECK_EQUAL(res.ab.b, 0);
	BOOST_CHECK_EQUAL(res.ab.c, 0);
}

BOOST_AUTO_TEST_CASE(TerrainRangeArcWrongTopPointTest1)
{
	TerrainRangeArc r({ Point2d(1, 3), Degrees(90), Point2d(3, 3) }, 1.0f);
	const auto & ranges = r.GetRanges();
	BOOST_CHECK_EQUAL(ranges.size(), 4u);
	BOOST_CHECK_EQUAL(ranges[0].y, 0u);
	BOOST_CHECK_EQUAL(ranges[1].y, 1u);
	BOOST_CHECK_EQUAL(ranges[2].y, 2u);
	BOOST_CHECK_EQUAL(ranges[3].y, 3u);
}