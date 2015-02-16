#include "TerrainHeightCheck.h"
#include "GeometryUtils.h"
#include "Point4d.h"
#include "Matrix.h"
#include "MatrixUtils.h"
#include "Logger.h"

using namespace trm;

TerrainHeightCheck::TerrainHeightCheck(const Point2d & p, const Matrix & pv)
	: pvMatrix_(pv)
	, point_(p)
{
}

bool
TerrainHeightCheck::operator () (const Point2d & p, float & height)
{
	Point3d p3(p.x(), p.y(), height);

	Point2d p2d = Point2d::Cast(pvMatrix_ * p3);

	utils::Logger().Debug() << "------------ " << p;

	const float dist = utils::GetDistance(p2d, point_);

	dist_ = std::min(dist_, dist);

	if (dist < 0.05f)
	{
		pointFound_ = p3;

		return false;
	}

	return true;
}

TerrainHeightCheck::OptionalPoint
TerrainHeightCheck::Get() const
{
	return pointFound_;
}