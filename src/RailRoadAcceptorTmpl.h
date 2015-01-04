#pragma once

#include "RailRoad.h"
#include "VisitorPattern.h"

namespace trm
{
	template<typename ThisType>
	using RailRoadAcceptorTmpl = utils::visitor::Acceptor<RailRoad, ThisType, RailRoadVisitor>;

} // namespace trm
