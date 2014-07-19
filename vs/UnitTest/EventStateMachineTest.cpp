#include <boost/test/unit_test.hpp>
#include <boost/ref.hpp>

#include <EventStateMachine.h>

using namespace trm::impl;

namespace
{
	enum class MoveDirection
	{
		Undefined,
		Left,
		Right,
		Up,
		Down
	};

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
			if (x < 0.0f) { dir = MoveDirection::Left; }
			if (x > 0.0f) { dir = MoveDirection::Right; }
			if (y < 0.0f) { dir = MoveDirection::Down; }
			if (y > 0.0f) { dir = MoveDirection::Up; }
		}

		void Stop()
		{
			stopped = true;
		}

		void EmulateDynamicScene1()
		{
			scene1Emulated = true;
		}

		MoveDirection dir;
		bool stopped;
		bool scene1Emulated;
	};

	std::ostream & operator << (std::ostream & o, const MoveDirection dir)
	{
		if (dir == MoveDirection::Left) { o << "MoveLeft" ;}
		else if (dir == MoveDirection::Right) { o << "MoveRight" ;}
		else if (dir == MoveDirection::Up) { o << "MoveUp" ;}
		else if (dir == MoveDirection::Down) { o << "MoveDown" ;}
		else { o << "Undefined"; }

		return o;
	}
}

BOOST_AUTO_TEST_CASE(EventStateMachineMovingTest1)
{
	Subject s;
	EventStateMachine<Subject> esm(s);
	
	esm.Emit(MoveKeyPressed<MoveKeys::Left>());
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Left);

	esm.Emit(MoveKeyPressed<MoveKeys::Right>());
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Right);

	esm.Emit(MoveKeyPressed<MoveKeys::Up>());
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Up);

	esm.Emit(MoveKeyPressed<MoveKeys::Down>());
	BOOST_CHECK_EQUAL(s.dir, MoveDirection::Down);
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