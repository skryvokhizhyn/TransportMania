#include <boost/test/unit_test.hpp>

#include <TrainStateMachine.h>

using namespace trm::impl;

enum TSMTestState
{
	Uninit,
	Inited,
	Loaded,
	Moved,
	Unloaded
};

struct TSMTest
{
	TSMTest()
		: state(Uninit), inc(0)
	{}

	bool Init()
	{
		state = Inited;
		return true;
	}

	bool Load() 
	{ 
		state = Loaded;
		return true; 
	}

	bool Move() 
	{
		state = Moved;

		if (inc++ < 2)
		{
			return false;
		}
		
		return true; 
	}

	bool Unload() 
	{ 
		state = Unloaded;
		return true; 
	}

	TSMTestState state;
	int inc;
};

BOOST_AUTO_TEST_CASE(TrainStateMachineTest1)
{
	TSMTest t;
	
	{
		TrainStateMachine<TSMTest> tsm(&t);
		
		BOOST_CHECK_EQUAL(t.state, TSMTestState::Uninit);
		
		tsm.Update();
		BOOST_CHECK_EQUAL(t.state, TSMTestState::Inited);

		tsm.Update();
		BOOST_CHECK_EQUAL(t.state, TSMTestState::Loaded);

		tsm.Update();
		BOOST_CHECK_EQUAL(t.state, TSMTestState::Moved);
		BOOST_CHECK_EQUAL(t.inc, 1);

		tsm.Update();
		BOOST_CHECK_EQUAL(t.state, TSMTestState::Moved);
		BOOST_CHECK_EQUAL(t.inc, 2);

		tsm.Update();
		BOOST_CHECK_EQUAL(t.state, TSMTestState::Moved);
		BOOST_CHECK_EQUAL(t.inc, 3);

		tsm.Update();
		BOOST_CHECK_EQUAL(t.state, TSMTestState::Unloaded);

		tsm.Update();
		BOOST_CHECK_EQUAL(t.state, TSMTestState::Inited);

		tsm.Update();
		BOOST_CHECK_EQUAL(t.state, TSMTestState::Loaded);
	}
}