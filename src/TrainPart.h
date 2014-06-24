#pragma once

#include "TrainTypes.h"
#include <vector>

namespace trm
{
	struct TrainPart
	{
		TrainPart(const TrainPartType tpt)
			: type(tpt)
		{}

		TrainPartType type;
	};

	typedef std::vector<TrainPart> TrainParts;

} // namespace trm
