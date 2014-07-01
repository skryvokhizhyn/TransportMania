#include "VehicleMovableObject.h"

using namespace trm;

VehicleMovableObject::VehicleMovableObject(const RoadRoutePtr & r, const Heading h)
	: roadPoint_(r->GetStartingPoint(h))
{
	SetPosition(roadPoint_.Get());
}

void
VehicleMovableObject::Move()
{
	static float dist = 1.0f;
	if (roadPoint_.Move(dist))
	{
		SetPosition(roadPoint_.Get());
	}
	else
	{
		dist *= -1.0f;
	}
}