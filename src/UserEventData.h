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

} // namespace UserEventData
} // namespace trm
