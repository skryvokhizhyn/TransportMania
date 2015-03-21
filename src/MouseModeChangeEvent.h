#pragma once

#include "SdlUserEvent.h"
#include "UserEventData.h"

#include <functional>

namespace trm
{
	auto MouseModeChangeEvent = []()
	{
		return []()
		{
			SdlUserEventWrapper evt = SdlUserEventCoder::Encode(UserEventData::ChangeMouseMode());
			evt.Emit();
		};
	};

} // namespace trm
