#pragma once

#include "RailRoadAcceptorTmpl.h"
#include "Point3d.h"
#include "Point2d.h"
#include "Angle.h"
#include "Rotation.h"

namespace trm
{
	class RailRoadArc 
		: public RailRoadAcceptorTmpl<RailRoadArc>
	{
	public:
		RailRoadArc(const Point3d & s, const Angle a, const Point2d & c);

		const Point3d & GetStart() const;
		Angle GetAngle() const;
		const Point2d & GetCenter() const;

	private:
		const Point3d start_;
		const Angle angle_;
		const Point2d center_;
	};

} // namespace trm
