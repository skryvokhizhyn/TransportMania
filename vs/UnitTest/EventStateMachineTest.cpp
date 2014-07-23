#include <boost/test/unit_test.hpp>
//#include <boost/ref.hpp>

#include <EventStateMachine.h>

using namespace trm::impl;

namespace
{
	namespace MoveDirection
	{
		const int Undefined = 0;
		const int Left = 0x1;
		const int Right = 0x2;
		const int Up = 0x4;
		const int Down = 0x8;
		/*LeftUp,
		LeftDown,
		RightUp,
		RightDown*/
	} // namespace MoveDirection

	struct Subject
		: boost::noncopyable
	{
		Subject()
			: dir(MoveDirection::Undefined)
			, stopped(false)
			, scene1Emulated(false)
		{}

		void ShiftScene(const float x, const float y)
		{
			if (x < 0.0f) { dir |= static_cast<int>(MoveDirection::Left); }
			if (x > 0.0f) { dir |= static_cast<int>(MoveDirection::Right); }
			if (y < 0.0f) { dir |= static_cast<int>(MoveDirection::Down); }
			if (y > 0.0f) { dir |= static_cast<int>(MoveDirection::Up); }
		}

		void Stop()
		{
			stopped = true;
		}

		void EmulateDynamicScene1()
		{
			scene1Emulated = true;
		}

		//MoveDirection dir;
		int dir;
		bool stopped;
		bool scene1Emulated;
	};
}

BOOST_AUTO_TEST_CASE(EventStateMachineMovingTest1)
{
	Subject s;
	EventStateMachine<Subject> esm(s);
	
	esm.Emit(MoveKeyPressed<MoveKeys::Left>());
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Left);
	s.dir = MoveDirection::Undefined;

	esm.Emit(MoveKeyPressed<MoveKeys::Right>());
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Right);
	s.dir = MoveDirection::Undefined;

	esm.Emit(MoveKeyPressed<MoveKeys::Up>());
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Up);
	s.dir = MoveDirection::Undefined;

	esm.Emit(MoveKeyPressed<MoveKeys::Down>());
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Down);
	s.dir = MoveDirection::Undefined;
}

BOOST_AUTO_TEST_CASE(EventStateMachineQuitTest1)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	esm.Emit(QuitFired());
	BOOST_CHECK_EQUAL(s.stopped, true);
}

BOOST_AUTO_TEST_CASE(EventStateMachineKeysTest1)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	esm.Emit(Key1Pressed());
	BOOST_CHECK_EQUAL(s.scene1Emulated, true);
}

BOOST_AUTO_TEST_CASE(EventStateMachineMouseMovedTest1)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	esm.Emit(LMBPressed());
	esm.Emit(LMBReleased());
	esm.Emit(QuitFired());
	BOOST_CHECK_EQUAL(s.stopped, true);
}

BOOST_AUTO_TEST_CASE(EventStateMachineMouseMovedTest2)
{
	Subject s;
	EventStateMachine<Subject> esm(s);

	esm.Emit(LMBPressed());
	
	esm.Emit(QuitFired());
	BOOST_CHECK_EQUAL(s.stopped, false);

	esm.Emit(MouseMoved{1, 1});
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Right | MoveDirection::Up);
	s.dir = MoveDirection::Undefined;

	esm.Emit(MouseMoved{-1, 1});
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Left | MoveDirection::Up);
	s.dir = MoveDirection::Undefined;

	esm.Emit(MouseMoved{-1, -1});
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Left | MoveDirection::Down);
	s.dir = MoveDirection::Undefined;

	esm.Emit(MouseMoved{1, -1});
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Right | MoveDirection::Down);
	s.dir = MoveDirection::Undefined;

	esm.Emit(LMBReleased());

	esm.Emit(QuitFired());
	BOOST_CHECK_EQUAL(s.stopped, true);
}