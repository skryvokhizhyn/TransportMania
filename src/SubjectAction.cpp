#include "SubjectAction.h"
#include "EventStateMachineDefs.h"
#include "Line.h"
#include "Point2d.h"
#include "GeometryUtils.h"

#include <boost/noncopyable.hpp>

using namespace trm;
using namespace trm::impl;

namespace trm
{
namespace impl
{
	const Angle DEFAULT_ACTION_DETECTION_ANGLE = Degrees(10);

} // namespace impl
} // namespace trm

ActionType 
trm::impl::GetSubjectAction(const PointMove & p1, const PointMove & p2, const Angle limitAngle)
{
	if (!(p1.Valid() && p2.Valid()))
		return ActionType::Undefined;

	const Point3d v1 = Point3d::Cast(p1.to - p1.from);
	const Point3d v2 = Point3d::Cast(p2.to - p2.from);

	const Angle aVectors = utils::GetAngle(v1, v2);

	if (p1.from != p2.from)
	{
		const Point3d vTo = Point3d::Cast(p2.from - p1.from);

		const Angle aTo = utils::GetAngle(v1, vTo);

		if (aTo < limitAngle || aTo > Degrees(180) - limitAngle)
			return ActionType::Zoom;

		if (aVectors > Degrees(180) - limitAngle)
			return ActionType::Rotate;
		else if (aVectors < limitAngle)
			return ActionType::Bend;
	}
	else
	{
		if (aVectors > Degrees(180) - limitAngle)
			return ActionType::Zoom;
		else if (aVectors < limitAngle)
			return ActionType::Bend;
	}

	return ActionType::Undefined;
}

float 
trm::impl::GetZoomValue(const PointMove & p1, const PointMove & p2)
{
	const float len1 = (p1.to - p1.from).GetLength();
	const float len2 = (p2.to - p2.from).GetLength();

	float maxLen = std::max(len1, len2);

	if (utils::GetDistance(p1.from, p2.from) > utils::GetDistance(p1.to, p2.to))
	{
		maxLen *= -1;
	}

	return maxLen;
}

Angle 
trm::impl::GetRotateAngle(const PointMove & p1, const PointMove & p2)
{
	const float len1 = (p1.to - p1.from).GetLength();
	const float len2 = (p2.to - p2.from).GetLength();

	const Line l1 = utils::GetLine(p1.from, p2.from);
	const Line l2 = utils::GetLine(p1.to, p2.to);

	if (utils::CheckParallel(l1, l2))
	{
		return Degrees(0);
	}

	const Point2d mid = utils::GetIntersectionPoint(l1, l2);
	Point2d v1;
	Point2d v2;

	if (len1 >= len2)
	{
		v1 = p1.from - mid;
		v2 = p1.to - mid;
	}
	else
	{
		v1 = p2.from - mid;
		v2 = p2.to - mid;
	}

	return utils::GetSignedAngle180(v1, v2);
}

AnglePair 
trm::impl::GetBendAngles(const PointMove & p1, PointMove p2)
{
	std::swap(p2.from, p2.to);
	const Angle a = GetRotateAngle(p1, p2);

	return AnglePair(a, Angle());
}