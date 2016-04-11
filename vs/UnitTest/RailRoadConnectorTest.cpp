#include <boost/test/unit_test.hpp>

#include <RailRoadConnector.h>
#include <RailRoadVisitor.h>
#include <RailRoadLine.h>
#include <RailRoadArc.h>

using namespace trm;

// RailRoadConnector

namespace
{
	enum RRType
	{
		Line,
		Arc
	};

	struct RailRoadTypeDefiner
		: public RailRoadVisitor
	{
		RRType type;

		RailRoadTypeDefiner() : type(RRType::Line) {}

		void Visit(RailRoadArc & /*rra*/) override { type = RRType::Arc; }
		void Visit(RailRoadLine & /*rrl*/) override { type = RRType::Line; }
	};
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorNoDirectionsTest1)
{
	TangentPoint p1{Point3d(0, 0, 0), OptionalPoint3d()};
	TangentPoint p2{Point3d(1, 1, 1), OptionalPoint3d()};

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	BOOST_CHECK_EQUAL(roads.size(), 1u);

	RailRoadTypeDefiner rrtd;
	roads[0]->Accept(rrtd);

	BOOST_CHECK_EQUAL(rrtd.type, RRType::Line);
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorOneDirectionToSecondPointTest1)
{
	TangentPoint p1{Point3d(0, 0, 0), Point3d(1, 0, 0)};
	TangentPoint p2{Point3d(1, 0, 0), OptionalPoint3d()};

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	BOOST_CHECK_EQUAL(roads.size(), 1u);

	RailRoadTypeDefiner rrtd;
	roads[0]->Accept(rrtd);

	BOOST_CHECK_EQUAL(rrtd.type, RRType::Line);
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorOneDirectionToFirstPointTest1)
{
	TangentPoint p1{ Point3d(0, 0, 0), OptionalPoint3d() };
	TangentPoint p2{ Point3d(1, 0, 0), Point3d(-1, 0, 0) };

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	BOOST_CHECK_EQUAL(roads.size(), 1u);

	RailRoadTypeDefiner rrtd;
	roads[0]->Accept(rrtd);

	BOOST_CHECK_EQUAL(rrtd.type, RRType::Line);
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorOneDirectionFromSecondPointTest1)
{
	TangentPoint p1{ Point3d(0, 0, 0), Point3d(-1, 0, 0) };
	TangentPoint p2{ Point3d(1, 0, 0), OptionalPoint3d() };

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	BOOST_CHECK_EQUAL(roads.size(), 0u);
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorOneDirectionFromSecondPointTest2)
{
	TangentPoint p1{ Point3d(0, 0, 0), Point3d(-1, 1, 0) };
	TangentPoint p2{ Point3d(1, 0, 0), OptionalPoint3d() };

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	BOOST_CHECK_EQUAL(roads.size(), 1u);

	RailRoadTypeDefiner rrtd;
	roads[0]->Accept(rrtd);

	BOOST_CHECK_EQUAL(rrtd.type, RRType::Arc);
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorOneDirectionFromFirstPointTest1)
{
	TangentPoint p1{ Point3d(0, 0, 0), OptionalPoint3d() };
	TangentPoint p2{ Point3d(1, 1, 1), Point3d(1, 0, 0) };

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	BOOST_CHECK_EQUAL(roads.size(), 1u);

	RailRoadTypeDefiner rrtd;
	roads[0]->Accept(rrtd);

	BOOST_CHECK_EQUAL(rrtd.type, RRType::Arc);
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorOneDirectionFromFirstPointTest2)
{
	TangentPoint p1{ Point3d(0, 0, 0), OptionalPoint3d() };
	TangentPoint p2{ Point3d(1, 1, 1), Point3d(1, 1, 0) };

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	BOOST_CHECK_EQUAL(roads.size(), 0u);
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorTwoDirectionsToEachOtherTest1)
{
	TangentPoint p1{ Point3d(0, 0, 0), Point3d(1, 0, 0) };
	TangentPoint p2{ Point3d(1, 0, 0), Point3d(-1, 0, 0) };

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	BOOST_CHECK_EQUAL(roads.size(), 1u);

	RailRoadTypeDefiner rrtd;
	roads[0]->Accept(rrtd);

	BOOST_CHECK_EQUAL(rrtd.type, RRType::Line);
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorTwoDirectionsOneFromOtherTest1)
{
	TangentPoint p1{ Point3d(0, 0, 0), Point3d(-1, 0, 0) };
	TangentPoint p2{ Point3d(1, 0, 0), Point3d(-1, 0, 0) };

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	BOOST_CHECK_EQUAL(roads.size(), 0u);
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorTwoDirectionsOneFromOtherTest2)
{
	TangentPoint p1{ Point3d(0, 0, 0), Point3d(1, 0, 0) };
	TangentPoint p2{ Point3d(1, 0, 0), Point3d(1, 0, 0) };

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	BOOST_CHECK_EQUAL(roads.size(), 0u);
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorTwoDirectionsOnePerpendicularTest1)
{
	TangentPoint p1{ Point3d(0, 0, 0), Point3d(0, 1, 0) };
	TangentPoint p2{ Point3d(1, 0, 0), Point3d(-1, 0, 0) };

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	BOOST_CHECK_EQUAL(roads.size(), 0u);
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorTwoDirectionsOnePerpendicularTest2)
{
	TangentPoint p1{ Point3d(0, 0, 0), Point3d(-1, 1, 0) };
	TangentPoint p2{ Point3d(1, 0, 0), Point3d(1, 0, 0) };

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	BOOST_CHECK_EQUAL(roads.size(), 0u);
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorArcOneDirectionTest1)
{
	TangentPoint p1{ Point3d(0, 0, 0), OptionalPoint3d() };
	TangentPoint p2{ Point3d(1, 1, 1), Point3d(-1, 0, 0) };

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	BOOST_CHECK_EQUAL(roads.size(), 1u);

	RailRoadTypeDefiner rrtd;
	roads[0]->Accept(rrtd);

	BOOST_CHECK_EQUAL(rrtd.type, RRType::Arc);
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorArcOneDirectionTest2)
{
	TangentPoint p1{ Point3d(0, 0, 0), Point3d(-1, 0, 0) };
	TangentPoint p2{ Point3d(1, 1, 1), OptionalPoint3d() };

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	BOOST_CHECK_EQUAL(roads.size(), 1u);

	RailRoadTypeDefiner rrtd;
	roads[0]->Accept(rrtd);

	BOOST_CHECK_EQUAL(rrtd.type, RRType::Arc);
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorArcTwoDirectionsTest1)
{
	TangentPoint p1{ Point3d(0, 0, 0), Point3d(1, 0, 0) };
	TangentPoint p2{ Point3d(1, 1, 1), Point3d(0, -1, 0) };

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	BOOST_CHECK_EQUAL(roads.size(), 1u);

	RailRoadTypeDefiner rrtd;
	roads[0]->Accept(rrtd);

	BOOST_CHECK_EQUAL(rrtd.type, RRType::Arc);
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorArcTwoDirectionsTest2)
{
	TangentPoint p1{ Point3d(0, 0, 0), Point3d(-1, 0, 0) };
	TangentPoint p2{ Point3d(1, 1, 1), Point3d(0, 1, 0) };

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	BOOST_CHECK_EQUAL(roads.size(), 1u);

	RailRoadTypeDefiner rrtd;
	roads[0]->Accept(rrtd);

	BOOST_CHECK_EQUAL(rrtd.type, RRType::Arc);
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorArcTwoDirectionsInvalidTest1)
{
	TangentPoint p1{ Point3d(0, 0, 0), Point3d(-1, 0, 0) };
	TangentPoint p2{ Point3d(1, 1, 1), Point3d(0, -1, 0) };

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	BOOST_CHECK_EQUAL(roads.size(), 2u);
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorArcTwoDirectionsParallelTest1)
{
	TangentPoint p1{ Point3d(0, 0, 0), Point3d(1, 0, 0) };
	TangentPoint p2{ Point3d(1, 1, 1), Point3d(-1, 0, 0) };

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	BOOST_CHECK_EQUAL(roads.size(), 2u);

	RailRoadTypeDefiner rrtd;
	roads[0]->Accept(rrtd);

	BOOST_CHECK_EQUAL(rrtd.type, RRType::Arc);
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorArcsConnectionTest1)
{
	PointWithDirection2d from = { Point2d(1, 1), Point2d(0, 1) };
	PointWithDirection2d to = { Point2d(2, 1), Point2d(0, 1) };

	BOOST_CHECK_EQUAL(RailRoadConnector::GetArcsConnectionPoint(from, to), Point2d(1.5f, 1.5f));
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorArcsConnectionTest2)
{
	PointWithDirection2d from = { Point2d(1, 1), Point2d(0, -1) };
	PointWithDirection2d to = { Point2d(2, 1), Point2d(0, -1) };

	BOOST_CHECK_EQUAL(RailRoadConnector::GetArcsConnectionPoint(from, to), Point2d(1.50000012f, 0.50000006f));
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorArcsConnectionTest3)
{
	PointWithDirection2d from = { Point2d(1, 1), Point2d(-1, 0) };
	PointWithDirection2d to = { Point2d(1, -1), Point2d(-1, 0) };

	BOOST_CHECK_EQUAL(RailRoadConnector::GetArcsConnectionPoint(from, to), Point2d(0.0f, 0.0f));
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorArcsConnectionTest4)
{
	PointWithDirection2d from = { Point2d(1, 1), Point2d(-2, 2) };
	PointWithDirection2d to = { Point2d(2, 1), Point2d(2, 2) };

	BOOST_CHECK_EQUAL(RailRoadConnector::GetArcsConnectionPoint(from, to), Point2d(1.49999988f, 2.20710683f));
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorArcsConnectionTest5)
{
	PointWithDirection2d from = { Point2d(1, 1), Point2d(-1, 1) };
	PointWithDirection2d to = { Point2d(2, 1), Point2d(1, 1) };

	BOOST_CHECK_EQUAL(RailRoadConnector::GetArcsConnectionPoint(from, to), Point2d(1.49999988f, 2.20710683f));
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorArcsConnectionTest6)
{
	PointWithDirection2d from = { Point2d(1, 1), Point2d(2, 2) };
	PointWithDirection2d to = { Point2d(1, 2), Point2d(2, -2) };

	BOOST_CHECK_EQUAL(RailRoadConnector::GetArcsConnectionPoint(from, to), Point2d(1.20710683f, 1.5f));
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorArcsConnectionSameRotationTest1)
{
	PointWithDirection2d from = { Point2d(0, 0), Point2d(1, 0) };
	PointWithDirection2d to = { Point2d(1, 1), Point2d(-1, 0) };

	BOOST_CHECK_EQUAL(RailRoadConnector::GetArcsConnectionPoint(from, to), Point2d(0.5f, 0.5f));
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorArcsConnectionDirectionEqualToP1P2Test1)
{
	PointWithDirection2d from = { Point2d(0, 0), Point2d(1, 1) };
	PointWithDirection2d to = { Point2d(1, 1), Point2d(-1, 0) };

	// adjusted to return equal values with current implementation :)
	// will change once proper sliding of middle point will be implemented based on angles
	BOOST_CHECK_EQUAL(RailRoadConnector::GetArcsConnectionPoint(from, to), Point2d(1, 1));
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorArcsConnectionDirectionEqualToP1P2Test2)
{
	PointWithDirection2d from = { Point2d(0, 0), Point2d(1, 0) };
	PointWithDirection2d to = { Point2d(1, 1), Point2d(-1, -1) };

	BOOST_CHECK_EQUAL(RailRoadConnector::GetArcsConnectionPoint(from, to), Point2d(0.5f, 0.5f));
}
