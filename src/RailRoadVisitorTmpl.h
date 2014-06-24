#pragma once

#include "RailRoad.h"

namespace trm
{
	template<typename ThisType>
	struct RailRoadVisitorTmpl
		: public RailRoad
	{
		virtual void Accept(RailRoadVisitor & v) override
		{
			v.Visit(static_cast<ThisType &>(*this));
		}
	};

} // namespace trm
