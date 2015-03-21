#pragma once

#include "UniqueId.h"

namespace trm
{
namespace UserEventData
{
	struct CloseWindow
	{
		UniqueId id;

		CloseWindow(UniqueId i) : id(i) {}
	};

	struct PauseApplication {};

	struct ChangeMouseMode {};

	struct SubmitDraftRoads { bool yesNo; };

} // namespace UserEventData
} // namespace trm
