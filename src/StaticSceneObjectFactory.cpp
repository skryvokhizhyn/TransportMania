#include "StaticSceneObjectFactory.h"
#include "RailRoadDrawableObject.h"

using namespace trm;

StaticSceneObjectPtr 
StaticSceneObjectFactory::ForRailRoad(const RailRoadPtr & rrp)
{
	const DrawableObjectPtr drawablePtr = std::make_shared<RailRoadDrawableObject>(rrp);

	return std::make_shared<StaticSceneObject>(drawablePtr);
}