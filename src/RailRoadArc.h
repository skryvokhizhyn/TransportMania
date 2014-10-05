#pragma once

#include "RailRoadVisitorTmpl.h"
#include "Point3d.h"
#include "Point2d.h"
#include "Angle.h"

namespace trm
{
	class RailRoadArc 
		: public RailRoadVisitorTmpl<RailRoadArc>
	{
	public:
		RailRoadArc(const Point3d & s, const Angle a, const Point2d & c, const Rotation r);

		const Point3d & GetStart() const;
		Angle GetAngle() const;
		const Point2d & GetCenter() const;
		Rotation GetRotation() const;

	private:
		const Point3d start_;
		const Angle angle_;
		const Point2d center_;
		const Rotation rotation_;
	};

} // namespace trm
