#pragma once

#include "UniqueId.h"

namespace trm
{
	class WindowCloseEvent
	{
	public:
		WindowCloseEvent(UniqueId id);

		void operator () () const;

	private:
		UniqueId id_;
	};

} // namespace trm
