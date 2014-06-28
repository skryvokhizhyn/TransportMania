//#define BOOST_TEST_DYN_LINK

#include <TriangleNode.h>
#include <TriangleNodeHandler.h>
#include <HeightMap.h>
#include <Variance.h>
#include <Point3d.h>
#include <Point2d.h>
#include <GeometryUtils.h>
#include <Logger.h>
#include <ModelData.h>
#include <boost/test/unit_test.hpp>

using namespace trm::terrain;
using namespace trm::terrain::lod;
using namespace trm;

namespace
{
	const HeightMap::Type v3x3[] = 
	{
		0, 0, 0,
		0, 0, 0,
		2, 0, 1
	};
	const size_t level = 2;
}

#include <Triangle2d.h>
#include <TriangleMapper.h>

BOOST_AUTO_TEST_CASE(TriangleNodeTest1)
{
	HeightMap hm;
	hm.Load(v3x3);
	Variance v;
	v.Generate(level, hm, utils::GetTriangles(hm).first);

	const Triangle3d t3d = utils::GetTriangles(hm).first;
	const Triangle2d t2d = Triangle2d(Point2d::Cast(t3d.l()), Point2d::Cast(t3d.e()), Point2d::Cast(t3d.r()));
	const TriangleMapper tm(t2d);

	TriangleNode tn;
	TriangleNodeHandler::Tasselate(&tn, level, v, hm, tm, Point3d());

	ModelData md;
	PointNormaleMap nm;

	TriangleNodeHandler::Render(&tn, level, hm, tm, md, nm);

	BOOST_CHECK_EQUAL(md.points.size(), 6u);
}

namespace ttl = trm::terrain::lod;

class /*ttl::*/TriangleNodeTester
{
	// helper methods
public:
	template<Direction head, Direction... tail>
	static TriangleNode * Traverse(TriangleNode * tn)
	{
		TriangleNode * pChild = Traverse<head>(tn);
		return Traverse<tail...>(pChild);
	}

	template<>
	static TriangleNode * Traverse<Direction::Left>(TriangleNode * tn)
	{
		tn->Split();
		return tn->GetLChild();
	}

	template<>
	static TriangleNode * Traverse<Direction::Right>(TriangleNode * tn)
	{
		tn->Split();
		return tn->GetRChild();
	}

	// test scenarios
public:
	static void TestSplit();
	static void TestSplitLeft();
	static void TestSplitRight();
	static void TestSplitComplex();
	static void TestSplitIndirect();
	static void TestSplitBase();
	static void TestSplitLNeighbor();
	static void TestSplitRNeighbor();
	static void TestSplitLRNeighbor();
	static void TestSplitRLNeighbor();
	static void TestMerge();
	static void TestMergeMultiple();
	static void TestMergeBase();
	static void TestMergeLNeighbor();
	static void TestMergeRNeighbor();
	static void TestMergeLRNeighbor();
	static void TestMergeRLNeighbor();
	static void TestMergeCausedSplittingSingle();
	static void TestMergeCausedSplittingMultiple();
	static void TestMergeClearCause();
	static void TestComplexStackOverflow();
	static void TestComplexStackOverflow2();
};

BOOST_FIXTURE_TEST_CASE(TriangleNodeTestSplit, TriangleNodeTester)
{TestSplit();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestSplitLeft, TriangleNodeTester)
{TestSplitLeft();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestSplitRight, TriangleNodeTester)
{TestSplitRight();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestSplitComplex, TriangleNodeTester)
{TestSplitComplex();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestSplitIndirect, TriangleNodeTester)
{TestSplitIndirect();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestSplitBase, TriangleNodeTester)
{TestSplitBase();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestSplitLNeighbor, TriangleNodeTester)
{TestSplitLNeighbor();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestSplitRNeighbor, TriangleNodeTester)
{TestSplitRNeighbor();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestSplitLRNeighbor, TriangleNodeTester)
{TestSplitLRNeighbor();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestSplitRLNeighbor, TriangleNodeTester)
{TestSplitRLNeighbor();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestMerge, TriangleNodeTester)
{TestMerge();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestMergeMultiple, TriangleNodeTester)
{TestMergeMultiple();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestMergeBase, TriangleNodeTester)
{TestMergeBase();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestMergeLNeighbor, TriangleNodeTester)
{TestMergeLNeighbor();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestMergeRNeighbor, TriangleNodeTester)
{TestMergeRNeighbor();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestMergeLRNeighbor, TriangleNodeTester)
{TestMergeLRNeighbor();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestMergeRLNeighbor, TriangleNodeTester)
{TestMergeRLNeighbor();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestMergeCausedSplittingSingle, TriangleNodeTester)
{TestMergeCausedSplittingSingle();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestMergeCausedSplittingMultiple, TriangleNodeTester)
{TestMergeCausedSplittingMultiple();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestMergeClearCause, TriangleNodeTester)
{TestMergeClearCause();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestComplexStackOverflow, TriangleNodeTester)
{TestComplexStackOverflow();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestComplexStackOverflow2, TriangleNodeTester)
{TestComplexStackOverflow2();}

