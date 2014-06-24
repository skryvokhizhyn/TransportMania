#include <boost/test/unit_test.hpp>

#include <RoadNetworkImpl.h>
#include <RoadNetwork.h>
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

	auto r1 = n.GetEdge(0, 1);
	BOOST_CHECK(r1.second);
	BOOST_CHECK_EQUAL(r1.first, 0);

	auto r2 = n.GetEdge(0, 3);
	BOOST_CHECK_EQUAL(r2.second, false);

	auto r3 = n.GetEdge(2, 0);
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
	RoadNetwork rn;
	rn.Insert(RailRoadFactory::Line(Point3d(), Point3d(1, 0, 0)));
	rn.Insert(RailRoadFactory::Line(Point3d(2, 0, 0), Point3d(1, 0, 0)));
	rn.Insert(RailRoadFactory::Arc(Point3d(2, 0, 0), Degrees(90), Point2d(2, 2), Direction::Right));
	rn.Insert(RailRoadFactory::Line(Point3d(4, 2, 0), Point3d(4, 4, 0)));

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

	const auto arcPtr = RailRoadFactory::Arc(p2, a, c1, Direction::Right);
	RailRoadSizer rrs;
	arcPtr->Accept(rrs);
	const float arcLen = rrs.GetLenght();

	RoadNetwork rn;
	rn.Insert(RailRoadFactory::Line(p1, p2));
	rn.Insert(arcPtr);
	rn.Insert(RailRoadFactory::Line(p3, p4));

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

/*
	4212726775 1
	4223548445 2

	4223548445 2
	4223416913 1

	4223416913 2
	4212860331 1
*/