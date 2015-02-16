#pragma once

#include "Logger.h"
#include "SubjectAction.h"

#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/adaptor/map.hpp>

#define ACTION_IMPLEMENTATION(name) \
	template<typename Subject> \
	template<class Fsm, class Evt, class SourceState, class TargetState> \
	void EventStateMachine<Subject>::EventSMImpl::Apply ## name ::operator () \
		(Evt const & evt, Fsm & fsm, SourceState & sourceState, TargetState & targetState) 

#define ACTION_IMPLEMENTATION_UNUSED_GUARD \
	(evt); (fsm); (sourceState); (targetState)

namespace trm
{
namespace impl
{
	template<typename Subject>
	template<class FSM, class Event>
	void EventStateMachine<Subject>::EventSMImpl::no_transition(const Event &, FSM &, int state)
	{
		std::stringstream ss;
		ss << "no transition from state " << state 
			<< " on event " << typeid(Event).name();

		utils::Logger().Debug() << ss.str();
	}

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

	ACTION_IMPLEMENTATION(Dummy)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD;

		std::stringstream ss;
		ss << "\nEvent " << typeid(Evt).name()
			<< "\nfrom " << typeid(SourceState).name()
			<< "\nto " << typeid(TargetState).name();

		utils::Logger().Debug() << "Dummy action fired. Info: " << ss.str();
	}

	template<typename Subject>
	template<class EVT, class FSM, class SourceState, class TargetState>
	bool 
	EventStateMachine<Subject>::EventSMImpl::MultipleFingersGuard::operator () 
		(const EVT & evt, FSM & fsm, SourceState & sourceState, TargetState & targetState)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD;

		if (sourceState.fingers.size() > 1)
		{
			return true;
		}

		return false;
	}

	ACTION_IMPLEMENTATION(RegisterFinger)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD;

		// when at least one finger is pressed we stop terrain updates
		if (targetState.fingers.empty())
			fsm.subj_.StopTerrainUpdate();
		else
			targetState.processPress = false;

		assert(targetState.fingers.count(evt.fingerId) == 0LL);

		// process 2 fingers only
		if (targetState.fingers.size() >= 2)
			return;

		// commit first
		FingerCommitImpl(fsm.subj_, targetState.fingers, true, false);

		auto & fingerStruct = targetState.fingers[evt.fingerId];
		fingerStruct = PointMove(evt.pos);

#ifdef UNIT_TESTING_TURNED_ON
		fsm.subj_.FingerAffected(evt.fingerId, targetState.fingers.size());
#endif // UNIT_TESTING_TURNED_ON
	}

	ACTION_IMPLEMENTATION(ReleaseFinger)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD;

		// commit first
		FingerCommitImpl(fsm.subj_, sourceState.fingers, true, sourceState.processPress);

		const int erazed = sourceState.fingers.erase(evt.fingerId);
		
		// when we release the last finger we can proceed with terrain updates
		if (sourceState.fingers.empty())
		{
			fsm.subj_.ResumeTerrainUpdate();
			sourceState.processPress = true;
		}

		// if we try to release non existing finger - it's ok
		if (!erazed)
			return;

#ifdef UNIT_TESTING_TURNED_ON
		fsm.subj_.FingerAffected(evt.fingerId, sourceState.fingers.size());
#endif // UNIT_TESTING_TURNED_ON
	}

	ACTION_IMPLEMENTATION(MoveFinger)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD;

		// ignored finger has been moved
		if (sourceState.fingers.count(evt.fingerId) == 0)
			return;

		auto & fingerStruct = sourceState.fingers.at(evt.fingerId);
		if (fingerStruct.from != evt.pos)
		{
			fingerStruct.to = evt.pos;
		}

		sourceState.processPress = false;

#ifdef UNIT_TESTING_TURNED_ON
		fsm.subj_.FingerMoved(evt.fingerId, fingerStruct.from, fingerStruct.to);
#endif // UNIT_TESTING_TURNED_ON
	}

	template<typename Subject>
	void UpdateSubject(const PointMove & p1, const PointMove & p2, Subject & subj)
	{
		const ActionType at = GetSubjectAction(p1, p2, DEFAULT_ACTION_DETECTION_ANGLE);

		switch(at)
		{
		case ActionType::Zoom:
			subj.ZoomScene(GetZoomValue(p1, p2));
			break;

		case ActionType::Rotate:
		{
			const Angle a = GetRotateAngle(p1, p2);
			subj.RotateScene(a);
		}
			break;

		case ActionType::Bend:
		{
			const AnglePair ap = GetBendAngles(p1, p2);
			subj.BendScene(ap.first, ap.second);
		}
			break;

		default:
			break;
		}
	}

	template<typename Subject, typename FingersType>
	void FingerCommitImpl(Subject & subj, FingersType & fingers, bool isFinal, bool processPress)
	{
		const int fingerCount = fingers.size();

		if (fingerCount == 0)
			return;

		if (fingerCount == 1)
		{
			const auto & f = fingers.begin()->second;
			const Point2d diff = f.to - f.from;

			if (diff.GetLength() > 0.1f)
			{
				subj.ShiftScene(diff.x(), diff.y());
			}
			else if (processPress)
			{
				subj.PressScene(f.to.x(), f.to.y());
			}
		}
		else
		{
			auto it = fingers.cbegin();
			const auto & p1 = (it++)->second;
			const auto & p2 = it->second;

			UpdateSubject(p1, p2, subj);
		}

		boost::for_each(fingers | boost::adaptors::map_values,
		[](typename FingersType::mapped_type & v)
		{
			v.from = v.to;
		});

		if (isFinal)
			subj.UpdateTerrain();
	}

	ACTION_IMPLEMENTATION(FingerCommit)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD;

		FingerCommitImpl(fsm.subj_, sourceState.fingers, false, false);
	}

	ACTION_IMPLEMENTATION(FingerReset)
	{
		ACTION_IMPLEMENTATION_UNUSED_GUARD;

		// commit first
		FingerCommitImpl(fsm.subj_, sourceState.fingers, true, false);

		#ifdef UNIT_TESTING_TURNED_ON
		fsm.subj_.FingerReset(sourceState.fingers.size());
		#endif // UNIT_TESTING_TURNED_ON

		// we stop processing everything
		sourceState.fingers.clear();

		fsm.subj_.ResumeTerrainUpdate();
	}

} // namespace impl
} // namespace trm

#undef ACTION_IMPLEMENTATION
#undef ACTION_IMPLEMENTATION_UNUSED_GUARD
