#pragma once

#include "SdlUserEvent.h"
#include "UserEventData.h"
#include "EventAction.h"

namespace trm
{
	auto SubmitDraftRoadsEvent = [](bool yesNo) -> EventAction
	{
		return [=]()
		{
			SdlUserEventWrapper evt = SdlUserEventCoder::Encode(UserEventData::SubmitDraftRoads{yesNo});
			evt.Emit();
		};
	};

} // namespace trm
