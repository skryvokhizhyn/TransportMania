#pragma once

#include "Train.h"
#include "RoadRouteHolder.h"
#include "ComponentHolder.h"

namespace trm
{
namespace impl
{
	template<typename T> class TrainStateMachine;
} // namespace impl

	class TransportManager
	{
	public:
		TransportManager(RoadRouteHolder rrh);

		void Update();

		bool Init();
		bool Load();
		bool Move();
		bool Unload();

	private:
		void CalcMoveParams();

	private:
		typedef impl::TrainStateMachine<TransportManager>  StateMachine;
		typedef std::shared_ptr<StateMachine> StateMachinePtr;

	private:
		StateMachinePtr stateMachinePtr_;
		ComponentHolder::TrainIt trainIt_;
		TrainPartType head_;
		float distance_;
		float speed_;
		float passed_;
		RoadRouteHolder rrh_;
		TrainMoveParameters moveParams_;
	};

} // namespace trm
