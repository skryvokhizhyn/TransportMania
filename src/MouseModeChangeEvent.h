#pragma once

#include "SdlUserEvent.h"
#include "UserEventData.h"

#include <functional>

namespace trm
{
	auto MouseModeChangeEvent = [](MoveSceneEventHandlerType type) -> EventAction
	{
		return [=]()
		{
			SdlUserEventWrapper evt = SdlUserEventCoder::Encode(UserEventData::ChangeMouseMode{type});
			evt.Emit();
		};
	};

} // namespace trm
