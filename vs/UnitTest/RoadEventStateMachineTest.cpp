#include <boost/test/unit_test.hpp>

#define UNIT_TESTING_TURNED_ON

#include <RoadEventStateMachine.h>

using namespace trm::impl;
using namespace trm;

namespace
{
	struct Subject
		: boost::noncopyable
	{
		void PutRoad(const Point2d & pFrom, const Point2d & pTo, bool commit)
		{
			from = pFrom;
			to = pTo;
			committed = commit;
		}

		Point2d from;
		Point2d to;
		bool committed = false;
	};
}

BOOST_AUTO_TEST_CASE(RoadEventStateMachineCommitTest1)
{
	Subject s;
	RoadEventStateMachine<Subject> esm(s);

	esm.Emit(FingerPressed(1, Point2d(1, 1)));
	BOOST_CHECK(!s.committed);

	esm.Emit(FingerPressed(2, Point2d(2, 2)));
	BOOST_CHECK(!s.committed);

	esm.Emit(FingerMoved(1, Point2d(2, 2)));
	esm.Emit(Commit());
	BOOST_CHECK_EQUAL(s.from, Point2d(1, 1));
	BOOST_CHECK_EQUAL(s.to, Point2d(2, 2));
	BOOST_CHECK(!s.committed);

	esm.Emit(FingerMoved(1, Point2d(3, 3)));
	esm.Emit(FingerReleased(1, Point2d(4, 4)));
	BOOST_CHECK_EQUAL(s.from, Point2d(1, 1));
	BOOST_CHECK_EQUAL(s.to, Point2d(3, 3));
	BOOST_CHECK(s.committed);
}

BOOST_AUTO_TEST_CASE(RoadEventStateMachineCommitTest2)
{
	Subject s;
	RoadEventStateMachine<Subject> esm(s);

	esm.Emit(FingerPressed(1, Point2d(1, 1)));
	esm.Emit(FingerMoved(1, Point2d(3, 3)));
	esm.Emit(Reset());
	BOOST_CHECK_EQUAL(s.from, Point2d(1, 1));
	BOOST_CHECK_EQUAL(s.to, Point2d(3, 3));
	BOOST_CHECK(s.committed);
}
