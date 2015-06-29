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

		void Visit(RailRoadArc & rra) override { type = RRType::Arc; }
		void Visit(RailRoadLine & rrl) override { type = RRType::Line; }
	};
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorTest1)
{
	/*TangentPoint p1{Point3d(), TangentData()};
	TangentPoint p2{Point3d(1, 1, 1), TangentData()};*/

	//auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	TangentPoint p1{Point3d(), OptionalPoint3d()};
	TangentPoint p2{Point3d(1, 1, 1), OptionalPoint3d()};

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	BOOST_CHECK_EQUAL(roads.size(), 1u);

	RailRoadTypeDefiner rrtd;
	roads[0]->Accept(rrtd);

	BOOST_CHECK_EQUAL(rrtd.type, RRType::Line);
}

BOOST_AUTO_TEST_CASE(RailRoadConnectorTest2)
{
	/*TangentPoint p1{Point3d(), TangentData(TangentData::Type::Arc, Point3d(1, 0, 0))};
	TangentPoint p2{Point3d(1, 1, 1), TangentData()};

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));*/

	TangentPoint p1{Point3d(), Point3d(1, 0, 0)};
	TangentPoint p2{Point3d(1, 1, 1), OptionalPoint3d()};

	auto roads = RailRoadConnector::GetRoads(TangentPointPair(p1, p2));

	BOOST_CHECK_EQUAL(roads.size(), 1u);

	RailRoadTypeDefiner rrtd;
	roads[0]->Accept(rrtd);

	BOOST_CHECK_EQUAL(rrtd.type, RRType::Arc);
}
