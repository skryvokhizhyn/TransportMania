#include "StaticSceneObjectFactory.h"
#include "RailRoadDrawableObject.h"
#include "TerrainCoverDrawableObject.h"

using namespace trm;

StaticSceneObjectPtr 
StaticSceneObjectFactory::ForRailRoad(const RailRoadPtr & rrp)
{
	const DrawableObjectPtr drawablePtr = std::make_shared<RailRoadDrawableObject>(rrp);

	return std::make_shared<StaticSceneObject>(drawablePtr);
}

StaticSceneObjectPtr 
StaticSceneObjectFactory::ForTerrainCover(const PointVector & pv)
{
	const DrawableObjectPtr drawblePtr = std::make_shared<TerrainCoverDrawableObject>(pv);

	return std::make_shared<StaticSceneObject>(drawblePtr);
}