void /*ttl::*/TriangleNodeTester::TestSplit()
{
	TriangleNode tn;
	Traverse<Direction::Left>(&tn);

	BOOST_CHECK(tn.Splitted());
}

void /*ttl::*/TriangleNodeTester::TestSplitLeft()
{
	TriangleNode tn;
	TriangleNode * pLeft = Traverse<Direction::Left>(&tn);

	BOOST_CHECK_EQUAL(tn.GetLChild(), pLeft);
}

void /*ttl::*/TriangleNodeTester::TestSplitRight()
{
	TriangleNode tn;
	TriangleNode * pRight = Traverse<Direction::Right>(&tn);

	BOOST_CHECK_EQUAL(tn.GetRChild(), pRight);
}

void /*ttl::*/TriangleNodeTester::TestSplitComplex()
{
	TriangleNode tn;
	TriangleNode * pChild = Traverse<Direction::Right, Direction::Left, Direction::Right, Direction::Left>(&tn);

	BOOST_CHECK_EQUAL(tn.GetRChild()->GetLChild()->GetRChild()->GetLChild(), pChild);
}

void /*ttl::*/TriangleNodeTester::TestSplitIndirect()
{
	TriangleNode tn;
	Traverse<Direction::Left, Direction::Left, Direction::Left>(&tn);

	BOOST_CHECK(tn.GetRChild()->Splitted());
	BOOST_CHECK(tn.GetRChild()->GetRChild()->Splitted());
}

void /*ttl::*/TriangleNodeTester::TestSplitBase()
{
	TriangleNode tnUp, tnDown;
	tnUp.SetBase(&tnDown);
	tnDown.SetBase(&tnUp);

	Traverse<Direction::Right>(&tnUp);

	BOOST_CHECK(tnDown.Splitted());
}

void /*ttl::*/TriangleNodeTester::TestSplitLNeighbor()
{
	TriangleNode tn, tnLNeighbor;
	tn.SetLNeighbor(&tnLNeighbor);
	tnLNeighbor.SetLNeighbor(&tn);

	Traverse<Direction::Left, Direction::Left>(&tn);

	BOOST_CHECK(tnLNeighbor.Splitted());
	BOOST_CHECK(tnLNeighbor.GetLChild()->Splitted());
	BOOST_CHECK(!tnLNeighbor.GetRChild()->Splitted());
}

void /*ttl::*/TriangleNodeTester::TestSplitRNeighbor()
{
	TriangleNode tn, tnRNeighbor;
	tn.SetRNeighbor(&tnRNeighbor);
	tnRNeighbor.SetRNeighbor(&tn);

	Traverse<Direction::Right, Direction::Right>(&tn);

	BOOST_CHECK(tnRNeighbor.Splitted());
	BOOST_CHECK(tnRNeighbor.GetRChild()->Splitted());
	BOOST_CHECK(!tnRNeighbor.GetLChild()->Splitted());
}

void /*ttl::*/TriangleNodeTester::TestSplitLRNeighbor()
{
	TriangleNode tn, tnLNeighbor;
	tn.SetLNeighbor(&tnLNeighbor);
	tnLNeighbor.SetRNeighbor(&tn);

	Traverse<Direction::Left, Direction::Left>(&tn);

	BOOST_CHECK(tnLNeighbor.Splitted());
	BOOST_CHECK(tnLNeighbor.GetRChild()->Splitted());
	BOOST_CHECK(!tnLNeighbor.GetLChild()->Splitted());
}

