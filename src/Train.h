#pragma once

#include "RoadRoute.h"
#include "RoadPoint.h"
#include "TrainPart.h"
#include "TrainMoveParameters.h"
#include <boost/noncopyable.hpp>
#include <boost/optional.hpp>
#include <memory>

namespace trm
{
	class Train
	{
	public:
		Train(TrainPart head);

		void SetRoadPoint(RoadPoint rp);

		void Append(TrainPart tp);
		void ClearParts();

		const TrainPart & Head() const;
		const TrainParts & Parts() const;

		TrainMoveParameters CalcMoveParams();

	private:
		using RoadPointType = boost::optional<RoadPoint>;

	private:
		RoadPointType position_;
		TrainPart head_;
		TrainParts parts_;
	};

} // namesapce trm
