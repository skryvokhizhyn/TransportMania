#include <boost/test/unit_test.hpp>

#define UNIT_TESTING_ON

#include <RoadNetworkImpl.h>
#include <RoadNetworkManager.h>
#include <RailRoadFactory.h>
#include <Point3d.h>
#include <Point2d.h>
#include <PointHash.h>
#include <RoadPoint.h>
#include <RailRoadSizer.h>

using namespace trm;
using namespace network_impl;

BOOST_AUTO_TEST_CASE(RoadNetworkTest1)
{
	RoadNetworkImpl<int, int> n;

	BOOST_CHECK(n.InsertEdge(0, 1, 0));
	BOOST_CHECK(!n.InsertEdge(0, 1, 0));
	BOOST_CHECK(n.InsertEdge(0, 2, 1));
	BOOST_CHECK(!n.InsertEdge(1, 0, 1));

	auto r1 = n.GetEdgeValue(0, 1);
	BOOST_CHECK(r1.second);
	BOOST_CHECK_EQUAL(r1.first, 0);

	auto r2 = n.GetEdgeValue(0, 3);
	BOOST_CHECK_EQUAL(r2.second, false);

	auto r3 = n.GetEdgeValue(2, 0);
	BOOST_CHECK(r3.second);
	BOOST_CHECK_EQUAL(r3.first, 1);
}

BOOST_AUTO_TEST_CASE(RoadNetworkTest2)
{
	RoadNetworkImpl<int, int> n;

	BOOST_CHECK(n.InsertEdge(0, 1, 0));
	
	auto r1 = n.GetRoute(0, 1);

	BOOST_CHECK_EQUAL(r1.Size(), 2u);

	auto it1 = r1.Begin();
	auto rit1 = r1.RBegin();

	BOOST_CHECK_EQUAL(it1.Value(), 0);
	BOOST_CHECK_EQUAL(rit1.Value(), 1);
	BOOST_CHECK(it1.Next());
	BOOST_CHECK(rit1.Next());
	BOOST_CHECK_EQUAL(it1.Value(), 1);
	BOOST_CHECK_EQUAL(rit1.Value(), 0);
	BOOST_CHECK(!it1.Next());
	BOOST_CHECK(!rit1.Next());
}

BOOST_AUTO_TEST_CASE(RoadNetworkTest3)
{
	RoadNetworkImpl<int, int> n;

	BOOST_CHECK(n.InsertEdge(0, 1, 1));
	BOOST_CHECK(n.InsertEdge(1, 2, 1));
	BOOST_CHECK(n.InsertEdge(2, 3, 1));
	BOOST_CHECK(n.InsertEdge(0, 3, 5));

	auto r1 = n.GetRoute(0, 3);
	auto r2 = n.GetRoute(3, 0);

	BOOST_CHECK_EQUAL(r1.Size(), 4u);
	BOOST_CHECK_EQUAL(r2.Size(), 4u);

	auto r1i1 = r1.Begin();
	auto r1i2 = r2.RBegin();

	BOOST_CHECK_EQUAL(r1i1.Value(), r1i2.Value());
	BOOST_CHECK(r1i1.Next());
	BOOST_CHECK(r1i2.Next());
	BOOST_CHECK_EQUAL(r1i1.Value(), r1i2.Value());
	BOOST_CHECK(r1i1.Next());
	BOOST_CHECK(r1i2.Next());
	BOOST_CHECK_EQUAL(r1i1.Value(), r1i2.Value());
	BOOST_CHECK(r1i1.Next());
	BOOST_CHECK(r1i2.Next());
	BOOST_CHECK_EQUAL(r1i1.Value(), r1i2.Value());
	BOOST_CHECK(!r1i1.Next());
	BOOST_CHECK(!r1i2.Next());
}

