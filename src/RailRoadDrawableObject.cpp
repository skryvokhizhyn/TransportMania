#include "RailRoadDrawableObject.h"
#include "ModelData.h"
#include "RailRoadRenderer.h"

using namespace trm;

RailRoadDrawableObject::RailRoadDrawableObject(const RailRoadPtr & rrp)
	: rrp_(rrp)
{
}

void
RailRoadDrawableObject::Render(ModelData & md)
{
	RailRoadRenderer rrr(md);

	rrp_->Accept(rrr);
}
