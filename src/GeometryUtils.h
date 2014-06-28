#ifndef _GEOMETRYUTILS_H_
#define _GEOMETRYUTILS_H_

#include "Triangle3d.h"
#include "Triangle2d.h"
#include "Angle.h"

namespace trm
{
	struct Point2d;
	struct Point3d;
	struct Line;

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
	trm::Point2d RotateVector(const trm::Point2d & p, const trm::Angle a, const trm::Direction dir);
	trm::Point2d GetIntersectionPoint(const trm::Line & l1, const trm::Line & l2);
	trm::Angle GetAngle(const trm::Point3d & a, const trm::Point3d & b);
	trm::Angle GetSignedAngle(const trm::Point2d & a, const trm::Point2d & b);
	trm::Triangle3dPair SplitTriangle(const trm::Triangle3d & t, const trm::terrain::HeightMap & hm);
	trm::Triangle2dPair SplitTriangle(const trm::Triangle2d & t);
	trm::Triangle3dPair GetTriangles(const trm::terrain::HeightMap & hm);
	trm::Point2d GetDirectionVector(const trm::Point2d & a, const trm::Point2d & b);
	trm::Point3d GetCrossProduct(const trm::Point3d & a, const trm::Point3d & b);
	//trm::Point3d GetCrossProductLeft(const trm::Point3d & a, const trm::Point3d & b);
	trm::Point3d GetNormaleForTriangleNonNormalized(const trm::Triangle3d & t);
	bool CheckTriangleValid(const trm::Triangle3d & t);
	trm::Triangle3d GetTriangle3dFrom2d(const trm::Triangle2d & t2d, const trm::terrain::HeightMap & hm);

	template<typename T, unsigned short N>
	inline float GetDistance(const trm::PointImpl<T, N> & lhs, const trm::PointImpl<T, N> & rhs)
	{
		return (lhs - rhs).GetLength();
	}

	template<typename T, unsigned short N>
	inline short CheckCodirectional(trm::PointImpl<T, N> l, trm::PointImpl<T, N> r)
	{
		typedef trm::PointImpl<T, N> PointType;

		l.Normalize();
		r.Normalize();

		if (l == r)
		{
			return 1;
		}
		else if (l == -1.0f * r)
		{
			return -1;
		}
		else
		{
			return 0;
		}
	}

	float GetDistance(const trm::Line & l, const trm::Point2d & p);

	template<typename T, unsigned short N>
	inline trm::PointImpl<T, N> 
	Normalized(trm::PointImpl<T, N> p)
	{
		p.Normalize();
		return p;
	}

} // namespace utils

#endif // _GEOMETRYUTILS_H_