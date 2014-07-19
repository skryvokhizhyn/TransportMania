#pragma once

#include "RailRoad.h"
#include "RoadRoute.h"
#include <memory>

namespace trm
{
	class RoadPoint
	{
	public:
		RoadPoint(const RoadRoute & r, const Heading h);

		bool Move(float dist);
		const Point3d & Get() const;

	private:
		bool Shift(const Heading h);
		void Init(const Heading h);

	private:
		Heading heading_;
		float chunkLengthPassed_;
		RoadRoute::RoadChunksIterator firstIt_;
		RoadRoute::RoadChunksIterator lastIt_;
		RoadRoute::RoadChunksIterator currIt_;
		Point3d position_;
	};

} // namspace trm