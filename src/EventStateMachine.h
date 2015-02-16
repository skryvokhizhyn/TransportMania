#pragma once

#include "EventStateMachineDefs.h"

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

namespace trm
{
namespace impl
{
	namespace bmf = boost::msm::front;

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

			struct EmptyState : public bmf::state<> {};
					
			struct FingerPressedState : public bmf::state<> 
			{
				using FingersType = std::map<FingerId, PointMove>;
				bool processPress = true;

				FingersType fingers;
			};

			typedef EmptyState initial_state;

			// actions

			ACTION_DEFINITION(Dummy);

			struct MultipleFingersGuard 
			{
				template <class EVT, class FSM, class SourceState, class TargetState>
				bool operator () (const EVT & evt, FSM & fsm, SourceState & sourceState, TargetState & targetState);
			};

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
				bmf::Row<	FingerPressedState, FingerPressed,	FingerPressedState, ApplyRegisterFinger,	bmf::none >,
				bmf::Row<	FingerPressedState,	FingerReleased,	EmptyState,			ApplyReleaseFinger,		bmf::euml::Not_<MultipleFingersGuard> >,
				bmf::Row<	FingerPressedState,	FingerReleased,	FingerPressedState,	ApplyReleaseFinger,		MultipleFingersGuard >,
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

} // namespace impl
} // namespace trm

#undef ACTION_DEFINITION

#include "EventStateMachineImpl.h"
