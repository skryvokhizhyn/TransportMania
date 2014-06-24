#include "RailRoadRendererArc.h"
#include "RailRoadArc.h"
#include "ModelData.h"
#include "GeometryUtils.h"

using namespace trm;
using namespace trm::visitor_impl;

namespace
{
	void PushPoint(const Point2d & p, const Point2d & c, const AxisType h, ModelData & md)
	{
		Point3d p3d = Point3d::Cast(p + c);
		p3d.z() = h;

		md.indexes.push_back(md.points.size());
		md.points.push_back(p3d);
	}
}

void RailRoadRendererArc::Do(const RailRoadArc & rrl, ModelData & md)
{
	const Point3d & start = rrl.GetStart();
	const Angle angle = rrl.GetAngle();
	const Point2d & center = rrl.GetCenter();
	const Direction dir = rrl.GetDirection();

	const AxisType h = start.z();
	const Point2d s = Point2d::Cast(start);

	const Point2d vec1 = s - center;

	const AxisType halfWidth = RailRoad::RAIL_ROAD_WIDTH / 2;
	const AxisType radii = vec1.GetLength();
	const AxisType radiiMax = radii + halfWidth;
	const AxisType radiiMin = radii - halfWidth;
	const Point2d farStart = Point2d::Cast(vec1 * radiiMax / radii);
	const Point2d nearStart = Point2d::Cast(vec1 * radiiMin / radii);

	const Angle rotationStep = Radians(2 * std::asin(RailRoad::RAIL_ROAD_STEP / 2 / radiiMax));

	for (Angle a = rotationStep; a < angle; a += rotationStep)
	{
		const Point2d far = utils::RotateVector(farStart, a, dir);
		const Point2d near = utils::RotateVector(nearStart, a, dir);

		PushPoint(far, center, h, md);
		PushPoint(near, center, h, md);

		a += rotationStep;
	}
}