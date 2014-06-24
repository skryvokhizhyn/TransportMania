#include "DynamicSceneObjectFactory.h"
#include "VehicleMovableObject.h"
#include "VehicleDrawableObject.h"
#include "Train.h"
#include "TrainMovableObject.h"
#include "WeakStoringCache.h"
#include "TrainDrawableObjectFactory.h"
#include <boost/range/algorithm.hpp>

using namespace trm;

//DynamicSceneObjectPtr 
//DynamicSceneObjectFactory::ForVehicle(VehicleData /*vd*/, RoadRoutePtr rrPtr, const Heading h)
//{
//	const auto vdoPtr = std::make_shared<VehicleDrawableObject>();
//
//	const auto vmoPtr = std::make_shared<VehicleMovableObject>(rrPtr, h);
//		
//	return std::make_shared<DynamicSceneObject>(vdoPtr, vmoPtr, TrainPtr());
//}

DynamicSceneObjectPtrs 
DynamicSceneObjectFactory::ForTrain(const TrainPtr & tPtr, const RoadPoint & rp)
{
	DynamicSceneObjectPtrs ret;

	const TrainParts & tps = tPtr->Parts();
	//const TrainEngine & te = tPtr->Engine();
	//const RoadRoute & rr = tPtr->Route();
	//const Heading h = tPtr->RouteHeading();

	//const RoadPoint rp = rr.GetStartingPoint(h);

	float trainLength = 0.0f;

	boost::range::for_each(tps,
		[&](const TrainParts::value_type & tp)
	{
		const auto tdoPtr = TrainDrawableObjectFactory::Create(tp.type);
		const auto tmoPtr = std::make_shared<TrainMovableObject>(rp, tPtr, trainLength);

		auto dso = std::make_shared<DynamicSceneObject>(tdoPtr, tmoPtr/*, tPtr*/);

		ret.push_back(std::move(dso));

		trainLength += tdoPtr->GetLength();
	});

	//tPtr->SetLength(trainLength);

	return ret;
} 