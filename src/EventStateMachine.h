#pragma once

#include "Logger.h"

#include <boost/noncopyable.hpp>

#include <typeinfo>

#pragma warning(push)
#pragma warning(disable: 4512 4127)

// back-end
#include <boost/msm/back/state_machine.hpp>
//front-end
#include <boost/msm/front/state_machine_def.hpp>
// functors
#include <boost/msm/front/functor_row.hpp>

#define ACTION_DEFINITION(name) \
	struct Apply ## name \
	{ \
		template<class Fsm, class Evt, class SourceState, class TargetState> \
		void operator()(Evt const & evt, Fsm & fsm, SourceState &, TargetState &); \
	};

#define ACTION_IMPLEMENTATION(name) \
	template<typename Subject> \
	template<class Fsm, class Evt, class SourceState, class TargetState> \
	void EventStateMachine<Subject>::EventSMImpl::Apply ## name ::operator () \
		(Evt const & evt, Fsm & fsm, SourceState & sourceState, TargetState & targetState) 

#define ACTION_IMPLEMENTATION_UNUSED_GUARD \
	(evt); (fsm); (sourceState); (targetState);

namespace trm
{
namespace impl
{
	namespace bmf = boost::msm::front;

	// Events

	enum class MoveKeys { Left, Right, Up, Down };
	template<MoveKeys Key>
	struct MoveKeyPressed {};

	struct QuitFired {};
	struct Key1Pressed {};

	struct LMBPressed {};
	struct MouseMoved 
	{ 
		float xShift, yShift; 

		MouseMoved() : xShift(0.0f), yShift(0.0f) {}
		MouseMoved(float xS, float yS) : xShift(xS), yShift(yS) {}
		
		MouseMoved & operator += (const MouseMoved & m)
		{
			xShift += m.xShift;
			yShift += m.yShift;
			
			return *this;
		}
	};
	struct LMBReleased {};

	struct Commit {};

	// State machine

	template<typename Subject>
	class EventStateMachine
		: boost::noncopyable
	{
	public:
		EventStateMachine(Subject & subj);

		template<typename Evt>
		void Emit(const Evt & e);

		bool Commitable() const;

	private:

		struct EventSMImpl : public boost::msm::front::state_machine_def<EventSMImpl>
		{
			EventSMImpl(Subject & s);

			struct EmptyState : public bmf::state<> {};
			struct LMBPressedState : public bmf::state<> {};
			struct LMBMovingState : public bmf::state<> 
			{
				MouseMoved data;
			};
		
			typedef EmptyState initial_state;

			// actions

			ACTION_DEFINITION(LeftKey);
			ACTION_DEFINITION(RightKey);
			ACTION_DEFINITION(UpKey);
			ACTION_DEFINITION(DownKey);

			ACTION_DEFINITION(Quit);
			ACTION_DEFINITION(Key1);

			ACTION_DEFINITION(MouseMove);
			ACTION_DEFINITION(MoveCommit);

			ACTION_DEFINITION(Dummy);

			struct transition_table : boost::mpl::vector<
				//			Start			Event							Next			Action			Guard
				bmf::Row<	EmptyState,		MoveKeyPressed<MoveKeys::Left>,	EmptyState,		ApplyLeftKey,	bmf::none >,
				bmf::Row<	EmptyState,		MoveKeyPressed<MoveKeys::Right>,EmptyState,		ApplyRightKey,	bmf::none >,
				bmf::Row<	EmptyState,		MoveKeyPressed<MoveKeys::Up>,	EmptyState,		ApplyUpKey,		bmf::none >,
				bmf::Row<	EmptyState,		MoveKeyPressed<MoveKeys::Down>,	EmptyState,		ApplyDownKey,	bmf::none >,
				bmf::Row<	EmptyState,		QuitFired,						EmptyState,		ApplyQuit,		bmf::none >,
				bmf::Row<	EmptyState,		Key1Pressed,					EmptyState,		ApplyKey1,		bmf::none >,
				bmf::Row<	EmptyState,		LMBPressed,						LMBPressedState,bmf::none,		bmf::none >,
				bmf::Row<	EmptyState,		MouseMoved,						EmptyState,		bmf::none,		bmf::none >,
				bmf::Row<	LMBPressedState,MouseMoved,						LMBMovingState,	ApplyMouseMove, bmf::none >,
				bmf::Row<	LMBPressedState,Commit,							LMBPressedState,bmf::none,		bmf::none >,
				bmf::Row<	LMBMovingState, MouseMoved,						LMBMovingState, ApplyMouseMove,	bmf::none >,
				bmf::Row<	LMBMovingState,	Commit,							LMBPressedState,ApplyMoveCommit,bmf::none >,
				bmf::Row<	LMBMovingState,	LMBReleased,					EmptyState,		ApplyMoveCommit,bmf::none >,
				bmf::Row<	LMBPressedState,LMBReleased,					EmptyState,		bmf::none,		bmf::none >
				> {};

			// Replaces the default no-transition response.
			template<class FSM, class Event>
			void no_transition(const Event &, FSM &, int state)
			{
				std::stringstream ss;
				ss << "no transition from state " << state 
					<< " on event " << typeid(Event).name();

				utils::Logger().Debug() << ss.str();
			}

			Subject & subj_;
		};

	private:
		// Pick a back-end
		typedef boost::msm::back::state_machine<EventSMImpl> EventSM;
		EventSM eventSM_;
	};

