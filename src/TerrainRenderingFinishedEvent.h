#pragma once

#include "SdlUserEvent.h"
#include "UserEventData.h"

namespace trm
{
	auto SendTerrainRenderingFinishedEvent = []()
	{
		SdlUserEventWrapper evt = SdlUserEventCoder::Encode(UserEventData::ActualizeTerrainRenderedData{});
		evt.Emit();
	};

} // namespace trm