BOOST_AUTO_TEST_CASE(RoadNetworkTest4)
{
	RoadNetworkImpl<int, int> n;

	BOOST_CHECK(n.InsertEdge(0, 1, 1));
	BOOST_CHECK(n.InsertEdge(1, 2, 1));
	BOOST_CHECK(n.InsertEdge(2, 3, 1));
	BOOST_CHECK(n.InsertEdge(0, 3, 5));

	auto r1 = n.GetRoute(0, 3);
	auto r2 = n.GetRoute(3, 0);

	BOOST_CHECK_EQUAL(r1.Size(), 4u);
	BOOST_CHECK_EQUAL(r2.Size(), 4u);

	auto r1i1 = r1.RBegin();
	auto r1i2 = r2.Begin();

	BOOST_CHECK_EQUAL(r1i1.Value(), r1i2.Value());
	BOOST_CHECK(r1i1.Next());
	BOOST_CHECK(r1i2.Next());
	BOOST_CHECK_EQUAL(r1i1.Value(), r1i2.Value());
	BOOST_CHECK(r1i1.Next());
	BOOST_CHECK(r1i2.Next());
	BOOST_CHECK_EQUAL(r1i1.Value(), r1i2.Value());
	BOOST_CHECK(r1i1.Next());
	BOOST_CHECK(r1i2.Next());
	BOOST_CHECK_EQUAL(r1i1.Value(), r1i2.Value());
	BOOST_CHECK(!r1i1.Next());
	BOOST_CHECK(!r1i2.Next());
}

BOOST_AUTO_TEST_CASE(RoadNetworkTest5)
{
	RoadNetworkImpl<int, int> n;

	BOOST_CHECK(n.InsertEdge(0, 1, 1));
	BOOST_CHECK(n.InsertEdge(1, 2, 6));
	BOOST_CHECK(n.InsertEdge(2, 3, 1));
	BOOST_CHECK(n.InsertEdge(0, 3, 5));

	auto r1 = n.GetRoute(0, 3);
	auto r2 = n.GetRoute(3, 0);

	BOOST_CHECK_EQUAL(r1.Size(), 2u);
	BOOST_CHECK_EQUAL(r2.Size(), 2u);

	auto r1i1 = r1.Begin();
	auto r1i2 = r2.RBegin();

	BOOST_CHECK_EQUAL(r1i1.Value(), r1i2.Value());
	BOOST_CHECK_EQUAL(r1i1.Value(), 0);
	BOOST_CHECK(r1i1.Next());
	BOOST_CHECK(r1i2.Next());
	BOOST_CHECK_EQUAL(r1i1.Value(), r1i2.Value());
	BOOST_CHECK_EQUAL(r1i1.Value(), 3);
	BOOST_CHECK(!r1i1.Next());
	BOOST_CHECK(!r1i2.Next());
}

BOOST_AUTO_TEST_CASE(RoadNetworkTest6)
{
	RoadNetworkImpl<int, int> n;

	BOOST_CHECK(n.InsertEdge(0, 1, 1));
	BOOST_CHECK(n.InsertEdge(1, 2, 6));
	BOOST_CHECK(n.InsertEdge(2, 3, 1));
	BOOST_CHECK(n.InsertEdge(0, 3, 5));

	auto r1 = n.GetRoute(0, 10);
	
	BOOST_CHECK_EQUAL(r1.Size(), 0u);	
}

BOOST_AUTO_TEST_CASE(RoadNetworkTest7)
{
	RoadNetworkImpl<int, int> n;

	BOOST_CHECK(n.InsertEdge(0, 1, 1));
	BOOST_CHECK(n.InsertEdge(2, 3, 1));

	auto r1 = n.GetRoute(0, 3);
	
	BOOST_CHECK_EQUAL(r1.Size(), 0u);	
}

BOOST_AUTO_TEST_CASE(RoadNetworkTest8)
{
	RoadNetworkImpl<int, int> n;

	BOOST_CHECK(n.InsertEdge(0, 1, 1));
	BOOST_CHECK(n.InsertEdge(1, 2, 1));
	BOOST_CHECK(n.InsertEdge(2, 3, 1));
	BOOST_CHECK(n.InsertEdge(4, 5, 1));
	BOOST_CHECK(n.InsertEdge(5, 6, 1));

	auto r1 = n.GetRoute(0, 6);
	
	BOOST_CHECK_EQUAL(r1.Size(), 0u);	
}

BOOST_AUTO_TEST_CASE(RoadNetworkTest9)
{
	RoadNetworkManager rn;
	rn.InsertPermanentRoad(RailRoadFactory::Line(Point3d(), Point3d(1, 0, 0)));
	rn.InsertPermanentRoad(RailRoadFactory::Line(Point3d(2, 0, 0), Point3d(1, 0, 0)));
	rn.InsertPermanentRoad(RailRoadFactory::Arc(Point3d(2, 0, 0), Degrees(90), Point2d(2, 2), Rotation::AntiClockwise));
	rn.InsertPermanentRoad(RailRoadFactory::Line(Point3d(4, 2, 0), Point3d(4, 4, 0)));

	RoadRoutePtr rrp = rn.GetRoute(Point3d(), Point3d(4, 4, 0));

	BOOST_CHECK(rrp);
	BOOST_CHECK(rrp->Valid());
} 

