#pragma once

#include "SdlUserEvent.h"
#include "UserEventData.h"

namespace trm
{
	inline void RailRoadUpdateEvent(const RailRoadAffectedIds & ids)
	{
		SdlUserEventWrapper evt = SdlUserEventCoder::Encode(ids);
		evt.Emit();
	};

} // namespace trm