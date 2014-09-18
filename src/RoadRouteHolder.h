#pragma once

#include "RoadRoute.h"

namespace trm
{
	struct RoadRouteDefinition
	{
		RoadRoutePtr roadRotePtr;
		Heading heading;
	};

	class RoadRouteHolder
	{
	public:
		RoadRouteHolder(RoadRoutePtr rrp, const Heading h);

		void Next();

		RoadPoint GetStartingPoint();
		float GetRouteLength();

	private:
		RoadRoutePtr roadRoutePtr_;
		Heading heading_;
	};

} // namespace trm
