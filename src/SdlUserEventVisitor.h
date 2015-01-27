#pragma once

#include "UserEventData.h"

#include <boost/noncopyable.hpp>

namespace trm
{
	class Application;

	class SdlUserEventVisitor
		: boost::noncopyable
	{
	public:
		SdlUserEventVisitor(Application & app);

		void Visit(UserEventData::CloseWindow & e);
		void Visit(UserEventData::PauseApplication & e);

	private:
		Application & app_;
	};

} // namespace trm
