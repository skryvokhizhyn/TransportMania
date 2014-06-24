#pragma once

#include "TrainDrawableObject.h"

namespace trm
{
	struct LocomotiveDrawableObject
		: public TrainDrawableObject
	{
		virtual void Render(ModelData & md) override;
	};

} // namespace trm