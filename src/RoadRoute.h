#pragma once

#include "RailRoad.h"
#include <boost/noncopyable.hpp>
#include <vector>
#include <memory>

namespace trm
{
	class RoadPoint;

	class RoadRoute
		: boost::noncopyable
	{
	public:
		struct RoadChunkType
		{
			RailRoadPtr roadPtr;
			Heading heading;
			float length;

			RoadChunkType();
			RoadChunkType(RailRoadPtr rp, Heading h, float l);
		};

		typedef std::vector<RoadChunkType> RoadChunks;
		typedef RoadChunks::const_iterator RoadChunksIterator;

	public:
		RoadRoute(RoadChunks && roads);

		bool Valid() const;
		RoadPoint GetStartingPoint(const Heading h) const;

		float Length() const;

		friend class RoadPoint;

	private:
		const RoadChunks roads_;
	};

	typedef std::shared_ptr<RoadRoute> RoadRoutePtr;

} // namespace trm
