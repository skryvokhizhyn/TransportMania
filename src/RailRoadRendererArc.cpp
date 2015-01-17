#include "RailRoadRendererArc.h"
#include "RailRoadArc.h"
#include "ModelData.h"
#include "GeometryUtils.h"
#include "GlobalDefines.h"

#include <boost/range/algorithm/transform.hpp>
#include <boost/lambda/lambda.hpp>

#include <algorithm>

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
	const Rotation rot = rrl.GetRotation();

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
	
	const int pointsToProceed = boost::numeric_cast<int>(std::ceil(angle / rotationStep) + 1.0f) * 2; 
	md.points.reserve(pointsToProceed);
	md.indexes.reserve(pointsToProceed);
	md.textures.reserve(pointsToProceed);

	Angle a = Degrees(0);
	float pathPassed = 0.0f;

	while(a <= angle)
	{
		const Point2d far = utils::RotateVector(farStart, a, rot);
		const Point2d near = utils::RotateVector(nearStart, a, rot);
		
		PushPoint(near, center, h, md);
		PushPoint(far, center, h, md);

		md.textures.push_back(Point2d(pathPassed, 0.0f));
		md.textures.push_back(Point2d(pathPassed, 1.0f));
		
		if (a < angle && (a + rotationStep) > angle)
		{
			a = angle;
		}
		else
		{
			a += rotationStep;
		}

		pathPassed += 1.0f;
	}

#ifdef DRAWING_MODE_FULL
	md.type = ModelData::Mode::TriangleStrip;
	md.textureId = TextureId::Railway;

	using namespace boost::lambda;
		placeholder1_type Arg1;

	boost::transform(md.points, md.points.begin(), 
			ret<Point3d>(Arg1 + Point3d(0.0f, 0.0f, RailRoad::RAIL_ROAD_Z_SHIFT)));

	md.normales.resize(md.points.size(), Point3d(0.0f, 0.0f, 1.0f));
#else
	md.type = ModelData::Mode::Line;
#endif //DRAWING_MODE_FULL
}