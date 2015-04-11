#pragma once

#include "VisitorPattern.h"

namespace trm
{
	class RailRoadLine;
	class RailRoadArc;

	using RailRoadVisitor = utils::visitor::Visitor<boost::mpl::vector<RailRoadLine, RailRoadArc>>;

} // namespace trm
