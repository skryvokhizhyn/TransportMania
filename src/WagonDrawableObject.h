#pragma once

#include "TrainDrawableObject.h"

namespace trm
{
	struct WagonDrawableObject
		: public TrainDrawableObject
	{
		virtual void Render(ModelData & md) override;
	};

} // namespace trm
