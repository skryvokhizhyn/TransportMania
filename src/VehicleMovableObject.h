#pragma once

#include "MovableObject.h"
#include "RoadRoute.h"
#include "RoadPoint.h"

namespace trm
{
	class VehicleMovableObject
		: public MovableObject
	{
	public:
		VehicleMovableObject(const RoadRoutePtr & r, const Heading h);

		virtual void Move() override;

	private:
		RoadPoint roadPoint_;
	};

} // namespace trm
