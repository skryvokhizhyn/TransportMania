#pragma once

#include <boost/noncopyable.hpp>

namespace trm
{
	class RailRoadLine;
	class RailRoadArc;

	struct RailRoadVisitor
		: private boost::noncopyable
	{
		virtual ~RailRoadVisitor() {}

		virtual void Visit(RailRoadLine & rrl) = 0;
		virtual void Visit(RailRoadArc & rra) = 0;
	};

} // namespace trm