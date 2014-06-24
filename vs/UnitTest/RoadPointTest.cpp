#include <boost/test/unit_test.hpp>

#include <Point3d.h>
#include <RoadPoint.h>
#include <RoadRoute.h>
#include <RailRoadFactory.h>

using namespace trm;

BOOST_AUTO_TEST_CASE(RoadPointTest1)
{
	RoadRoute::RoadChunks rc;
	rc.push_back(RoadRoute::RoadChunkType(RailRoadFactory::Line(Point3d(), Point3d(1, 0, 0)), Heading::Forward, 1.0f));

	RoadRoute rr(std::move(rc));

	RoadPoint rp = rr.GetStartingPoint(Heading::Forward);

	BOOST_CHECK_EQUAL(rp.Get(), Point3d());
	
	BOOST_CHECK(rp.Move(0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(0.5f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(0.3f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(0.8f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(0.2f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(1.0f, 0.0f, 0.0f));

	BOOST_CHECK(!rp.Move(0.2f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(1.0f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(-0.2f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(0.8f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(-0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(0.3f, 0.0f, 0.0f));

	BOOST_CHECK(!rp.Move(-0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(0.0f, 0.0f, 0.0f));
}

BOOST_AUTO_TEST_CASE(RoadPointTest2)
{
	RoadRoute::RoadChunks rc;
	rc.push_back(RoadRoute::RoadChunkType(RailRoadFactory::Line(Point3d(), Point3d(1, 0, 0)), Heading::Forward, 1.0f));

	RoadRoute rr(std::move(rc));

	RoadPoint rp = rr.GetStartingPoint(Heading::Backward);

	BOOST_CHECK_EQUAL(rp.Get(), Point3d(1, 0, 0));
	
	BOOST_CHECK(rp.Move(0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(0.5f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(0.3f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(0.2f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(0.2f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(0.0f, 0.0f, 0.0f));

	BOOST_CHECK(!rp.Move(0.2f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(0.0f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(-0.2f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(0.2f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(-0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(0.7f, 0.0f, 0.0f));

	BOOST_CHECK(!rp.Move(-0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(1.0f, 0.0f, 0.0f));
}

BOOST_AUTO_TEST_CASE(RoadPointTest3)
{
	RoadRoute::RoadChunks rc;
	rc.push_back(RoadRoute::RoadChunkType(RailRoadFactory::Line(Point3d(), Point3d(1, 0, 0)), Heading::Forward, 1.0f));
	rc.push_back(RoadRoute::RoadChunkType(RailRoadFactory::Line(Point3d(2, 0, 0), Point3d(1, 0, 0)), Heading::Backward, 1.0f));
	rc.push_back(RoadRoute::RoadChunkType(RailRoadFactory::Line(Point3d(2, 0, 0), Point3d(3, 0, 0)), Heading::Forward, 1.0f));

	RoadRoute rr(std::move(rc));

	RoadPoint rp = rr.GetStartingPoint(Heading::Backward);

	BOOST_CHECK_EQUAL(rp.Get(), Point3d(3, 0, 0));
	
	BOOST_CHECK(rp.Move(0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(2.5f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(2.0f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(0.2f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(1.8f, 0.0f, 0.0f));
	 
	BOOST_CHECK(rp.Move(0.2f));
	//BOOST_CHECK_EQUAL(rp.Get(), Point3d(1.6f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(1.0f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(0.6f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(0.1f, 0.0f, 0.0f));

	BOOST_CHECK(!rp.Move(0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(0.0f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(-0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(0.5f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(-0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(1.0f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(-0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(1.5f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(-1.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(3.0f, 0.0f, 0.0f));

	BOOST_CHECK(!rp.Move(-0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(3.0f, 0.0f, 0.0f));
}

BOOST_AUTO_TEST_CASE(RoadPointTest4)
{
	RoadRoute::RoadChunks rc;
	rc.push_back(RoadRoute::RoadChunkType(RailRoadFactory::Line(Point3d(2, 0, 0), Point3d(3, 0, 0)), Heading::Backward, 1.0f));
	rc.push_back(RoadRoute::RoadChunkType(RailRoadFactory::Line(Point3d(2, 0, 0), Point3d(1, 0, 0)), Heading::Forward, 1.0f));
	rc.push_back(RoadRoute::RoadChunkType(RailRoadFactory::Line(Point3d(), Point3d(1, 0, 0)), Heading::Backward, 1.0f));
	
	RoadRoute rr(std::move(rc));

	BOOST_CHECK(rr.Valid());

	RoadPoint rp = rr.GetStartingPoint(Heading::Forward);

	BOOST_CHECK_EQUAL(rp.Get(), Point3d(3, 0, 0));
	
	BOOST_CHECK(rp.Move(0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(2.5f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(2.0f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(0.2f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(1.8f, 0.0f, 0.0f));
	 
	BOOST_CHECK(rp.Move(0.2f));
	//BOOST_CHECK_EQUAL(rp.Get(), Point3d(1.6f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(1.0f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(0.6f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(0.1f, 0.0f, 0.0f));

	BOOST_CHECK(!rp.Move(0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(0.0f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(-0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(0.5f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(-0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(1.0f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(-0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(1.5f, 0.0f, 0.0f));

	BOOST_CHECK(rp.Move(-1.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(3.0f, 0.0f, 0.0f));

	BOOST_CHECK(!rp.Move(-0.5f));
	BOOST_CHECK_EQUAL(rp.Get(), Point3d(3.0f, 0.0f, 0.0f));
}


BOOST_AUTO_TEST_CASE(RoadPointTest5)
{
	RoadRoute::RoadChunks rc;
	
	RoadRoute rr(std::move(rc));

	BOOST_CHECK(!rr.Valid());
	BOOST_CHECK_THROW(rr.GetStartingPoint(Heading::Forward), std::runtime_error);
}