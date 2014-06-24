#pragma once

#include "RailRoadVisitor.h"
#include "Point3d.h"

namespace trm
{
	class RailRoadMover
		: public RailRoadVisitor
	{
	public:
		RailRoadMover(Point3d p, const float dist);

		virtual void Visit(RailRoadArc & rra) override;
		virtual void Visit(RailRoadLine & rrl) override;

		const Point3d & GetPosition() const;

	private:
		const float dist_;
		Point3d pos_;
	};

} // namespace trm