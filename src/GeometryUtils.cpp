#include "Point3d.h"
#include "Point2d.h"
#include "GeometryUtils.h"
#include "Line.h"
#include "HeightMap.h"
#include "Triangle2d.h"

#include <boost/units/cmath.hpp>

#include <math.h>

using namespace trm;
using namespace trm::terrain;

namespace
{
	float GetDenominator(const trm::Line & l1, const trm::Line & l2)
	{
		return l1.a * l2.b - l2.a * l1.b;
	}
}

Line 
utils::GetLine(const Point2d & a, const Point2d & b)
{
	if (a == b)
	{
		throw std::runtime_error("Cannot build a line based on one point");
	}

	return Line(b.y() - a.y(), a.x() - b.x(), a.x() * (a.y() - b.y()) + a.y() * (b.x() - a.x()));
}

bool 
utils::CheckParallel(const trm::Line & l1, const trm::Line & l2)
{
	return utils::CheckEqual(GetDenominator(l1, l2), 0.0f);
}

Point2d 
utils::GetIntersectionPoint(const Line & l1, const Line & l2)
{
	/*if (CheckParallel(l1, l2))
	{
		if (l1.c <= l2.c)
		{
			return Point2d(std::numeric_limits<Point2d::Type>::min(), std::numeric_limits<Point2d::Type>::min());
		}
		else
		{
			return Point2d(std::numeric_limits<Point2d::Type>::max(), std::numeric_limits<Point2d::Type>::max());
		}
	}*/ 

	const float denom = GetDenominator(l1, l2);

	if (utils::CheckEqual(denom, 0.0f))
	{
		throw std::runtime_error("Trying to intersect parallel lines");
	}
	 
	Point2d res;

	if (utils::CheckEqual(l1.a, 0.0f))
	{
		res.y() = -l1.c / l1.b;
	}
	else if (utils::CheckEqual(l2.a, 0.0f))
	{
		res.y() = -l2.c / l2.b;
	}
	else
	{
		res.y() = (l1.c * l2.a - l2.c * l1.a) / denom;
	}

	if (utils::CheckEqual(l1.b, 0.0f))
	{
		res.x() = -l1.c / l1.a;
	}
	else if (utils::CheckEqual(l2.b, 0.0f))
	{
		res.x() = -l2.c / l2.a;
	}
	else
	{
		res.x() = -(l1.c * l2.b - l2.c * l1.b) / denom;
	}

	return res;
}

namespace
{
template<typename T, unsigned short N>
Angle
GetAngleImpl(const PointImpl<T, N> & a, const PointImpl<T, N> & b)
{
	assert((a != PointImpl<T, N>()));
	assert((b != PointImpl<T, N>()));

	float acosVal = std::inner_product(a.Begin(), a.End(), b.Begin(), 0.0f) / (a.GetLength() * b.GetLength());

	if (acosVal > 1.0f)
	{
		acosVal = 1.0f;
	}
	else if (acosVal < -1.0f)
	{
		acosVal = -1.0f;
	}

	return Radians(acos(acosVal));
}

} // namespace

Angle 
utils::GetAngle(const Point3d & a, const Point3d & b)
{
	return GetAngleImpl(a, b);
}

Angle 
utils::GetAngle(const trm::Point2d & a, const trm::Point2d & b)
{
	return GetAngleImpl(a, b);
}

Angle 
utils::GetAngleAbs(trm::Angle a)
{
	if (a < Degrees(0))
	{
		return a * -1.0f;
	}

	return a;
}

Rotation
utils::GetAngleRotation(trm::Angle a)
{
	if (a >= Degrees(0))
	{
		return Rotation::AntiClockwise;
	}
	
	return Rotation::Clockwise;
}

Angle
utils::GetAdjustedAngleByRotation(trm::Angle a, Rotation r)
{
	if (GetAngleRotation(a) == r)
	{
		return a;
	}

	return a * -1.0f;
}