void /*ttl::*/TriangleNodeTester::TestSplitRLNeighbor()
{
	TriangleNode tn, tnRNeighbor;
	tn.SetRNeighbor(&tnRNeighbor);
	tnRNeighbor.SetLNeighbor(&tn);

	Traverse<Direction::Right, Direction::Right>(&tn);

	BOOST_CHECK(tnRNeighbor.Splitted());
	BOOST_CHECK(tnRNeighbor.GetLChild()->Splitted());
	BOOST_CHECK(!tnRNeighbor.GetRChild()->Splitted());
}

void /*ttl::*/TriangleNodeTester::TestMerge()
{
	TriangleNode tn;
	
	Traverse<Direction::Left>(&tn);
	tn.Merge(TriangleNode::ProcessCause::Clear, TriangleNode::ProcessBase::Merge);

	BOOST_CHECK(!tn.Splitted());
}

void /*ttl::*/TriangleNodeTester::TestMergeMultiple()
{
	TriangleNode tn;
	
	Traverse<Direction::Left, Direction::Left>(&tn);
	tn.Merge(TriangleNode::ProcessCause::Clear, TriangleNode::ProcessBase::Merge);

	BOOST_CHECK(!tn.Splitted());
}

void /*ttl::*/TriangleNodeTester::TestMergeBase()
{
	TriangleNode tn, tnBase;
	tn.SetBase(&tnBase);
	tnBase.SetBase(&tn);
	
	Traverse<Direction::Left>(&tn);
	tn.Merge(TriangleNode::ProcessCause::Clear, TriangleNode::ProcessBase::Merge);

	BOOST_CHECK(!tnBase.Splitted());
}

void /*ttl::*/TriangleNodeTester::TestMergeLNeighbor()
{
	TriangleNode tn, tnLNeighbor;
	tn.SetLNeighbor(&tnLNeighbor);
	tnLNeighbor.SetLNeighbor(&tn);
	
	Traverse<Direction::Left, Direction::Left>(&tn);
	tn.Merge(TriangleNode::ProcessCause::Clear, TriangleNode::ProcessBase::Merge);

	// we don't merge parent but only direct base
	// thus neighbor stays splitted but its children - merged
	BOOST_CHECK(tnLNeighbor.Splitted());
	BOOST_CHECK(!tnLNeighbor.GetLChild()->Splitted());
	BOOST_CHECK(!tnLNeighbor.GetRChild()->Splitted());
}

void /*ttl::*/TriangleNodeTester::TestMergeRNeighbor()
{
	TriangleNode tn, tnRNeighbor;
	tn.SetRNeighbor(&tnRNeighbor);
	tnRNeighbor.SetRNeighbor(&tn);
	
	Traverse<Direction::Right, Direction::Right>(&tn);
	tn.Merge(TriangleNode::ProcessCause::Clear, TriangleNode::ProcessBase::Merge);

	// we don't merge parent but only direct base
	// thus neighbor stays splitted but its children - merged
	BOOST_CHECK(tnRNeighbor.Splitted());
	BOOST_CHECK(!tnRNeighbor.GetLChild()->Splitted());
	BOOST_CHECK(!tnRNeighbor.GetRChild()->Splitted());
}

void /*ttl::*/TriangleNodeTester::TestMergeLRNeighbor()
{
	TriangleNode tn, tnLNeighbor;
	tn.SetLNeighbor(&tnLNeighbor);
	tnLNeighbor.SetRNeighbor(&tn);
	
	Traverse<Direction::Left, Direction::Left>(&tn);
	tn.Merge(TriangleNode::ProcessCause::Clear, TriangleNode::ProcessBase::Merge);

	// we don't merge parent but only direct base
	// thus neighbor stays splitted but its children - merged
	BOOST_CHECK(tnLNeighbor.Splitted());
	BOOST_CHECK(!tnLNeighbor.GetLChild()->Splitted());
	BOOST_CHECK(!tnLNeighbor.GetRChild()->Splitted());
}