BOOST_AUTO_TEST_CASE(RoadNetworkTest10)
{
	const Point3d p1(10, 10, 3);
	const Point3d p2(20, 10, 3);
	const Point2d c1(20, 20);
	const Angle a = Degrees(180);
	const Point3d p3(20, 30, 3);
	const Point3d p4(10, 30, 3);

	const auto arcPtr = RailRoadFactory::Arc(p2, a, c1, Rotation::AntiClockwise);
	RailRoadSizer rrs;
	arcPtr->Accept(rrs);
	const float arcLen = rrs.GetLenght();

	RoadNetworkManager rn;
	rn.InsertPermanentRoad(RailRoadFactory::Line(p1, p2));
	rn.InsertPermanentRoad(arcPtr);
	rn.InsertPermanentRoad(RailRoadFactory::Line(p3, p4));

	RoadRoutePtr rrp = rn.GetRoute(p1, p4);

	BOOST_CHECK(rrp);
	BOOST_CHECK(rrp->Valid());

	RoadPoint rp = rrp->GetStartingPoint(Heading::Forward);

	BOOST_CHECK_EQUAL(rp.Get(), p1);
	rp.Move(5.0f);
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(15, 10, 3));
	rp.Move(5.0f);
	BOOST_CHECK_EQUAL(rp.Get(), p2);
	rp.Move(arcLen / 2.0f);
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(30, 20, 3));
	rp.Move(arcLen / 2.0f);
	BOOST_CHECK_EQUAL(rp.Get(), p3);
	rp.Move(5.0f);
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(15, 30, 3));
	rp.Move(5.0f);
	BOOST_CHECK_EQUAL(rp.Get(), p4);
} 

BOOST_AUTO_TEST_CASE(RoadNetworkTest11)
{
	RoadNetworkImpl<int, int> n;

	BOOST_CHECK(n.InsertEdge(0, 1, 0));
	BOOST_CHECK(n.GetEdgeValue(0, 1).second);
	BOOST_CHECK(n.CheckPointExists(0));
	BOOST_CHECK(n.CheckPointExists(1));

	BOOST_CHECK(n.RemoveEdge(0, 1));
	BOOST_CHECK(!n.GetEdgeValue(0, 1).second);
	BOOST_CHECK(!n.CheckPointExists(0));
	BOOST_CHECK(!n.CheckPointExists(1));

	BOOST_CHECK(n.InsertEdge(0, 1, 0));
}

BOOST_AUTO_TEST_CASE(RoadNetworkTest12)
{
	RoadNetworkImpl<int, int> n;

	BOOST_CHECK(n.InsertEdge(0, 1, 0));
	BOOST_CHECK(n.InsertEdge(1, 2, 0));
	BOOST_CHECK(n.InsertEdge(2, 3, 0));
	
	BOOST_CHECK(n.RemoveEdge(1, 2));
	BOOST_CHECK(!n.GetEdgeValue(1, 2).second);
	BOOST_CHECK(n.CheckPointExists(1));
	BOOST_CHECK(n.CheckPointExists(2));
}

BOOST_AUTO_TEST_CASE(RoadNetworkTest13)
{
	RoadNetworkImpl<int, int> n;

	BOOST_CHECK(!n.RemoveEdge(1, 2));
	BOOST_CHECK(n.InsertEdge(0, 1, 0));
	BOOST_CHECK(!n.RemoveEdge(1, 2));
	BOOST_CHECK(n.InsertEdge(2, 3, 0));
	BOOST_CHECK(!n.RemoveEdge(1, 2));
}

BOOST_AUTO_TEST_CASE(RoadNetworkTest14)
{
	RoadNetworkImpl<int, int> n;

	BOOST_CHECK(n.InsertEdge(0, 1, 0));
	BOOST_CHECK(n.InsertEdge(1, 2, 0));
	
	auto r1 = n.GetRoute(0, 2);

	BOOST_CHECK_EQUAL(r1.Size(), 3u);

	auto it = r1.Begin();

	BOOST_CHECK_EQUAL(it.Value(), 0); it.Next();
	BOOST_CHECK_EQUAL(it.Value(), 1); it.Next();
	BOOST_CHECK_EQUAL(it.Value(), 2);

	BOOST_CHECK(n.RemoveEdge(0, 1));
	BOOST_CHECK_EQUAL(n.GetRoute(0, 2).Size(), 0u);
	BOOST_CHECK(!n.CheckPointExists(0));

	BOOST_CHECK(n.InsertEdge(0, 1, 0));
	BOOST_CHECK_EQUAL(n.GetRoute(0, 2).Size(), 3u);
}

