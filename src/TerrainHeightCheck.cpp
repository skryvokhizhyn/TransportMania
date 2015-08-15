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
	, minDist_(std::numeric_limits<float>::max())
{}

bool
TerrainHeightCheck::operator () (const Point2d & p, float & height)
{
	Point3d p3(p.x(), p.y(), height);

	Point2d p2d = Point2d::Cast(pvMatrix_ * p3);

	const float dist = utils::GetDistance(p2d, point_);

	if (dist < 0.05f)
	{
		// keep on searching for the closest point
		if (dist < minDist_)
		{
			pointFound_ = p3;
		}
	}
	// getting away from the nearest found point
	else if (pointFound_)
	{
		return false;
	}

	return true;
}

OptionalPoint3d
TerrainHeightCheck::Get() const
{
	return pointFound_;
}