#include <boost/test/unit_test.hpp>

#include <RailRoadArc.h>
#include <RailRoadLine.h>

#include <RailRoadSplitter.h>

using namespace trm;

BOOST_AUTO_TEST_CASE(RailRoadSplitterTest0)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadSplitter rrs1({Point3d(-1, -1, 0)});
	rrl.Accept(rrs1);

	BOOST_CHECK(rrs1.GetSplitResult().empty());
}

BOOST_AUTO_TEST_CASE(RailRoadSplitterTest1)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadSplitter rrs1({Point3d(-1, 0, 0)});
	rrl.Accept(rrs1);

	BOOST_CHECK(rrs1.GetSplitResult().empty());
}

namespace
{
	bool CheckRoadsEqual(const RailRoadLine & rrl, const RailRoadPtr & r2)
	{
		using RailRoadLinePtr = std::shared_ptr<RailRoadLine>;
		RailRoadLinePtr r2ToLine = std::dynamic_pointer_cast<RailRoadLine>(r2);

		if (!r2ToLine)
			return false;

		if (rrl.GetStart() != r2ToLine->GetStart())
		{
			return false;
		}

		if (rrl.GetEnd() != r2ToLine->GetEnd())
		{
			return false;
		}

		return true;
	}

	bool CheckRoadsEqual(const RailRoadArc & rra, const RailRoadPtr & r2)
	{
		using RailRoadArcPtr = std::shared_ptr<RailRoadArc>;
		RailRoadArcPtr r2ToArc = std::dynamic_pointer_cast<RailRoadArc>(r2);

		if (!r2ToArc)
			return false;

		if (rra.GetStart() != r2ToArc->GetStart())
		{
			return false;
		}

		if (rra.GetAngle() != r2ToArc->GetAngle())
		{
			return false;
		}

		if (rra.GetCenter() != r2ToArc->GetCenter())
		{
			return false;
		}

		return true;
	}
}

BOOST_AUTO_TEST_CASE(RailRoadSplitterTest2)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadSplitter rrs1({Point3d(0, 0, 0)});
	rrl.Accept(rrs1);

	BOOST_CHECK(rrs1.GetSplitResult().empty());

	RailRoadSplitter rrs2({Point3d(2, 0, 0)});
	rrl.Accept(rrs2);

	BOOST_CHECK(rrs2.GetSplitResult().empty());
}

BOOST_AUTO_TEST_CASE(RailRoadSplitterTest3)
{
	RailRoadLine rrl(Point3d(0, 0, 0), Point3d(2, 0, 0));

	RailRoadSplitter rrs1({Point3d(1, 0, 0)});
	rrl.Accept(rrs1);

	BOOST_CHECK_EQUAL(rrs1.GetSplitResult().size(), 2u);
	BOOST_CHECK(CheckRoadsEqual(RailRoadLine(Point3d(0, 0, 0), Point3d(1, 0, 0)), rrs1.GetSplitResult().front()));
	BOOST_CHECK(CheckRoadsEqual(RailRoadLine(Point3d(1, 0, 0), Point3d(2, 0, 0)), rrs1.GetSplitResult().back()));
}

BOOST_AUTO_TEST_CASE(RailRoadSplitterTest4)
{
	RailRoadArc rra(Point3d(0, 0, 0), Degrees(90), Point2d(0, 2));

	RailRoadSplitter rrs1({Point3d(-1, -1, 0)});
	rra.Accept(rrs1);

	BOOST_CHECK(rrs1.GetSplitResult().empty());

	RailRoadSplitter rrs2({Point3d(0, 0, 0)});
	rra.Accept(rrs2);

	BOOST_CHECK(rrs2.GetSplitResult().empty());

	RailRoadSplitter rrs3({Point3d(2, 2, 0)});
	rra.Accept(rrs3);

	BOOST_CHECK(rrs3.GetSplitResult().empty());

	RailRoadSplitter rrs4({Point3d(-2, 2, 0)});
	rra.Accept(rrs4);

	BOOST_CHECK(rrs4.GetSplitResult().empty());
}

BOOST_AUTO_TEST_CASE(RailRoadSplitterTest5)
{
	RailRoadArc rra(Point3d(0, 0, 0), Degrees(180), Point2d(0, 2));
	
	RailRoadSplitter rrs1({Point3d(2, 2, 0)});
	rra.Accept(rrs1);

	BOOST_CHECK_EQUAL(rrs1.GetSplitResult().size(), 2u);

	BOOST_CHECK(CheckRoadsEqual(
		RailRoadArc(Point3d(0, 0, 0), Degrees(90), Point2d(0, 2)), 
		rrs1.GetSplitResult().front()));
	BOOST_CHECK(CheckRoadsEqual(
		RailRoadArc(Point3d(2, 2, 0), Degrees(90), Point2d(0, 2)),
		rrs1.GetSplitResult().back()));
}

BOOST_AUTO_TEST_CASE(RailRoadSplitterTest6)
{
	RailRoadArc rra(Point3d(0, 0, 0), Degrees(-180), Point2d(0, 2));
	
	RailRoadSplitter rrs1({Point3d(-2, 2, 0)});
	rra.Accept(rrs1);

	BOOST_CHECK_EQUAL(rrs1.GetSplitResult().size(), 2u);

	BOOST_CHECK(CheckRoadsEqual(
		RailRoadArc(Point3d(0, 0, 0), Degrees(-90), Point2d(0, 2)), 
		rrs1.GetSplitResult().front()));
	BOOST_CHECK(CheckRoadsEqual(
		RailRoadArc(Point3d(-2, 2, 0), Degrees(-90), Point2d(0, 2)),
		rrs1.GetSplitResult().back()));
}
