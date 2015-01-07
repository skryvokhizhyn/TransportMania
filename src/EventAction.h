#pragma once

#include <functional>

namespace trm
{
	using EventAction = std::function<void ()>;

	enum class EventActionType
	{
		Repeatable,
		Single
	};

} // namespace trm
