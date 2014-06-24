#pragma once

#include "RailRoadVisitor.h"
#include "Point3d.h"

namespace trm
{
	class RailRoadParametersTaker
		: public RailRoadVisitor
	{
	public:
		virtual void Visit(RailRoadArc & rra) override;
		virtual void Visit(RailRoadLine & rrl) override;

		const Point3d & GetStart() const;
		const Point3d & GetEnd() const;

	private:
		Point3d start_;
		Point3d end_;
	};

} // namespace trm
