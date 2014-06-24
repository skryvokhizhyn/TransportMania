#include <Size2d.h>
#include <Point2d.h>
#include <PatchGrid.h>
#include <HeightMapLoader.h>
#include <boost/test/unit_test.hpp>
#include <algorithm>

using namespace trm;
using namespace trm::terrain::lod;

BOOST_AUTO_TEST_CASE(PatchGridTest1)
{
	PatchGrid pg(3, 2, terrain::HeightMapLoaderPtr());
	const PatchGrid::Positions pos = pg.GetAdjucentPatches(Point2d(0, 0));

	BOOST_CHECK_EQUAL(pos.size(), 1u);
	BOOST_CHECK_EQUAL(pos.front(), Size2d(0, 0));
}

BOOST_AUTO_TEST_CASE(PatchGridTest2)
{
	PatchGrid pg(3, 2, terrain::HeightMapLoaderPtr());
	const PatchGrid::Positions pos = pg.GetAdjucentPatches(Point2d(1, 1));

	BOOST_CHECK_EQUAL(pos.size(), 1u);
	BOOST_CHECK_EQUAL(pos[0], Size2d(0, 0));
}

BOOST_AUTO_TEST_CASE(PatchGridTest3)
{
	PatchGrid pg(3, 2, terrain::HeightMapLoaderPtr());
	PatchGrid::Positions pos = pg.GetAdjucentPatches(Point2d(2, 1));

	std::sort(pos.begin(), pos.end());

	BOOST_CHECK_EQUAL(pos.size(), 2u);
	BOOST_CHECK_EQUAL(pos[0], Size2d(0, 0));
	BOOST_CHECK_EQUAL(pos[1], Size2d(2, 0));
}

BOOST_AUTO_TEST_CASE(PatchGridTest4)
{
	PatchGrid pg(3, 2, terrain::HeightMapLoaderPtr());
	const PatchGrid::Positions pos = pg.GetAdjucentPatches(Point2d(1, 0));

	BOOST_CHECK_EQUAL(pos.size(), 1u);
	BOOST_CHECK_EQUAL(pos[0], Size2d(0, 0));
}

BOOST_AUTO_TEST_CASE(PatchGridTest5)
{
	PatchGrid pg(3, 2, terrain::HeightMapLoaderPtr());
	const PatchGrid::Positions pos = pg.GetAdjucentPatches(Point2d(0, 1));

	BOOST_CHECK_EQUAL(pos.size(), 1u);
	BOOST_CHECK_EQUAL(pos[0], Size2d(0, 0));
}

BOOST_AUTO_TEST_CASE(PatchGridTest6)
{
	PatchGrid pg(3, 2, terrain::HeightMapLoaderPtr());
	PatchGrid::Positions pos = pg.GetAdjucentPatches(Point2d(2, 2));

	std::sort(pos.begin(), pos.end());

	BOOST_CHECK_EQUAL(pos.size(), 4u);
	BOOST_CHECK_EQUAL(pos[0], Size2d(0, 0));
	BOOST_CHECK_EQUAL(pos[1], Size2d(0, 2));
	BOOST_CHECK_EQUAL(pos[2], Size2d(2, 0));
	BOOST_CHECK_EQUAL(pos[3], Size2d(2, 2));
}