#pragma once

#include "RailRoadVisitor.h"
#include "Point2d.h"

namespace trm
{
	class RailRoadClosestPoint
		: public RailRoadVisitor
	{
	public:
		RailRoadClosestPoint(const Point2d & p, bool stickToBorders);

		virtual void Visit(RailRoadArc & rra) override;
		virtual void Visit(RailRoadLine & rrl) override;

		const Point2d & GetPoint() const;

	private:
		Point2d currentPos_;
		bool stickToBorders_;
		Point2d foundPos_;
	};

} // namespace trm
