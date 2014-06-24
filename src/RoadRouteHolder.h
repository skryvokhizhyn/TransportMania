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
		
		const RoadRouteDefinition & GetDefinition() const;

		void Next();

	private:
		RoadRouteDefinition rrd_;
	};

} // namespace trm
