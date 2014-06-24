#pragma once

#include "RailRoadVisitorTmpl.h"
#include "Point3d.h"

namespace trm
{
	class RailRoadLine 
		: public RailRoadVisitorTmpl<RailRoadLine>
	{
	public:
		RailRoadLine(const Point3d & start, const Point3d & end);

		const Point3d & GetStart() const;
		const Point3d & GetEnd() const;

	private:
		const Point3d start_;
		const Point3d end_;
	};

} // namespace trm