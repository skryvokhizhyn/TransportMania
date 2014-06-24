#pragma once

#include "DrawableObject.h"
#include "RailRoad.h"

namespace trm
{
	class RailRoadDrawableObject
		: public DrawableObject
	{
	public:
		RailRoadDrawableObject(const RailRoadPtr & rrp);

		virtual void Render(ModelData & md) override;

	private:
		const RailRoadPtr & rrp_;
	};

} // namesapce trm
