#pragma once

#include "TrainDrawableObject.h"
#include "TrainTypes.h"

namespace trm
{
	struct TrainDrawableObjectFactory
	{
		static TrainDrawableObjectPtr Create(const TrainPartType tpt);
	};

} // namespace trm
