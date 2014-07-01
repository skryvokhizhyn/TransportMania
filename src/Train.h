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
		Train(TrainPart head);

		void Append(TrainPart tp);
		void Clear();

		const TrainPart Head() const;
		const TrainParts & Parts() const;
		const TrainMoveParameters & MoveParameters() const;
		float Length() const;

		void SetMoveDistance(const float dist);
		float GetMoveDistance() const;

	private:
		void ApplyParameters(const TrainPart & tp);

	private:
		float moveDistance_;
		const TrainPart head_;
		TrainParts trainParts_;
		TrainMoveParameters moveParams_;
		float length_;
	};

	typedef std::shared_ptr<Train> TrainPtr;
	typedef std::weak_ptr<Train> TrainWPtr;

} // namesapce trm
