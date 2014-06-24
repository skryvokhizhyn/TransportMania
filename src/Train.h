#pragma once

#include "TrainEngine.h"
#include "RoadRoute.h"
#include "TrainPart.h"
#include "TrainMoveParameters.h"
#include <boost/noncopyable.hpp>
#include <memory>

namespace trm
{
	class Train
		: boost::noncopyable
	{
	public:
		Train(TrainPart head/*, RoadRoutePtr rrp, const Heading h*/);

		void Append(TrainPart tp);
		void Clear();
		//void SetLength(const float len);

		const TrainPart Head() const;
		const TrainParts & Parts() const;
		//const TrainEngine & Engine() const;
		/*const RoadRoute & Route() const;
		Heading RouteHeading() const;*/
		const TrainMoveParameters & MoveParameters() const;
		float Length() const;

		//bool Update();

		void SetMoveDistance(const float dist);
		float GetMoveDistance() const;

	private:
		void ApplyParameters(const TrainPart & tp);

	private:
		//RoadRoutePtr roadRoutePtr_;
		//Heading heading_;
		//TrainEngine engine_;
		float moveDistance_;
		const TrainPart head_;
		TrainParts trainParts_;
		TrainMoveParameters moveParams_;
		float length_;
	};

	typedef std::shared_ptr<Train> TrainPtr;
	typedef std::weak_ptr<Train> TrainWPtr;

} // namesapce trm