void /*ttl::*/TriangleNodeTester::TestMergeRLNeighbor()
{
	TriangleNode tn, tnRNeighbor;
	tn.SetRNeighbor(&tnRNeighbor);
	tnRNeighbor.SetLNeighbor(&tn);
	
	Traverse<Direction::Right, Direction::Right>(&tn);
	tn.Merge(TriangleNode::ProcessCause::Clear, TriangleNode::ProcessBase::Merge);

	// we don't merge parent but only direct base
	// thus neighbor stays splitted but its children - merged
	BOOST_CHECK(tnRNeighbor.Splitted());
	BOOST_CHECK(!tnRNeighbor.GetLChild()->Splitted());
	BOOST_CHECK(!tnRNeighbor.GetRChild()->Splitted());
}

void /*ttl::*/TriangleNodeTester::TestMergeCausedSplittingSingle()
{
	TriangleNode tn;

	TriangleNode * pMiddleLeft = Traverse<Direction::Left>(&tn);
	Traverse<Direction::Left>(pMiddleLeft);

	pMiddleLeft->SetClearCause(true, false);

	tn.Merge(TriangleNode::ProcessCause::Preserve, TriangleNode::ProcessBase::Merge);

	BOOST_CHECK(pMiddleLeft->Splitted());
	BOOST_CHECK(tn.Splitted());

	pMiddleLeft->SetClearCause(false, true);

	tn.Merge(TriangleNode::ProcessCause::Preserve, TriangleNode::ProcessBase::Merge);

	BOOST_CHECK(!tn.Splitted());
}

void /*ttl::*/TriangleNodeTester::TestMergeCausedSplittingMultiple()
{
	TriangleNode tn, tnNLeft;
	tn.SetLNeighbor(&tnNLeft);
	tnNLeft.SetLNeighbor(&tn);

	Traverse<Direction::Left, Direction::Left>(&tn);
	TriangleNode * pMiddleChild = Traverse<Direction::Left>(&tnNLeft);

	pMiddleChild->SetClearCause(true, false);

	tn.Merge(TriangleNode::ProcessCause::Preserve, TriangleNode::ProcessBase::Merge);

	BOOST_CHECK(tn.Splitted());
	BOOST_CHECK(tn.GetLChild()->Splitted());
	BOOST_CHECK(tnNLeft.Splitted());
	BOOST_CHECK(tnNLeft.GetLChild()->Splitted());

	pMiddleChild->SetClearCause(false, true);

	tnNLeft.Merge(TriangleNode::ProcessCause::Preserve, TriangleNode::ProcessBase::Merge);

	BOOST_CHECK(!tnNLeft.Splitted());
	
	// we don't merge parent but only direct base
	BOOST_CHECK(tn.Splitted());
}

void /*ttl::*/TriangleNodeTester::TestMergeClearCause()
{
	TriangleNode tn, tnBase;
	tn.SetBase(&tnBase);
	tnBase.SetBase(&tn);

	Traverse<Direction::Left>(&tn);

	tn.SetClearCause(true, false);

	tn.Merge(TriangleNode::ProcessCause::Clear, TriangleNode::ProcessBase::Merge);

	BOOST_CHECK(!tn.Splitted());
	BOOST_CHECK(!tnBase.Splitted());
}

void /*ttl::*/TriangleNodeTester::TestComplexStackOverflow()
{
	TriangleNode tn;

	Traverse<Direction::Right, Direction::Left, Direction::Left, Direction::Left>(&tn);

	tn.Merge(TriangleNode::ProcessCause::Preserve, TriangleNode::ProcessBase::Merge);

	BOOST_CHECK(!tn.Splitted());
}

void /*ttl::*/TriangleNodeTester::TestComplexStackOverflow2()
{
	TriangleNode tn;

	Traverse<
		Direction::Left
		, Direction::Left
		, Direction::Left
		, Direction::Left
		, Direction::Left
		, Direction::Right
		, Direction::Left
		, Direction::Left
	>(&tn);

	tn.Merge(TriangleNode::ProcessCause::Preserve, TriangleNode::ProcessBase::Merge);

	BOOST_CHECK(!tn.Splitted());
}