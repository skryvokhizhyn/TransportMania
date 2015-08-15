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

		const RangeRectangle rr = GetRangeRectangle(pS, pE, RailRoad::GetRoadWidth()); 

		const Point2d vec = utils::GetDirectionVector(pS, pE) * RailRoad::GetRoadStep();
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
		md.type = ModelData::Mode::TriangleStrip;

		const Point2d pS = Point2d::Cast(start);
		const Point2d pE = Point2d::Cast(end);

		const RangeRectangle rr = GetRangeRectangle(pS, pE, RailRoad::GetRoadWidth()); 

		const Line lS = utils::GetPerpendicularAtPoint(rr.base, pS);
		const Point2d pS1 = utils::GetIntersectionPoint(lS, rr.bc);
		const Point2d pS2 = utils::GetIntersectionPoint(lS, rr.da);

		const Point3d pSUp3d(pS1.x(), pS1.y(), start.z() + RailRoad::GetRoadZShift());
		const Point3d pSDown3d(pS2.x(), pS2.y(), start.z() + RailRoad::GetRoadZShift());

		const Line lE = utils::GetPerpendicularAtPoint(rr.base, pE);
		const Point2d pE1 = utils::GetIntersectionPoint(lE, rr.bc);
		const Point2d pE2 = utils::GetIntersectionPoint(lE, rr.da);

		const Point3d pEUp3d(pE1.x(), pE1.y(), end.z() + RailRoad::GetRoadZShift());
		const Point3d pEDown3d(pE2.x(), pE2.y(), end.z() + RailRoad::GetRoadZShift());

		md.points.reserve(4);
		md.indexes.reserve(4);
		md.textures.reserve(4);

		using namespace boost::assign;

		md.points += pSUp3d, pSDown3d, pEUp3d, pEDown3d;
		md.indexes += 0, 1, 2, 3;
		const float len = utils::GetDistance(pE, pS);
		md.textures += Point2d(0.0f, 0.0f), Point2d(0.0f, 1.0f), Point2d(len, 0.0f), Point2d(len, 1.0f);
		md.normales.resize(md.points.size(), Point3d(0, 0, 1));
	}
}

void RailRoadRendererLine::Do(const RailRoadLine & rrl, ModelData & md)
{
	const Point3d & start = rrl.GetStart();
	const Point3d & end = rrl.GetEnd();

#ifdef DRAWING_MODE_FULL
	RenderTriangles(start, end, md);
	md.textureId = TextureId::Railway;
#else
	if (!utils::CheckEqual(start.z(), end.z()))
	{
		throw std::runtime_error("RailRoadLine doesn't support height difference");
	}

	RenderLines(start, end, md);
	md.type = ModelData::Mode::Line;
#endif // DRAWING_MODE_FULL
}