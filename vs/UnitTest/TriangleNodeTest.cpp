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
#include <PointNormaleMap.h>
#include <boost/test/unit_test.hpp>

using namespace trm::terrain;
using namespace trm::terrain::lod;
using namespace trm;

namespace
{
	template<Direction head, Direction... tail>
	TriangleNode * Traverse(TriangleNode * tn)
	{
		TriangleNode * pChild = Traverse<head>(tn);
		return Traverse<tail...>(pChild);
	}

	template<>
	TriangleNode * Traverse<Direction::Left>(TriangleNode * tn)
	{
		tn->Split();
		return tn->GetLChild();
	}

	template<>
	TriangleNode * Traverse<Direction::Right>(TriangleNode * tn)
	{
		tn->Split();
		return tn->GetRChild();
	}
}

#include <Triangle2d.h>
#include <TriangleMapper.h>

BOOST_AUTO_TEST_CASE(TriangleNodeTest1)
{
	HeightMap hm;

	auto vec = std::vector<float>(
	{
		0, 0, 0,
		0, 0, 0,
		2, 0, 1
	});

	HeightMap::Container hmt = PrepareDataFromVectorReversed(&vec[0], vec.size());
	hm.Swap(hmt);
	const size_t level = 2;
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

class TriangleNodeTester
{
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
	static void TestMergeClearCause1();
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
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestMergeClearCause1, TriangleNodeTester)
{TestMergeClearCause1();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestComplexStackOverflow, TriangleNodeTester)
{TestComplexStackOverflow();}
BOOST_FIXTURE_TEST_CASE(TriangleNodeTestComplexStackOverflow2, TriangleNodeTester)
{TestComplexStackOverflow2();}

void TriangleNodeTester::TestSplit()
{
	TriangleNode tn;
	Traverse<Direction::Left>(&tn);

	BOOST_CHECK(tn.Splitted());
}

void TriangleNodeTester::TestSplitLeft()
{
	TriangleNode tn;
	TriangleNode * pLeft = Traverse<Direction::Left>(&tn);

	BOOST_CHECK_EQUAL(tn.GetLChild(), pLeft);
}

void TriangleNodeTester::TestSplitRight()
{
	TriangleNode tn;
	TriangleNode * pRight = Traverse<Direction::Right>(&tn);

	BOOST_CHECK_EQUAL(tn.GetRChild(), pRight);
}

void TriangleNodeTester::TestSplitComplex()
{
	TriangleNode tn;
	TriangleNode * pChild = Traverse<Direction::Right, Direction::Left, Direction::Right, Direction::Left>(&tn);

	BOOST_CHECK_EQUAL(tn.GetRChild()->GetLChild()->GetRChild()->GetLChild(), pChild);
}

void TriangleNodeTester::TestSplitIndirect()
{
	TriangleNode tn;
	Traverse<Direction::Left, Direction::Left, Direction::Left>(&tn);

	BOOST_CHECK(tn.GetRChild()->Splitted());
	BOOST_CHECK(tn.GetRChild()->GetRChild()->Splitted());
}

void TriangleNodeTester::TestSplitBase()
{
	TriangleNode tnUp, tnDown;
	tnUp.SetBase(&tnDown);
	tnDown.SetBase(&tnUp);

	Traverse<Direction::Right>(&tnUp);

	BOOST_CHECK(tnDown.Splitted());
}

void TriangleNodeTester::TestSplitLNeighbor()
{
	TriangleNode tn, tnLNeighbor;
	tn.SetLNeighbor(&tnLNeighbor);
	tnLNeighbor.SetLNeighbor(&tn);

	Traverse<Direction::Left, Direction::Left>(&tn);

	BOOST_CHECK(tnLNeighbor.Splitted());
	BOOST_CHECK(tnLNeighbor.GetLChild()->Splitted());
	BOOST_CHECK(!tnLNeighbor.GetRChild()->Splitted());
}

void TriangleNodeTester::TestSplitRNeighbor()
{
	TriangleNode tn, tnRNeighbor;
	tn.SetRNeighbor(&tnRNeighbor);
	tnRNeighbor.SetRNeighbor(&tn);

	Traverse<Direction::Right, Direction::Right>(&tn);

	BOOST_CHECK(tnRNeighbor.Splitted());
	BOOST_CHECK(tnRNeighbor.GetRChild()->Splitted());
	BOOST_CHECK(!tnRNeighbor.GetLChild()->Splitted());
}

void TriangleNodeTester::TestSplitLRNeighbor()
{
	TriangleNode tn, tnLNeighbor;
	tn.SetLNeighbor(&tnLNeighbor);
	tnLNeighbor.SetRNeighbor(&tn);

	Traverse<Direction::Left, Direction::Left>(&tn);

	BOOST_CHECK(tnLNeighbor.Splitted());
	BOOST_CHECK(tnLNeighbor.GetRChild()->Splitted());
	BOOST_CHECK(!tnLNeighbor.GetLChild()->Splitted());
}

void TriangleNodeTester::TestSplitRLNeighbor()
{
	TriangleNode tn, tnRNeighbor;
	tn.SetRNeighbor(&tnRNeighbor);
	tnRNeighbor.SetLNeighbor(&tn);

	Traverse<Direction::Right, Direction::Right>(&tn);

	BOOST_CHECK(tnRNeighbor.Splitted());
	BOOST_CHECK(tnRNeighbor.GetLChild()->Splitted());
	BOOST_CHECK(!tnRNeighbor.GetRChild()->Splitted());
}

void TriangleNodeTester::TestMerge()
{
	TriangleNode tn;
	
	Traverse<Direction::Left>(&tn);
	tn.Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Delete);

	BOOST_CHECK(!tn.Splitted());
}

