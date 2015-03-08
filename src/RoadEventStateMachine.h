#pragma once

#include "EventStateMachineDefs.h"
#include "Logger.h"

#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/optional.hpp>

#include <typeinfo>
#include <map>

#pragma warning(push)
#pragma warning(disable: 4512 4127 4503)

// back-end
#include <boost/msm/back/state_machine.hpp>
//front-end
#include <boost/msm/front/state_machine_def.hpp>
// functors
#include <boost/msm/front/functor_row.hpp>
// Not_
#include <boost/msm/front/euml/operator.hpp>

#define ACTION_DEFINITION(name) \
	struct Apply ## name \
	{ \
		template<class Fsm, class Evt, class SourceState, class TargetState> \
		void operator()(Evt const & evt, Fsm & fsm, SourceState &, TargetState &); \
	};

#define ACTION_IMPLEMENTATION(name) \
	template<typename Subject> \
	template<class Fsm, class Evt, class SourceState, class TargetState> \
	void RoadEventStateMachine<Subject>::EventSMImpl::Apply ## name ::operator () \
		(Evt const & evt, Fsm & fsm, SourceState & sourceState, TargetState & targetState) 

#define ACTION_IMPLEMENTATION_UNUSED_GUARD \
	(evt); (fsm); (sourceState); (targetState)

namespace trm
{
namespace impl
{
	namespace bmf = boost::msm::front;

	// State machine

	template<typename Subject>
	class RoadEventStateMachine
		: boost::noncopyable
	{
	public:
		RoadEventStateMachine(Subject & subj);

		template<typename Evt>
		void Emit(const Evt & e);

	private:

		struct EventSMImpl : public boost::msm::front::state_machine_def<EventSMImpl>
		{
			EventSMImpl(Subject & s);

			struct EmptyState : public bmf::state<> {};
					
			struct FingerPressedState : public bmf::state<> 
			{
				using FingerData = boost::optional<PointMove>;

				FingerData finger;
			};

			typedef EmptyState initial_state;

			// actions

			ACTION_DEFINITION(RegisterFinger);
			ACTION_DEFINITION(ReleaseFinger);
			ACTION_DEFINITION(MoveFinger);
			ACTION_DEFINITION(FingerCommit);
			ACTION_DEFINITION(FingerReset);

			struct transition_table : boost::mpl::vector<
				//			Start				Event			Next				Action					Guard
				bmf::Row<	EmptyState,			FingerPressed,	FingerPressedState,	ApplyRegisterFinger,	bmf::none >,
				bmf::Row<	EmptyState,			FingerMoved,	EmptyState,			bmf::none,				bmf::none >,
				bmf::Row<	EmptyState,			FingerReleased,	EmptyState,			bmf::none,				bmf::none >,
				bmf::Row<	FingerPressedState, FingerPressed,	FingerPressedState, bmf::none,				bmf::none >,
				bmf::Row<	FingerPressedState,	FingerReleased,	EmptyState,			ApplyReleaseFinger,		bmf::none >,
				bmf::Row<	FingerPressedState,	FingerMoved,	FingerPressedState,	ApplyMoveFinger,		bmf::none >,
				bmf::Row<	FingerPressedState, Commit,			FingerPressedState, ApplyFingerCommit,		bmf::none >,
				bmf::Row<	EmptyState,			Commit,			EmptyState,			bmf::none,				bmf::none >,
				bmf::Row<	EmptyState,			Reset,			EmptyState,			bmf::none,				bmf::none >,
				bmf::Row<	FingerPressedState,	Reset,			EmptyState,			ApplyFingerReset,		bmf::none >
				> {};

			// Replaces the default no-transition response.
			template<class FSM, class Event>
			void no_transition(const Event &, FSM &, int state);

			Subject & subj_;
		};

	private:
		// Pick a back-end
		typedef boost::msm::back::state_machine<EventSMImpl> EventSM;

	private:
		EventSM eventSM_;
	};

	template<typename Subject>
	template<class FSM, class Event>
	void RoadEventStateMachine<Subject>::EventSMImpl::no_transition(const Event &, FSM &, int state)
	{
		std::stringstream ss;
		ss << "no transition from state " << state 
			<< " on event " << typeid(Event).name();

		utils::Logger().Debug() << ss.str();
	}

	template<typename Subject>
	RoadEventStateMachine<Subject>::RoadEventStateMachine(Subject & s)
		: eventSM_(boost::ref(s))
	{}

	template<typename Subject>
	RoadEventStateMachine<Subject>::EventSMImpl::EventSMImpl(Subject & s)
		: subj_(s)
	{}

	template<typename Subject>
	template<typename Evt>
	void RoadEventStateMachine<Subject>::Emit(const Evt & evt)
	{
		eventSM_.process_event(evt);
	}

	// Actions

	ACTION_IMPLEMENTATION(RegisterFinger)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD;

		assert(!targetState.finger);

		targetState.finger = PointMove(evt.pos);
	}

	ACTION_IMPLEMENTATION(ReleaseFinger)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD;

		FingerCommitImpl(fsm.subj_, sourceState.finger, true);

		sourceState.finger.reset();
	}

	ACTION_IMPLEMENTATION(MoveFinger)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD;

		// ignored finger has been moved
		if (!sourceState.finger)
			return;

		sourceState.finger->to = evt.pos;
	}

	template<typename Subject, typename FingerData>
	void FingerCommitImpl(Subject & subj, FingerData & finger, bool commit)
	{
		if (!finger)
			return;

		subj.PutRoad(finger->from, finger->to, commit);
	}

	ACTION_IMPLEMENTATION(FingerCommit)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD;

		FingerCommitImpl(fsm.subj_, sourceState.finger, false);
	}

	ACTION_IMPLEMENTATION(FingerReset)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD;

		// commit first
		FingerCommitImpl(fsm.subj_, sourceState.finger, true);

		sourceState.finger.reset();
	}

} // namespace impl
} // namespace trm

#undef ACTION_DEFINITION
#undef ACTION_IMPLEMENTATION
#undef ACTION_IMPLEMENTATION_UNUSED_GUARD

