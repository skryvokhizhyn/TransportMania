#pragma once

#include "Train.h"
#include "RoadRouteHolder.h"
#include "DynamicSceneObject.h"
#include <boost/noncopyable.hpp>

namespace trm
{

namespace impl
{
	template<typename T> class TrainStateMachine;
} // namespace impl

	typedef std::vector<DynamicSceneObjectPtr> DynamicSceneObjectPtrs;

	class TransportManager
		: boost::noncopyable
	{
	public:
		TransportManager(TrainPtr tPtr, RoadRouteHolder rrH);

		bool Update();

		bool Init();
		bool Load();
		bool Move();
		bool Unload();

		const DynamicSceneObjectPtrs & GetDynamicSceneObjects() const;

	private:
		typedef impl::TrainStateMachine<TransportManager> StateMachine;
		typedef std::shared_ptr<StateMachine> ImplPtr;

	private:
		TrainPtr trainPtr_;
		float passed_;
		float distance_;
		float speed_;
		RoadRouteHolder rrH_;
		ImplPtr implPtr_;
		DynamicSceneObjectPtrs dsoPtrs_;
	};

} // namespace trm
