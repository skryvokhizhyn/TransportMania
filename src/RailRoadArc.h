#pragma once

#include "RailRoadAcceptorTmpl.h"
#include "Spiral3d.h"

namespace trm
{
	class RailRoadArc 
		: public RailRoadAcceptorTmpl<RailRoadArc>
	{
	public:
		RailRoadArc(const Spiral3d & data);

		const Point3d & GetStart() const;
		Angle GetAngle() const;
		const Point2d & GetCenter() const;
		float GetZShift() const;
		const Spiral3d & GetSpiral() const;

	private:
		const Spiral3d data_;
	};

} // namespace trm
