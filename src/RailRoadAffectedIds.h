#pragma once

#include "UniqueId.h"

namespace trm
{
	struct RailRoadAffectedIds
	{
		enum Type
		{
			Permanent,
			Temporary
		};

		RailRoadAffectedIds(Type t) 
			: type(t) {}

		Type type;
		UniqueIdVector addedIds;
	};

} // namespace trm