void TriangleNodeTester::TestMergeMultiple()
{
	TriangleNode tn;
	
	Traverse<Direction::Left, Direction::Left>(&tn);
	tn.Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Delete);

	BOOST_CHECK(!tn.Splitted());
}

void TriangleNodeTester::TestMergeBase()
{
	TriangleNode tn, tnBase;
	tn.SetBase(&tnBase);
	tnBase.SetBase(&tn);
	
	Traverse<Direction::Left>(&tn);
	tn.Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Delete);

	BOOST_CHECK(!tnBase.Splitted());
}

void TriangleNodeTester::TestMergeLNeighbor()
{
	TriangleNode tn, tnLNeighbor;
	tn.SetLNeighbor(&tnLNeighbor);
	tnLNeighbor.SetLNeighbor(&tn);
	
	Traverse<Direction::Left, Direction::Left>(&tn);
	tn.Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Delete);

	// we don't merge parent but only direct base
	// thus neighbor stays splitted but its children - merged
	BOOST_CHECK(tnLNeighbor.Splitted());
	BOOST_CHECK(!tnLNeighbor.GetLChild()->Splitted());
	BOOST_CHECK(!tnLNeighbor.GetRChild()->Splitted());
}

void TriangleNodeTester::TestMergeRNeighbor()
{
	TriangleNode tn, tnRNeighbor;
	tn.SetRNeighbor(&tnRNeighbor);
	tnRNeighbor.SetRNeighbor(&tn);
	
	Traverse<Direction::Right, Direction::Right>(&tn);
	tn.Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Delete);

	// we don't merge parent but only direct base
	// thus neighbor stays splitted but its children - merged
	BOOST_CHECK(tnRNeighbor.Splitted());
	BOOST_CHECK(!tnRNeighbor.GetLChild()->Splitted());
	BOOST_CHECK(!tnRNeighbor.GetRChild()->Splitted());
}

void TriangleNodeTester::TestMergeLRNeighbor()
{
	TriangleNode tn, tnLNeighbor;
	tn.SetLNeighbor(&tnLNeighbor);
	tnLNeighbor.SetRNeighbor(&tn);
	
	Traverse<Direction::Left, Direction::Left>(&tn);
	tn.Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Delete);

	// we don't merge parent but only direct base
	// thus neighbor stays splitted but its children - merged
	BOOST_CHECK(tnLNeighbor.Splitted());
	BOOST_CHECK(!tnLNeighbor.GetLChild()->Splitted());
	BOOST_CHECK(!tnLNeighbor.GetRChild()->Splitted());
}

void TriangleNodeTester::TestMergeRLNeighbor()
{
	TriangleNode tn, tnRNeighbor;
	tn.SetRNeighbor(&tnRNeighbor);
	tnRNeighbor.SetLNeighbor(&tn);
	
	Traverse<Direction::Right, Direction::Right>(&tn);
	tn.Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Delete);

	// we don't merge parent but only direct base
	// thus neighbor stays splitted but its children - merged
	BOOST_CHECK(tnRNeighbor.Splitted());
	BOOST_CHECK(!tnRNeighbor.GetLChild()->Splitted());
	BOOST_CHECK(!tnRNeighbor.GetRChild()->Splitted());
}

