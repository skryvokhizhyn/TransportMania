#include <boost/test/unit_test.hpp>

#define UNIT_TESTING_TURNED_ON

#include <EventStateMachine.h>

using namespace trm::impl;
using namespace trm;

namespace
{
	namespace MoveDirection
	{
		const int Undefined = 0;
		const int Left = 0x1;
		const int Right = 0x2;
		const int Up = 0x4;
		const int Down = 0x8;
	} // namespace MoveDirection

	struct Subject
		: boost::noncopyable
	{
		Subject()
			: dir(MoveDirection::Undefined)
			, fingerAffected(-1)
			, fingersRegistered(0)
			, updates(0)
		{}

		void ShiftScene(const float x, const float y)
		{
			if (x < 0.0f) { dir |= static_cast<int>(MoveDirection::Left); }
			if (x > 0.0f) { dir |= static_cast<int>(MoveDirection::Right); }
			if (y < 0.0f) { dir |= static_cast<int>(MoveDirection::Down); }
			if (y > 0.0f) { dir |= static_cast<int>(MoveDirection::Up); }
		}

		void ZoomScene(const float)
		{}

		void RotateScene(const Angle)
		{}

		void PressScene(const float, const float)
		{
			pressed = true;
		}

		void BendScene(const Angle, const Angle)
		{}

		void FingerAffected(FingerId id, int cnt)
		{
			fingerAffected = id;
			fingersRegistered = cnt;
		}

		void FingerReset(int cnt)
		{
			fingersRegistered = cnt;
		}

		void FingerMoved(FingerId id, const Point2d & f, const Point2d & t)
		{
			fingerAffected = id;
			from = f;
			to = t;
		}

		void UpdateTerrain()
		{
			updates = 2;
		}

		void Update()
		{
			if (canUpdate)
				--updates;
		}

		bool Updated() const
		{
			return updates == 0;
		}

		void StopTerrainUpdate()
		{
			canUpdate = false;
		}
		
		void ResumeTerrainUpdate()
		{
			canUpdate = true;
		}

		int dir;
		FingerId fingerAffected;
		int fingersRegistered;
		Point2d from;
		Point2d to;
		int updates;
		bool pressed = false;
		bool canUpdate = false;
	};
}

BOOST_AUTO_TEST_CASE(EventStateMachineCommitTest1)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	esm.Emit(Commit());
	esm.Emit(Commit());

	esm.Emit(FingerPressed{0, Point2d(0, 0)});
	esm.Emit(Commit());
	esm.Emit(Commit());
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Undefined);
	esm.Emit(FingerMoved{0, Point2d(1, 1)});
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Undefined);
	esm.Emit(Commit());
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Right | MoveDirection::Up);
	s.dir = MoveDirection::Undefined;
	esm.Emit(Commit());
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Undefined);
	esm.Emit(FingerReleased{0, Point2d()});
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Undefined);
	esm.Emit(Commit());
	esm.Emit(Commit());
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Undefined);
}

BOOST_AUTO_TEST_CASE(EventStateMachineMouseMovedTest2)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	esm.Emit(FingerPressed{0, Point2d(0, 0)});
	esm.Emit(FingerMoved{0, Point2d(1, 1)});
	esm.Emit(Commit{});
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Right | MoveDirection::Up);
	s.dir = MoveDirection::Undefined;

	esm.Emit(FingerMoved{0, Point2d(0, 2)});
	esm.Emit(Commit{});
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Left | MoveDirection::Up);
	s.dir = MoveDirection::Undefined;

	esm.Emit(FingerMoved{0, Point2d(-1, 1)});
	esm.Emit(Commit{});
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Left | MoveDirection::Down);
	s.dir = MoveDirection::Undefined;

	esm.Emit(FingerMoved{0, Point2d(0, 0)});
	esm.Emit(Commit{});
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Right | MoveDirection::Down);
}

BOOST_AUTO_TEST_CASE(EventStateMachineMouseMovedTest3)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	esm.Emit(FingerMoved{0, Point2d(1.0f, 1.0f)});
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Undefined);

	esm.Emit(FingerPressed{0, Point2d(0, 0)});
	esm.Emit(FingerMoved{0, Point2d(1.0f, 1.0f)});
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Undefined);

	esm.Emit(FingerMoved{0, Point2d(-1.5f, -3.0f)});
	esm.Emit(Commit{});
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Down | MoveDirection::Left);
	s.dir = MoveDirection::Undefined;

	esm.Emit(FingerMoved{0, Point2d(4.5f, 7.0f)});
	esm.Emit(FingerReleased{0, Point2d()});
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Up | MoveDirection::Right);
}

BOOST_AUTO_TEST_CASE(EventStateMachineMouseMovedTest4)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	esm.Emit(FingerPressed{0, Point2d()});
	esm.Emit(FingerReleased{0, Point2d()});
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Undefined);
}

