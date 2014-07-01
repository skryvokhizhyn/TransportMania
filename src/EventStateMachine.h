#pragma once

#include <boost/noncopyable.hpp>

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
		(Evt const & /*evt*/, Fsm & fsm, SourceState &, TargetState &) 

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

	// State machine

	template<typename Subject>
	class EventStateMachine
		: boost::noncopyable
	{
	public:
		EventStateMachine(Subject & subj);

		template<typename Evt>
		void Emit(const Evt & e);

	private:

		struct EventSMImpl : public boost::msm::front::state_machine_def<EventSMImpl>
		{
			EventSMImpl(Subject & s);

			struct EmptyState : public bmf::state<>
			{};
		
			typedef EmptyState initial_state;

			// actions

			ACTION_DEFINITION(LeftKey);
			ACTION_DEFINITION(RightKey);
			ACTION_DEFINITION(UpKey);
			ACTION_DEFINITION(DownKey);

			ACTION_DEFINITION(Quit)

			struct transition_table : boost::mpl::vector<
				//			Start			Event							Next		Action			Guard
				bmf::Row<	EmptyState,		MoveKeyPressed<MoveKeys::Left>,	EmptyState,	ApplyLeftKey,	bmf::none >,
				bmf::Row<	EmptyState,		MoveKeyPressed<MoveKeys::Right>,EmptyState,	ApplyRightKey,	bmf::none >,
				bmf::Row<	EmptyState,		MoveKeyPressed<MoveKeys::Up>,	EmptyState,	ApplyUpKey,		bmf::none >,
				bmf::Row<	EmptyState,		MoveKeyPressed<MoveKeys::Down>,	EmptyState,	ApplyDownKey,	bmf::none >,
				bmf::Row<	EmptyState,		QuitFired,						EmptyState,	ApplyQuit,		bmf::none >
				> {};

			Subject & subj_;
		};

		// Replaces the default no-transition response.
		template<class FSM, class Event>
		void no_transition(const Event & /*e*/, FSM &, int state)
		{
			std::stringstream ss;
			ss << "no transition from state " << state
				<< " on event " << typeid(Event).name();

			throw std::runtime_error(ss.str());
		}

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

	// Actions

	ACTION_IMPLEMENTATION(LeftKey)
	{
		fsm.subj_.ShiftScene(-1.0f, 0.0f);
	}

	ACTION_IMPLEMENTATION(RightKey)
	{
		fsm.subj_.ShiftScene(1.0f, 0.0f);
	}

	ACTION_IMPLEMENTATION(UpKey)
	{
		fsm.subj_.ShiftScene(0.0f, 1.0f);
	}

	ACTION_IMPLEMENTATION(DownKey)
	{
		fsm.subj_.ShiftScene(0.0f, -1.0f);
	}

	ACTION_IMPLEMENTATION(Quit)
	{
		fsm.subj_.Stop();
	}

} // namespace impl
} // namespace trm

#undef ACTION_DEFINITION
#undef ACTION_IMPLEMENTATION
