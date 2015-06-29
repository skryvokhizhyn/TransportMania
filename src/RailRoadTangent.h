#pragma once

#include "RailRoadVisitor.h"
#include "Point2d.h"
#include "Point3d.h"
#include "RailRoad.h"
#include "Angle.h"
#include "RailRoadTangentData.h"
#include "OptionalPoint3d.h"

namespace trm
{
	class RailRoadTangent
		: public RailRoadVisitor
	{
	public:
		RailRoadTangent(const Point2d & near, const Point2d & far, Angle invalidAngle);

		virtual void Visit(RailRoadArc & rra) override;
		virtual void Visit(RailRoadLine & rrl) override;

		const OptionalPoint3d & GetTangentVector() const;

	private:
		void ProcessLine(const Point3d & beg3d, const Point3d & end3d);

	private:
		const Angle invalidAngle_;
		Point2d nearPos_;
		Point2d farPos_;
		OptionalPoint3d direction_;
	};

} // namespace trm
