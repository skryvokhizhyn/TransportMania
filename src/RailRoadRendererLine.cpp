#include "RailRoadRendererLine.h"
#include "RailRoadLine.h"
#include "GeometryUtils.h"
#include "Point2d.h"
#include "RangeRectangle.h"
#include "ModelData.h"

using namespace trm;
using namespace trm::visitor_impl;

void RailRoadRendererLine::Do(const RailRoadLine & rrl, ModelData & md)
{
	const Point3d & start = rrl.GetStart();
	const Point3d & end = rrl.GetEnd();

	if (!utils::CheckEqual(start.z(), end.z()))
	{
		throw std::runtime_error("RailRoadLine doesn't support height difference");
	}

	const Point2d pS = Point2d::Cast(start);
	const Point2d pE = Point2d::Cast(end);
	const Point2d dist = pE - pS;
	Point2d distAbs(std::abs(dist.x()), std::abs(dist.y()));

	const RangeRectangle rr = GetRangeRectangle(pS, pE, RailRoad::RAIL_ROAD_WIDTH); 

	const Point2d vec = utils::GetDirectionVector(pS, pE) * RailRoad::RAIL_ROAD_STEP;
	const Point2d vecAbs(std::abs(vec.x()), std::abs(vec.y()));

	for (Point2d p = pS; distAbs >= Point2d(); p += vec, distAbs -= vecAbs)
	{
		const Line l = utils::GetPerpendicularAtPoint(rr.base, p);
		const Point2d p1 = utils::GetIntersectionPoint(l, rr.bc);
		const Point2d p2 = utils::GetIntersectionPoint(l, rr.da);

		const Point3d pS3d(p1.x(), p1.y(), start.z());
		const Point3d pE3d(p2.x(), p2.y(), start.z());

		md.indexes.push_back(md.points.size());
		md.points.push_back(pS3d);
		md.indexes.push_back(md.points.size());
		md.points.push_back(pE3d);
	}
}