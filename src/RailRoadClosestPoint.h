#pragma once

#include "RailRoadVisitor.h"
#include "Point2d.h"

namespace trm
{
	class RailRoadClosestPoint
		: public RailRoadVisitor
	{
	public:
		RailRoadClosestPoint(const Point2d & p, bool stickToBorders, float bordersTolerance);

		virtual void Visit(RailRoadArc & rra) override;
		virtual void Visit(RailRoadLine & rrl) override;

		const Point3d & GetPoint() const;

	private:
		const Point2d currentPos_;
		const bool stickToBorders_;
		const float bordersTolerance_;
		Point3d foundPos_;
	};

} // namespace trm