Angle 
utils::GetSignedAngle(const trm::Point2d & a, const trm::Point2d & b)
{
	assert(a != Point2d());
	assert(b != Point2d());

	float asinVal = (a.x() * b.y() - a.y() * b.x()) / (a.GetLength() * b.GetLength());

	if (asinVal > 1.0f)
	{
		asinVal = 1.0f;
	}
	else if (asinVal < -1.0f)
	{
		asinVal = -1.0f;
	}

	return Radians(::asin(asinVal));
}

Angle 
utils::GetSignedAngle180(const trm::Point2d & v1, const trm::Point2d & v2)
{
	Angle a = GetAngle(v1, v2);

	if (CheckCodirectional(RotateVector(v1, a), v2) != Codirection::Same || a == Degrees(180))
	{
		a *= -1;
	}

	return a;
}

Angle 
utils::GetRotationAngle360(const trm::Point2d & a, const trm::Point2d & b, const Rotation r)
{
	const Line l1 = utils::GetLine(Point2d(), a);
	const Line l2 = utils::GetLine(Point2d(), b);

	if (utils::CheckParallel(l1, l2))
	{
		if (utils::CheckColinear(a, b))
		{
			return Degrees(0);
		}
		else
		{
			return Degrees(180);
		}
	}
	else
	{
		Angle aUnsigned = utils::GetAngle(a, b);
		const Angle aSigned = utils::GetSignedAngle(a, b);
			
		if ((aSigned > Degrees(0) && r == Rotation::Clockwise)
			|| (aSigned < Degrees(0) && r == Rotation::AntiClockwise))
		{
			aUnsigned = Degrees(360) - aUnsigned;
		}

		if (aUnsigned == Degrees(360))
		{
			aUnsigned = Degrees(0);
		}

		return aUnsigned;
	}
}

Triangle3dPair
utils::SplitTriangle(const Triangle3d & t, const HeightMap & hm)
{
	Point3d pc = (t.l() + t.r()) / 2;
	pc.z() = hm.At(Point2d::Cast(pc));

	return Triangle3dPair(
		Triangle3d(t.e(), pc, t.l()),
		Triangle3d(t.r(), pc, t.e()));
}

Triangle2dPair
utils::SplitTriangle(const Triangle2d & t)
{
	Point2d pc = (t.l() + t.r()) / 2;
	
	return Triangle2dPair(
		Triangle2d(t.e(), pc, t.l()),
		Triangle2d(t.r(), pc, t.e()));
}

Triangle3dPair 
utils::GetTriangles(const HeightMap & hm)
{
	const float szF = boost::numeric_cast<float>(hm.GetSize()) - 1;

	const HeightMap::Value 
		z00 = hm.At(Point2d(0, 0)),
		zSzSz = hm.At(Point2d(szF, szF)),
		z0Sz = hm.At(Point2d(0, szF)),
		zSz0 = hm.At(Point2d(szF, 0));

	return Triangle3dPair(
		Triangle3d(
			Point3d(0, 0, z00),
			Point3d(0, szF, z0Sz),
			Point3d(szF, szF, zSzSz)),
		Triangle3d(
			Point3d(szF, szF, zSzSz),
			Point3d(szF, 0, zSz0),
			Point3d(0, 0, z00)));
}

bool 
utils::CheckColinear(const Point2d & p1, const Point2d & p2)
{
	if (p1 == Point2d() || p2 == Point2d())
	{
		throw std::runtime_error("Cannot check colinearity for zero vector");
	}

	if (utils::CheckEqual(p2.x(), 0.0f))
	{
		return (utils::CheckEqual(p1.x(), 0.0f) && (p1.y() / p2.y() > 0.0f));
	}

	const float xDiv = p1.x() / p2.x();
	
	if (utils::CheckEqual(p2.y(), 0.0f))
	{
		return (utils::CheckEqual(p1.y(), 0.0f) && (xDiv > 0.0f));
	}

	return utils::CheckNear(p1.y(), p2.y() * std::abs(xDiv), 0.000006f);
}

