#pragma once

#include "SdlUserEvent.h"
#include "UserEventData.h"

#include <functional>

namespace trm
{
	auto ApplicationPauseEvent = []()
	{
		return []()
		{
			SdlUserEventWrapper evt = SdlUserEventCoder::Encode(UserEventData::PauseApplication());
			evt.Emit();
		};
	};

} // namespace trm
