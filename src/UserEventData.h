#pragma once

#include "UniqueId.h"
#include "RailRoadAffectedIds.h"
#include "MoveSceneEventHandlerType.h"

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

	struct ChangeMouseMode { MoveSceneEventHandlerType type; };

	struct SubmitDraftRoads { bool yesNo; };

	using RoadAffectedIds = RailRoadAffectedIds;

} // namespace UserEventData
} // namespace trm