BOOST_AUTO_TEST_CASE(EventStateMachineFingerTest1)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	esm.Emit(FingerPressed{0, Point2d(1.0f, 1.0f)});

	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 1);

	esm.Emit(FingerReleased{0, Point2d()});
	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 0);
}

BOOST_AUTO_TEST_CASE(EventStateMachineFingerTest2)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	esm.Emit(FingerPressed{0, Point2d(1.0f, 1.0f)});

	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 1);

	esm.Emit(FingerPressed{1, Point2d(1.0f, 1.0f)});

	BOOST_CHECK_EQUAL(s.fingerAffected, 1);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 2);

	esm.Emit(FingerReleased{1, Point2d()});
	BOOST_CHECK_EQUAL(s.fingerAffected, 1);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 1);

	esm.Emit(FingerReleased{0, Point2d()});
	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 0);
}

BOOST_AUTO_TEST_CASE(EventStateMachineFingerTest3)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	esm.Emit(FingerPressed{0, Point2d(1.0f, 1.0f)});

	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 1);

	esm.Emit(FingerPressed{1, Point2d(1.0f, 1.0f)});

	BOOST_CHECK_EQUAL(s.fingerAffected, 1);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 2);

	esm.Emit(FingerReleased{0, Point2d()});
	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 1);

	esm.Emit(FingerReleased{1, Point2d()});
	BOOST_CHECK_EQUAL(s.fingerAffected, 1);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 0);
}

BOOST_AUTO_TEST_CASE(EventStateMachineFingerTest4)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	esm.Emit(FingerPressed{0, Point2d(1.0f, 1.0f)});

	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 1);

	esm.Emit(FingerPressed{1, Point2d(1.0f, 1.0f)});

	BOOST_CHECK_EQUAL(s.fingerAffected, 1);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 2);

	// ignore 3rd finger
	esm.Emit(FingerPressed{2, Point2d(1.0f, 1.0f)});

	BOOST_CHECK_EQUAL(s.fingerAffected, 1);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 2);

	esm.Emit(FingerReleased{0, Point2d()});
	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 1);

	esm.Emit(FingerReleased{1, Point2d()});
	BOOST_CHECK_EQUAL(s.fingerAffected, 1);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 0);
}

BOOST_AUTO_TEST_CASE(EventStateMachineFingerMoveTest1)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	esm.Emit(FingerPressed{0, Point2d(1.0f, 1.0f)});
	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 1);

	esm.Emit(FingerMoved{0, Point2d(2.0f, 2.0f)});
	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.from, Point2d(1.0f, 1.0f));
	BOOST_CHECK_EQUAL(s.to, Point2d(2.0f, 2.0f));

	esm.Emit(FingerReleased{0, Point2d()});
	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 0);
}

BOOST_AUTO_TEST_CASE(EventStateMachineFingerMoveTest2)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	esm.Emit(FingerPressed{0, Point2d(1.0f, 1.0f)});
	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 1);

	esm.Emit(FingerMoved{0, Point2d(2.0f, 2.0f)});
	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.from, Point2d(1.0f, 1.0f));
	BOOST_CHECK_EQUAL(s.to, Point2d(2.0f, 2.0f));

	esm.Emit(FingerPressed{1, Point2d(2.0f, 2.0f)});
	BOOST_CHECK_EQUAL(s.fingerAffected, 1);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 2);

	esm.Emit(FingerMoved{0, Point2d(3.0f, 3.0f)});
	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.from, Point2d(2.0f, 2.0f));
	BOOST_CHECK_EQUAL(s.to, Point2d(3.0f, 3.0f));

	esm.Emit(FingerMoved{1, Point2d(3.0f, 3.0f)});
	BOOST_CHECK_EQUAL(s.fingerAffected, 1);
	BOOST_CHECK_EQUAL(s.from, Point2d(2.0f, 2.0f));
	BOOST_CHECK_EQUAL(s.to, Point2d(3.0f, 3.0f));

	esm.Emit(FingerReleased{0, Point2d()});
	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 1);
}


BOOST_AUTO_TEST_CASE(EventStateMachineFingerMoveTest3)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	esm.Emit(FingerPressed{0, Point2d(1.0f, 1.0f)});
	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 1);

	esm.Emit(FingerMoved{0, Point2d(3.0f, 3.0f)});
	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.from, Point2d(1.0f, 1.0f));
	BOOST_CHECK_EQUAL(s.to, Point2d(3.0f, 3.0f));

	esm.Emit(Commit{});
	esm.Emit(FingerMoved{0, Point2d(1.0f, 1.0f)});
	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.from, Point2d(3.0f, 3.0f));
	BOOST_CHECK_EQUAL(s.to, Point2d(1.0f, 1.0f));

	esm.Emit(FingerReleased{0, Point2d()});
	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 0);
}