void TriangleNodeTester::TestMergeCausedSplittingSingle()
{
	TriangleNode tn;

	TriangleNode * pMiddleLeft = Traverse<Direction::Left>(&tn);
	Traverse<Direction::Left>(pMiddleLeft);

	pMiddleLeft->SetClearCause(true, TriangleNode::RecursiveMode::No);

	tn.Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Delete);

	BOOST_CHECK(pMiddleLeft->Splitted());
	BOOST_CHECK(tn.Splitted());

	pMiddleLeft->SetClearCause(false, TriangleNode::RecursiveMode::Yes);

	tn.Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Delete);

	BOOST_CHECK(!tn.Splitted());
}

void TriangleNodeTester::TestMergeCausedSplittingMultiple()
{
	TriangleNode tn, tnNLeft;
	tn.SetLNeighbor(&tnNLeft);
	tnNLeft.SetLNeighbor(&tn);

	Traverse<Direction::Left, Direction::Left>(&tn);
	TriangleNode * pMiddleChild = Traverse<Direction::Left>(&tnNLeft);

	pMiddleChild->SetClearCause(true, TriangleNode::RecursiveMode::No);

	tn.Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Delete);

	BOOST_CHECK(tn.Splitted());
	BOOST_CHECK(tn.GetLChild()->Splitted());
	BOOST_CHECK(tnNLeft.Splitted());
	BOOST_CHECK(tnNLeft.GetLChild()->Splitted());

	pMiddleChild->SetClearCause(false, TriangleNode::RecursiveMode::Yes);

	tnNLeft.Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Delete);

	BOOST_CHECK(!tnNLeft.Splitted());
	
	// we don't merge parent but only direct base
	BOOST_CHECK(tn.Splitted());
}

void TriangleNodeTester::TestMergeClearCause()
{
	TriangleNode tn, tnBase;
	tn.SetBase(&tnBase);
	tnBase.SetBase(&tn);

	Traverse<Direction::Left>(&tn);

	tn.SetClearCause(true, TriangleNode::RecursiveMode::No);

	tn.Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Delete);

	BOOST_CHECK(tn.Splitted());
	BOOST_CHECK(tnBase.Splitted());
}

void TriangleNodeTester::TestMergeClearCause1()
{
	TriangleNode tn, tnBase;
	tn.SetBase(&tnBase);
	tnBase.SetBase(&tn);

	Traverse<Direction::Left>(&tn);

	tn.SetClearCause(false, TriangleNode::RecursiveMode::No);

	tn.Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Delete);

	BOOST_CHECK(!tn.Splitted());
	BOOST_CHECK(!tnBase.Splitted());
}

void TriangleNodeTester::TestComplexStackOverflow()
{
	TriangleNode tn;

	Traverse<Direction::Right, Direction::Left, Direction::Left, Direction::Left>(&tn);

	tn.Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Delete);

	BOOST_CHECK(!tn.Splitted());
}

void TriangleNodeTester::TestComplexStackOverflow2()
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

	tn.Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Delete);

	BOOST_CHECK(!tn.Splitted());
}

#include <TriangleNodeCache.h>
 
BOOST_AUTO_TEST_CASE(TriangleNodeCacheTest1)
{
	TriangleNodeCache::TriangleNodes nodes;
	TriangleNode tn;

	nodes.push_back(&tn);

	TriangleNodeCache::Update(nodes);

	BOOST_CHECK_EQUAL(nodes.size(), 1u);
}

BOOST_AUTO_TEST_CASE(TriangleNodeCacheTest2)
{
	TriangleNodeCache::TriangleNodes nodes;
	TriangleNode tn;

	nodes.push_back(&tn);

	Traverse<Direction::Left>(&tn);

	TriangleNodeCache::Update(nodes);

	BOOST_CHECK(tn.Splitted());
	BOOST_CHECK_EQUAL(nodes.size(), 2u);
	auto it = nodes.begin();
	BOOST_CHECK_EQUAL(*it++, tn.GetLChild());
	BOOST_CHECK_EQUAL(*it, tn.GetRChild());
}

BOOST_AUTO_TEST_CASE(TriangleNodeCacheTest3)
{
	TriangleNodeCache::TriangleNodes nodes;
	TriangleNode tn;

	nodes.push_back(&tn);

	Traverse<Direction::Left>(&tn);
	tn.Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Mark);

	TriangleNodeCache::Update(nodes);

	BOOST_CHECK_EQUAL(nodes.size(), 1u);
	auto it = nodes.begin();
	BOOST_CHECK_EQUAL(*it, &tn);
}