Point2d 
utils::RotateVector(const trm::Point2d & p, const trm::Angle a)
{
	const float cosa = boost::units::cos(a);
	const float sina = boost::units::sin(a);

	Point2d::Type x = cosa * p.x() - sina * p.y();
	Point2d::Type y = sina * p.x() + cosa * p.y();

	return Point2d(x, y);
}

float 
utils::GetDistance(const trm::Line & l, const trm::Point2d & p)
{
	return std::abs(l.a * p.x() + l.b * p.y() + l.c) / std::sqrt(l.a * l.a + l.b * l.b);
}

bool 
utils::CheckPerpendicular(const trm::Line & l1, const trm::Line & l2)
{
	if (utils::CheckEqual(l1.a, 0.0f) && utils::CheckEqual(l2.b, 0.0f))
	{
		return true;
	}

	if (utils::CheckEqual(l2.a, 0.0f) && utils::CheckEqual(l1.b, 0.0f))
	{
		return true;
	}

	if (utils::CheckParallel(l1, l2))
	{
		return false;
	}

	return utils::CheckEqual(l1.a * l2.a, -l2.b * l1.b);
}

trm::Line 
utils::GetPerpendicularAtPoint(const trm::Line & l, const trm::Point2d & p)
{
	/*if (!utils::CheckEqual(l.a * p.x() + l.b * p.y() + l.c, 0.0f))
	{
		throw std::runtime_error("Point is not on the line");
	}*/

	if (utils::CheckEqual(l.b, 0.0f))
	{
		const AxisType c = -p.y() * l.a;

		return Line(0.0f, l.a, c);
	} 

	if (utils::CheckEqual(l.a, 0.0f))
	{
		const AxisType c = -p.x() * l.b;

		return Line(l.b, 0.0f, c);
	}

	const Line temp(-l.b, l.a, 0);

	const AxisType c = -(temp.a * p.x() + temp.b * p.y());
	 
	return Line(temp.a, temp.b, c);
}

trm::Line 
utils::GetParallelAtPoint(const trm::Line & l, const trm::Point2d & p)
{
	const AxisType c = -l.a * p.x() - l.b * p.y();

	return Line(l.a, l.b, c);
}

Point2d 
utils::GetDirectionVector(const trm::Point2d & a, const trm::Point2d & b)
{
	Point2d p = b - a;
	p.Normalize();

	return p;
}

Point3d 
utils::GetCrossProduct(const trm::Point3d & a, const trm::Point3d & b)
{
	return Point3d(a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(), a.x() * b.y() - a.y() * b.x());
}

//Point3d 
//utils::GetCrossProductLeft(const trm::Point3d & a, const trm::Point3d & b)
//{
//	return -1 * GetCrossProduct(a, b);
//}

Point3d
utils::GetNormaleForTriangleNonNormalized(const trm::Triangle3d & t)
{
	if (!CheckTriangleValid(t))
	{
		throw std::runtime_error("Invalid triangle given for normale calculation");
	}

	return GetCrossProduct(t.l() - t.e(), t.r() - t.e());
}

Point3d
utils::GetNormaleForTriangleNonNormalized(const Point3d & l, const Point3d & e, const Point3d & r)
{
	if (!CheckTriangleValid(l, e, r))
	{
		throw std::runtime_error("Invalid triangle given for normale calculation");
	}

	return GetCrossProduct(l - e, r - e);
}

bool
utils::CheckTriangleValid(const trm::Triangle3d & t)
{
	return CheckTriangleValid(t.l(), t.e(), t.r());
}

bool 
utils::CheckTriangleValid(const Point3d & l, const Point3d & e, const Point3d & r)
{
	return e != l && e != r && l != r;
}

trm::Triangle3d 
utils::GetTriangle3dFrom2d(const trm::Triangle2d & t2d, const trm::terrain::HeightMap & hm)
{
	Triangle3d t3d(Point3d::Cast(t2d.l()), Point3d::Cast(t2d.e()), Point3d::Cast(t2d.r()));

	t3d.l().z() = hm.At(t2d.l());
	t3d.e().z() = hm.At(t2d.e());
	t3d.r().z() = hm.At(t2d.r());

	return t3d;
}