BOOST_AUTO_TEST_CASE(EventStateMachineFingerMoveTest4)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	esm.Emit(FingerPressed{0, Point2d(1.0f, 1.0f)});
	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 1);

	esm.Emit(FingerMoved{0, Point2d(3.0f, 3.0f)});
	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.from, Point2d(1.0f, 1.0f));
	BOOST_CHECK_EQUAL(s.to, Point2d(3.0f, 3.0f));

	esm.Emit(FingerPressed{1, Point2d(1.0f, 1.0f)});
	BOOST_CHECK_EQUAL(s.fingerAffected, 1);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 2);
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Up | MoveDirection::Right);

	esm.Emit(Commit{});
	esm.Emit(FingerMoved{0, Point2d(1.0f, 1.0f)});
	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.from, Point2d(3.0f, 3.0f));
	BOOST_CHECK_EQUAL(s.to, Point2d(1.0f, 1.0f));

	esm.Emit(FingerReleased{0, Point2d()});
	BOOST_CHECK_EQUAL(s.fingerAffected, 0);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 1);

	esm.Emit(FingerReleased{1, Point2d()});
	BOOST_CHECK_EQUAL(s.fingerAffected, 1);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 0);
}

BOOST_AUTO_TEST_CASE(EventStateMachineUpdatesTest1)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	BOOST_CHECK(s.Updated());
	
	esm.Emit(FingerPressed{1, Point2d(1.0f, 1.0f)});
	
	// no updates after first finger
	BOOST_CHECK_EQUAL(s.updates, 0);

	BOOST_CHECK(!s.canUpdate);

	s.Update();
	s.Update();

	BOOST_CHECK_EQUAL(s.updates, 0);

	esm.Emit(FingerReleased{1, Point2d()});

	BOOST_CHECK_EQUAL(s.updates, 2);

	s.Update();
	s.Update();

	BOOST_CHECK(s.Updated());
}

BOOST_AUTO_TEST_CASE(EventStateMachineUpdatesTest2)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	esm.Emit(FingerPressed{1, Point2d(1.0f, 1.0f)});
	
	esm.Emit(FingerReleased{1, Point2d()});

	BOOST_CHECK_EQUAL(s.updates, 2);

	s.Update();
	BOOST_CHECK_EQUAL(s.updates, 1);

	esm.Emit(FingerPressed{1, Point2d(1.0f, 1.0f)});

	s.Update();
	BOOST_CHECK_EQUAL(s.updates, 1);

	esm.Emit(FingerReleased{1, Point2d()});

	s.Update();
	s.Update();
	BOOST_CHECK_EQUAL(s.updates, 0);

	BOOST_CHECK(s.Updated());
}

BOOST_AUTO_TEST_CASE(EventStateMachineResetTest1)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	esm.Emit(Reset());

	BOOST_CHECK_EQUAL(s.fingersRegistered, 0);

	esm.Emit(FingerPressed(0, Point2d(0, 0)));

	BOOST_CHECK_EQUAL(s.fingersRegistered, 1);

	esm.Emit(FingerMoved(0, Point2d(1, 1)));

	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Undefined);

	esm.Emit(Reset());

	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Right | MoveDirection::Up);
	BOOST_CHECK_EQUAL(s.fingersRegistered, 1);
}

BOOST_AUTO_TEST_CASE(EventStateMachinePressTest1)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	BOOST_CHECK(!s.pressed);

	esm.Emit(FingerPressed(0, Point2d(0, 0)));

	BOOST_CHECK(!s.pressed);

	esm.Emit(FingerReleased{0, Point2d(0, 0)});

	BOOST_CHECK(s.pressed);

	s.pressed = false;
	
	esm.Emit(FingerPressed(0, Point2d(0, 0)));
	esm.Emit(FingerPressed{1, Point2d(0, 0)});
	esm.Emit(FingerReleased{0, Point2d(0, 0)});

	BOOST_CHECK(!s.pressed);

	esm.Emit(FingerReleased{1, Point2d(0, 0)});

	BOOST_CHECK(!s.pressed);

	s.pressed = false;

	esm.Emit(FingerPressed(0, Point2d(0, 0)));
	esm.Emit(FingerMoved(0, Point2d(1, 1)));
	esm.Emit(FingerReleased{0, Point2d(0, 0)});

	BOOST_CHECK(!s.pressed);
}

BOOST_AUTO_TEST_CASE(EventStateMachinePressTest2)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	esm.Emit(FingerPressed(0, Point2d(0, 0)));
	esm.Emit(FingerMoved(0, Point2d(1, 1)));
	esm.Emit(Commit());
	esm.Emit(FingerReleased{0, Point2d(0, 0)});

	BOOST_CHECK(!s.pressed);

	esm.Emit(FingerPressed(0, Point2d(0, 0)));
	esm.Emit(FingerMoved(0, Point2d(1, 1)));
	esm.Emit(Reset());
	esm.Emit(FingerReleased{0, Point2d(0, 0)});

	BOOST_CHECK(!s.pressed);
}
