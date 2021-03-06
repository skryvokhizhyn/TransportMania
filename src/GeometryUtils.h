#ifndef _GEOMETRYUTILS_H_
#define _GEOMETRYUTILS_H_

#include "Triangle3d.h"
#include "Triangle2d.h"
#include "Angle.h"
#include "Rotation.h"
#include "PointWithDirection2d.h"
#include "Codirection.h"
#include "Point2d.h"
#include "Point3d.h"
#include "Line.h"
#include "Arc2d.h"
#include <tuple>

namespace trm
{
namespace terrain
{
	class HeightMap;
} // namespace terrain
} // namespace trm

namespace utils
{
	trm::Line GetLine(const trm::Point2d & a, const trm::Point2d & b);
	bool CheckParallel(const trm::Line & l1, const trm::Line & l2);
	bool CheckPerpendicular(const trm::Line & l1, const trm::Line & l2);
	trm::Line GetPerpendicularAtPoint(const trm::Line & l, const trm::Point2d & p);
	trm::Line GetParallelAtPoint(const trm::Line & l, const trm::Point2d & p);
	bool CheckColinear(const trm::Point2d & p1, const trm::Point2d & p2);
	trm::Point2d RotateVector(const trm::Point2d & p, const trm::Angle a);
	trm::Point2d GetIntersectionPoint(const trm::Line & l1, const trm::Line & l2);
	trm::Angle GetAngle(const trm::Point3d & a, const trm::Point3d & b);
	trm::Angle GetAngle(const trm::Point2d & a, const trm::Point2d & b);
	trm::Angle GetAngleAbs(trm::Angle a);
	trm::Rotation GetAngleRotation(trm::Angle a);
	trm::Angle GetAdjustedAngleByRotation(trm::Angle a, trm::Rotation r);
	// returns angle from (-90, 90) based on sin
	trm::Angle GetSignedAngle(const trm::Point2d & a, const trm::Point2d & b);
	// returns angle from range [-180, 180) based on cos and rotation.
	trm::Angle GetSignedAngle180(const trm::Point2d & v1, const trm::Point2d & v2);
	// return angle from range [0, 360) wrt rotation direction
	trm::Angle GetRotationAngle360(const trm::Point2d & a, const trm::Point2d & b, const trm::Rotation r);
	trm::Triangle3dPair SplitTriangle(const trm::Triangle3d & t, const trm::terrain::HeightMap & hm);
	trm::Triangle2dPair SplitTriangle(const trm::Triangle2d & t);
	trm::Triangle3dPair GetTriangles(const trm::terrain::HeightMap & hm);
	trm::Point2d GetDirectionVector(const trm::Point2d & a, const trm::Point2d & b);
	trm::Point3d GetCrossProduct(const trm::Point3d & a, const trm::Point3d & b);
	trm::Point3d GetNormaleForTriangleNonNormalized(const trm::Triangle3d & t);
	trm::Point3d GetNormaleForTriangleNonNormalized(const trm::Point3d & l, const trm::Point3d & e, const trm::Point3d & r);
	bool CheckTriangleValid(const trm::Triangle3d & t);
	bool CheckTriangleValid(const trm::Point3d & l, const trm::Point3d & e, const trm::Point3d & r);
	trm::Triangle3d GetTriangle3dFrom2d(const trm::Triangle2d & t2d, const trm::terrain::HeightMap & hm);
	template<typename T, unsigned short N>
	float GetDistance(trm::PointImpl<T, N> l, trm::PointImpl<T, N> r);
	// check for codirecsion withing tolerance
	template<typename T, unsigned short N>
	trm::Codirection CheckCodirectionalWithinTolerance(trm::PointImpl<T, N> l, trm::PointImpl<T, N> r, trm::Angle tolerance);
	// check for precise codirections (0.00001f tolerance)
	template<typename T, unsigned short N>
	trm::Codirection CheckCodirectional(trm::PointImpl<T, N> l, trm::PointImpl<T, N> r);
	float GetDistance(const trm::Line & l, const trm::Point2d & p);
	template<typename T, unsigned short N>
	trm::PointImpl<T, N> Normalized(trm::PointImpl<T, N> p);
	bool CheckPointsOnLine(const trm::PointWithDirection2d & p1, const trm::PointWithDirection2d & p2, trm::Angle toleranceAngle);
	bool CheckPointsOnCircle(const trm::PointWithDirection2d & p1, const trm::PointWithDirection2d & p2, trm::Angle toleranceAngle);

} // namespace utils

#endif // _GEOMETRYUTILS_H_