BOOST_AUTO_TEST_CASE(RoadNetworkTest15)
{
	RoadNetworkManager rn;
	BOOST_CHECK(rn.InsertPermanentRoad(RailRoadFactory::Line(Point3d(0, 0, 0), Point3d(1, 0, 0))));
	BOOST_CHECK(rn.RemovePermanentRoad(RailRoadFactory::Line(Point3d(0, 0, 0), Point3d(1, 0, 0))));

	// reversed remove
	BOOST_CHECK(rn.InsertPermanentRoad(RailRoadFactory::Line(Point3d(0, 0, 0), Point3d(1, 0, 0))));
	BOOST_CHECK(rn.RemovePermanentRoad(RailRoadFactory::Line(Point3d(1, 0, 0), Point3d(0, 0, 0))));
} 

BOOST_AUTO_TEST_CASE(RoadNetworkTest16)
{
	const Point3d p1(0, 0, 0);
	const Point3d p2(5, 0, 0);
	const Point3d p3(10, 0, 0);
	const Point3d p4(20, 0, 0);

	const RailRoadPtr r = RailRoadFactory::Line(p1, p4);

	RoadNetworkManager rn;
	BOOST_CHECK(rn.InsertPermanentRoad(r));
	
	BOOST_CHECK(rn.GetRoute(p1, p4)->Valid());

	RailRoadIntersections intersections;
	intersections.push_back(RailRoadIntersection(r, p2));
	intersections.push_back(RailRoadIntersection(r, p3));

	rn.InsertTemporaryIntersections(intersections);
	rn.CommitTemporaryRoads();

	RoadRoutePtr rrp1 = rn.GetRoute(p1, p4);
	
	BOOST_CHECK(rrp1->Valid());
	
	RoadPoint rp = rrp1->GetStartingPoint(Heading::Forward);

	BOOST_CHECK_EQUAL(rp.Get(), p1);
	rp.Move(5.0f);
	BOOST_CHECK_EQUAL(rp.Get(), p2);
	rp.Move(5.0f);
	BOOST_CHECK_EQUAL(rp.Get(), p3);
	rp.Move(10.0f);
	BOOST_CHECK_EQUAL(rp.Get(), p4);

	BOOST_CHECK(!rn.InsertPermanentRoad(RailRoadFactory::Line(p1, p2)));
	BOOST_CHECK(!rn.InsertPermanentRoad(RailRoadFactory::Line(p2, p3)));
	BOOST_CHECK(!rn.InsertPermanentRoad(RailRoadFactory::Line(p3, p4)));
	BOOST_CHECK(rn.InsertPermanentRoad(RailRoadFactory::Line(p1, p3)));
	BOOST_CHECK(rn.InsertPermanentRoad(RailRoadFactory::Line(p1, p4)));
	BOOST_CHECK(rn.InsertPermanentRoad(RailRoadFactory::Line(p2, p4)));
} 

// RailRoadIntersection

BOOST_AUTO_TEST_CASE(RailRoadIntersectionTest1)
{
	RailRoadIntersectionMap rrim;

	RailRoadPtr rrl = RailRoadFactory::Line(Point3d(0, 0, 0), Point3d(10, 10, 10));

	rrim.Insert(RailRoadIntersection(rrl, Point3d(1, 1, 1)));
	rrim.Insert(RailRoadIntersection(rrl, Point3d(2, 2, 2)));
	rrim.Insert(RailRoadIntersection(rrl, Point3d(2, 2, 2)));

	RailRoadPtr rra = RailRoadFactory::Arc(Point3d(2, 0, 0), Degrees(90), Point2d(2, 2), Rotation::AntiClockwise);

	rrim.Insert(RailRoadIntersection(rra, Point3d(2, 2, 0)));

	const auto & intersections = rrim.GetIntersections();

	BOOST_CHECK_EQUAL(intersections.size(), 2u);

	BOOST_CHECK_EQUAL(intersections.front().second.size(), 3u);
	BOOST_CHECK_EQUAL(intersections.back().second.size(), 1u);
}
