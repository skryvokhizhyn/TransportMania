#include "TrainVisibilityHandler.h"
#include "WorldProjection.h"
#include "TrainMovableObject.h"
#include "ComponentHolder.h"

using namespace trm;

TrainVisibilityHandler::TrainVisibilityHandler(const WorldProjection & wp, ComponentHolder & ch)
	: worldProjection_(wp)
	, componentHolder_(ch)
{}

void
TrainVisibilityHandler::operator () (TrainMovableObject & to) const
{
	const bool isVisible = worldProjection_.IsPointVisible(to.Position());
		
	if (to.GetVisible() == isVisible)
	{
		return;
	}

	// must be before visibles creating
	// visible takes shared state which is initialized when isVisible put as true
	to.SetVisible(isVisible);

	if (isVisible)
	{
		auto sharedPosition = to.SharedPosition();
		const Point3d nextPosition = to.MovedPosition(0.1f);

		componentHolder_.visibles.emplace_back(to.Type(), sharedPosition, nextPosition);
	}
}