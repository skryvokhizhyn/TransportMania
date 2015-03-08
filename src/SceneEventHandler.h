#pragma once

#include "EventHandler.h"

namespace trm
{
	struct SceneEventHandler
		: public EventHandler
	{
		virtual void Commit() const = 0;
	};

	using SceneEventHandlerPtr = std::shared_ptr<SceneEventHandler>;

} // namespace trm
