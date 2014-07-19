#include "TrainDrawableObjectFactory.h"
#include "LocomotiveDrawableObject.h"
#include "WagonDrawableObject.h"

using namespace trm;

TrainDrawableObjectPtr
TrainDrawableObjectFactory::Create(const TrainPartType tpt)
{
	switch (tpt)
	{
	case TrainPartType::Locomotive:
		return std::make_shared<LocomotiveDrawableObject>();
		
	case TrainPartType::Wagon:
		return std::make_shared<WagonDrawableObject>();

	default:
		throw std::runtime_error("Unhandled train part type in drawable object factory");
	}
}
