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


} // namespace UserEventData
} // namespace trm