	template<typename Subject>
	EventStateMachine<Subject>::EventStateMachine(Subject & s)
		: eventSM_(boost::ref(s))
	{}

	template<typename Subject>
	EventStateMachine<Subject>::EventSMImpl::EventSMImpl(Subject & s)
		: subj_(s)
	{}

	template<typename Subject>
	template<typename Evt>
	void EventStateMachine<Subject>::Emit(const Evt & evt)
	{
		eventSM_.process_event(evt);
	}

	template<typename Subject>
	bool EventStateMachine<Subject>::Commitable() const
	{
		const int commitableStateId = boost::msm::back::get_state_id<EventSM::stt, EventSMImpl::LMBMovingState>::value;

		return eventSM_.current_state()[0] == commitableStateId;
	}

	// Actions

	ACTION_IMPLEMENTATION(LeftKey)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD

		fsm.subj_.ShiftScene(-1.0f, 0.0f);
	}

	ACTION_IMPLEMENTATION(RightKey)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD

		fsm.subj_.ShiftScene(1.0f, 0.0f);
	}

	ACTION_IMPLEMENTATION(UpKey)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD

		fsm.subj_.ShiftScene(0.0f, 1.0f);
	}

	ACTION_IMPLEMENTATION(DownKey)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD

		fsm.subj_.ShiftScene(0.0f, -1.0f);
	}

	ACTION_IMPLEMENTATION(Quit)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD

		fsm.subj_.Stop();
	}

	ACTION_IMPLEMENTATION(Key1)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD

		fsm.subj_.EmulateDynamicScene1();
	}

	ACTION_IMPLEMENTATION(Dummy)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD

		std::stringstream ss;
		ss << "\nEvent " << typeid(Evt).name()
			<< "\nfrom " << typeid(SourceState).name()
			<< "\nto " << typeid(TargetState).name();

		utils::Logger().Debug() << "Dummy action fired. Info: " << ss.str();
	}

	ACTION_IMPLEMENTATION(MouseMove)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD

		targetState.data += evt;
	}

	ACTION_IMPLEMENTATION(MoveCommit)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD

		fsm.subj_.ShiftScene(sourceState.data.xShift, sourceState.data.yShift);
		sourceState.data = MouseMoved();
	}

} // namespace impl
} // namespace trm

#undef ACTION_DEFINITION
#undef ACTION_IMPLEMENTATION
#undef ACTION_IMPLEMENTATION_UNUSED_GUARD
