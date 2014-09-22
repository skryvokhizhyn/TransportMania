#include "RailRoadRendererLine.h"
#include "RailRoadLine.h"
#include "GeometryUtils.h"
#include "Point2d.h"
#include "RangeRectangle.h"
#include "ModelData.h"
#include "GlobalDefines.h"

#include <boost/range/algorithm/transform.hpp>
#include <boost/lambda/lambda.hpp>

#pragma warning(push)
#pragma warning(disable: 4512)

#include <boost/assign/std/vector.hpp>

#pragma warning(pop)

using namespace trm;
using namespace trm::visitor_impl;
using namespace boost::assign;

namespace
{
	void RenderLines(const Point3d & start, const Point3d & end, ModelData & md)
	{
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

	void RenderTriangles(const Point3d & start, const Point3d & end, ModelData & md)
	{
		const Point2d pS = Point2d::Cast(start);
		const Point2d pE = Point2d::Cast(end);
		const Point2d dist = pE - pS;

		const RangeRectangle rr = GetRangeRectangle(pS, pE, RailRoad::RAIL_ROAD_WIDTH); 

		const Point2d a = utils::GetIntersectionPoint(rr.ab, rr.da);
		const Point2d b = utils::GetIntersectionPoint(rr.ab, rr.bc);
		const Point2d c = utils::GetIntersectionPoint(rr.bc, rr.cd);
		const Point2d d = utils::GetIntersectionPoint(rr.cd, rr.da);

		md.points += Point3d::Cast(a), Point3d::Cast(b), Point3d::Cast(c), Point3d::Cast(d);

		using namespace boost::lambda;
		placeholder1_type Arg1;

		boost::transform(md.points, md.points.begin(), 
			ret<Point3d>(Arg1 + Point3d(0.0f, 0.0f, start.z() + RailRoad::RAIL_ROAD_Z_SHIFT)));

		md.indexes += 0, 2, 1, 2, 0, 3;

		md.normales += 
			Point3d(0.0f, 1.0f, 1.0f), Point3d(0.0f, 1.0f, 1.0f), Point3d(0.0f, 1.0f, 1.0f), Point3d(0.0f, 1.0f, 1.0f);
	}
}

void RailRoadRendererLine::Do(const RailRoadLine & rrl, ModelData & md)
{
	const Point3d & start = rrl.GetStart();
	const Point3d & end = rrl.GetEnd();

	if (!utils::CheckEqual(start.z(), end.z()))
	{
		throw std::runtime_error("RailRoadLine doesn't support height difference");
	}

	#ifdef DRAWING_MODE_FULL
		RenderTriangles(start, end, md);
		md.type = ModelData::Mode::Triangle;
	#else
		RenderLines(start, end, md);
		md.type = ModelData::Mode::Line;
	#endif // DRAWING_MODE_FULL
}