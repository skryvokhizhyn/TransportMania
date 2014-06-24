#pragma once

#include "DynamicSceneObject.h"
#include "VehicleData.h"
#include "RoadRoute.h"

namespace trm
{
	typedef std::vector<DynamicSceneObjectPtr> DynamicSceneObjectPtrs;

	class Train;

	struct DynamicSceneObjectFactory
	{
		//static DynamicSceneObjectPtr ForVehicle(VehicleData vd, RoadRoutePtr rrPtr, const Heading h);
		static DynamicSceneObjectPtrs ForTrain(const TrainPtr & tPtr, const RoadPoint & rp);
	};

} // namespace trm
