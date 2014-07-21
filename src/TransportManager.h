#pragma once

#include "Train.h"
#include "RoadRouteHolder.h"
#include "ComponentId.h"

namespace trm
{
namespace impl
{
	template<typename T> class TrainStateMachine;
} // namespace impl

	struct ComponentHolder;

	class TransportManager
	{
	public:
		TransportManager(ComponentHolder * ch, RoadRouteHolder1 rrh);

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
		ComponentId id_;
		ComponentHolder * componentHolderPtr_;
		float distance_;
		float speed_;
		float passed_;
		RoadRouteHolder1 rrh_;
		TrainMoveParameters moveParams_;
	};

} // namespace trm