BOOST_AUTO_TEST_CASE(TriangleNodeCacheTest4)
{
	TriangleNodeCache::TriangleNodes nodes;
	TriangleNode tn;

	nodes.push_back(&tn);

	{
		auto ll = Traverse<Direction::Left, Direction::Left>(&tn);

		TriangleNodeCache::Update(nodes);

		BOOST_CHECK_EQUAL(nodes.size(), 3u);
		auto it = nodes.begin();
		BOOST_CHECK_EQUAL(*it++, ll);
		BOOST_CHECK_EQUAL(*it++, ll->GetParent()->GetRChild());
		BOOST_CHECK_EQUAL(*it++, tn.GetRChild());
	}

	tn.Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Mark);

	TriangleNodeCache::Update(nodes); 

	BOOST_CHECK_EQUAL(nodes.size(), 1u);
	BOOST_CHECK_EQUAL(nodes.front(), &tn);

	nodes.clear();
}

BOOST_AUTO_TEST_CASE(TriangleNodeCacheTest5)
{
	TriangleNodeCache::TriangleNodes nodes;
	TriangleNode tn;

	nodes.push_back(&tn);

	auto llll = Traverse<Direction::Left, Direction::Left, Direction::Left, Direction::Left>(&tn);

	TriangleNodeCache::Update(nodes);

	BOOST_CHECK_EQUAL(nodes.size(), 9u);
	auto it = nodes.begin();
	TriangleNode * lll = llll->GetParent();
	BOOST_CHECK_EQUAL(*it++, lll->GetLChild());
	BOOST_CHECK_EQUAL(*it++, lll->GetRChild());
	TriangleNode * llr = lll->GetParent()->GetRChild();
	BOOST_CHECK_EQUAL(*it++, llr);
	TriangleNode * lr = lll->GetParent()->GetParent()->GetRChild();
	BOOST_CHECK_EQUAL(*it++, lr->GetLChild());
	TriangleNode * lrr = lr->GetRChild();
	BOOST_CHECK_EQUAL(*it++, lrr->GetLChild());
	BOOST_CHECK_EQUAL(*it++, lrr->GetRChild());
	TriangleNode * r = tn.GetRChild();
	BOOST_CHECK_EQUAL(*it++, r->GetLChild());
	TriangleNode * rr = r->GetRChild();
	BOOST_CHECK_EQUAL(*it++, rr->GetLChild());
	BOOST_CHECK_EQUAL(*it++, rr->GetRChild());
}

BOOST_AUTO_TEST_CASE(TriangleNodeCacheTest6)
{
	TriangleNodeCache::TriangleNodes nodes;
	TriangleNode tn;

	nodes.push_back(&tn);

	// split tn
	Traverse<Direction::Left, Direction::Left, Direction::Left, Direction::Left>(&tn);

	TriangleNodeCache::Update(nodes);
	BOOST_CHECK_EQUAL(nodes.size(), 9u);

	TriangleNode * ll = tn.GetLChild()->GetLChild();
	ll->Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Mark);

	TriangleNodeCache::Update(nodes);
	BOOST_CHECK_EQUAL(nodes.size(), 5u);

	auto it = nodes.begin();
	TriangleNode * l = tn.GetLChild();
	BOOST_CHECK_EQUAL(*it++, l->GetLChild());
	TriangleNode * lr = l->GetRChild();
	BOOST_CHECK_EQUAL(*it++, lr->GetLChild());
	BOOST_CHECK_EQUAL(*it++, lr->GetRChild());
	TriangleNode * r = tn.GetRChild();
	BOOST_CHECK_EQUAL(*it++, r->GetLChild());
	BOOST_CHECK_EQUAL(*it++, r->GetRChild());
}

BOOST_AUTO_TEST_CASE(TriangleNodeChildrenAllocationTest1)
{
	TriangleNode tn;

	Traverse<Direction::Left, Direction::Left, Direction::Left, Direction::Left>(&tn);

	TriangleNode * ll = tn.GetLChild()->GetLChild();
	ll->Merge(TriangleNode::ProcessBase::Merge, TriangleNode::RemoveAction::Mark);

	Traverse<Direction::Left, Direction::Left, Direction::Left>(ll);
}