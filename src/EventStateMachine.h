#pragma once

#include "EventStateMachineDefs.h"

#include <typeinfo>
#include <map>

#pragma warning(push)
#pragma warning(disable: 4512 4127)

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

		bool Commitable() const;

	private:

		struct EventSMImpl : public boost::msm::front::state_machine_def<EventSMImpl>
		{
			EventSMImpl(Subject & s);

			struct EmptyState : public bmf::state<> {};
					
			struct FingerPressedState : public bmf::state<> 
			{
				using FingersType = std::map<FingerId, PointMove>;

				FingersType fingers;
			};

			typedef EmptyState initial_state;

			// actions

			ACTION_DEFINITION(LeftKey);
			ACTION_DEFINITION(RightKey);
			ACTION_DEFINITION(UpKey);
			ACTION_DEFINITION(DownKey);

			ACTION_DEFINITION(Quit);
			ACTION_DEFINITION(Key1);

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

			using MoveKeyU = MoveKeyPressed<MoveKeys::Up>;
			using MoveKeyL = MoveKeyPressed<MoveKeys::Left>;
			using MoveKeyR = MoveKeyPressed<MoveKeys::Right>;
			using MoveKeyD = MoveKeyPressed<MoveKeys::Down>;

			struct transition_table : boost::mpl::vector<
				//			Start				Event			Next				Action					Guard
				bmf::Row<	EmptyState,			MoveKeyL,		EmptyState,			ApplyLeftKey,			bmf::none >,
				bmf::Row<	EmptyState,			MoveKeyR,		EmptyState,			ApplyRightKey,			bmf::none >,
				bmf::Row<	EmptyState,			MoveKeyU,		EmptyState,			ApplyUpKey,				bmf::none >,
				bmf::Row<	EmptyState,			MoveKeyD,		EmptyState,			ApplyDownKey,			bmf::none >,
				bmf::Row<	EmptyState,			QuitFired,		EmptyState,			ApplyQuit,				bmf::none >,
				bmf::Row<	EmptyState,			Key1Pressed,	EmptyState,			ApplyKey1,				bmf::none >,
				bmf::Row<	EmptyState,			FingerPressed,	FingerPressedState,	ApplyRegisterFinger,	bmf::none >,
				bmf::Row<	EmptyState,			FingerMoved,	EmptyState,			bmf::none,				bmf::none >,
				bmf::Row<	FingerPressedState, FingerPressed,	FingerPressedState, ApplyRegisterFinger,	bmf::none >,
				bmf::Row<	FingerPressedState,	FingerReleased,	EmptyState,			ApplyReleaseFinger,		bmf::euml::Not_<MultipleFingersGuard> >,
				bmf::Row<	FingerPressedState,	FingerReleased,	FingerPressedState,	ApplyReleaseFinger,		MultipleFingersGuard >,
				bmf::Row<	FingerPressedState,	FingerMoved,	FingerPressedState,	ApplyMoveFinger,		bmf::none >,
				bmf::Row<	FingerPressedState, Commit,			FingerPressedState, ApplyFingerCommit,		bmf::none >
				> {};

			// Replaces the default no-transition response.
			template<class FSM, class Event>
			void no_transition(const Event &, FSM &, int state);

			Subject & subj_;
		};

	private:
		// Pick a back-end
		typedef boost::msm::back::state_machine<EventSMImpl> EventSM;
		EventSM eventSM_;
	};

} // namespace impl
} // namespace trm

#undef ACTION_DEFINITION

#include "EventStateMachineImpl.h"
