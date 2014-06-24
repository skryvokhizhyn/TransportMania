#include "TrainMovableObject.h"

using namespace trm;

TrainMovableObject::TrainMovableObject(const RoadPoint & rp, const TrainWPtr & tPtr, const float initialShift)
	: roadPoint_(rp)
	, trainPtr_(tPtr)
{
	roadPoint_.Move(initialShift);
	SetPosition(roadPoint_.Get());
}

void
TrainMovableObject::Move()
{
	const TrainPtr tPtr = trainPtr_.lock();

	if (!tPtr)
	{
		throw std::logic_error("Train has already been disposed");
	}

	const float dist = tPtr->GetMoveDistance();
	roadPoint_.Move(dist);
	SetPosition(roadPoint_.Get());
}