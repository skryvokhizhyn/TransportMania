#include "TerrainRangeArc.h"
#include "GeometryUtils.h"
#include "CommonUtils.h"
#include "Line.h"
#include <boost/units/cmath.hpp>

using namespace trm;

namespace
{
	AxisType GetXShift(const int y, const int radii)
	{
		const double x = sqrt(radii * radii - y * y);

		return boost::numeric_cast<float>(x);
	}

	Point2d GetIntersectionWithVector(const Line & l, const Point2d & vec)
	{
		const Line lVec = utils::GetLine(vec, Point2d());

		return utils::GetIntersectionPoint(l, lVec);
	}
}

TerrainRangeArc::Data::Data(const Point2d & s, const Angle a, const Point2d & c)
: start(s), angle(a), center(c)
{
}

void 
TerrainRangeArc::ProcessRange(const Point2d & vec, const Angle a, const int y, const AxisPairType & pt, const Point2d & c)
{
	const IntersectionType & r = GetIntersection(vec, a, y, pt);

	for (const AxisPairType & apt : r)
	{
		const TerrainRange::Range range(
			utils::FloatFloorToInt(y + c.y()),
			utils::FloatFloorToInt(apt.first + c.x()), 
			utils::FloatCeilToInt(apt.second + c.x())
			);

		PutRange(range);
	}
}

TerrainRangeArc::TerrainRangeArc(const Data & data, AxisType width)
{
	const Point2d vec = data.start - data.center;

	const AxisType radii = vec.GetLength();
	const AxisType radiiMax = radii + width / 2;
	const AxisType radiiMin = (width / 2 < radii) ? radii - width / 2 : 0;

	const int iMin = utils::FloatFloorToInt(-radiiMax);
	const int iMax = utils::FloatCeilToInt(radiiMax);
	const int iRadiiMin = utils::FloatFloorToInt(radiiMin);

	for (int i = iMin; i <= iMax; ++i)
	{
		const AxisType xMax = GetXShift(i, iMax);

		if (std::abs(i) >= iRadiiMin)
		{
			ProcessRange(vec, data.angle, i, AxisPairType(-xMax, xMax), data.center);
		}
		else
		{
			const AxisType xMin = GetXShift(i, iRadiiMin);

			const int lxMin = utils::FloatCeilToInt(-xMin);
			const int rxMin = utils::FloatFloorToInt(xMin);

			if (lxMin == rxMin)
			{
				ProcessRange(vec, data.angle, i, AxisPairType(-xMax, xMax), data.center);
			}
			else
			{
				ProcessRange(vec, data.angle, i, AxisPairType(-xMax, (float)lxMin), data.center);
				ProcessRange(vec, data.angle, i, AxisPairType((float)rxMin, xMax), data.center);
			}
		}
	}
}

auto
TerrainRangeArc::GetIntersection(Point2d vec, Angle a, const int y, const AxisPairType & p) -> IntersectionType
{
	Rotation rot = utils::GetAngleRotation(a);
	a = utils::GetAngleAbs(a);

	if ((y >= 0 && rot == Rotation::AntiClockwise) || (y < 0 && rot == Rotation::Clockwise))
	{
		vec = utils::RotateVector(vec, a, rot);
		rot = (rot == Rotation::AntiClockwise) ? Rotation::Clockwise : Rotation::AntiClockwise;
	}

	IntersectionType r;

	const Point2d::Type yf = boost::numeric_cast<Point2d::Type>(y);
	const Point2d pf(p.first, yf);
	const Point2d pt(p.second, yf);

	Angle aFrom = utils::GetRotationAngle360(vec, pf, rot);
	Angle aTo = utils::GetRotationAngle360(vec, pt, rot);

	const Line l = (pf != pt) ? utils::GetLine(pf, pt) : utils::GetLine(pf, pt + Point2d(1.0f, 0.0f));

	if (y == 0) 
	{
		if ((a >= aFrom - MINIMAL_MEANINGFUL_ANGLE) && p.first <= 0.0f)
		{
			r.emplace_back(p.first, std::min(0.0f, p.second));
		}

		if ((a >= aTo - MINIMAL_MEANINGFUL_ANGLE) && 0.0f <= p.second)
		{
			r.emplace_back(std::max(0.0f, p.first), p.second);
		}
	}
	else 
	{
		if (aFrom > aTo)
		{
			if (a > aFrom)
			{
				const Point2d lpF = GetIntersectionWithVector(l, vec);
				const Point2d vecTo = utils::RotateVector(vec, a, rot);
				const Point2d lpT = GetIntersectionWithVector(l, vecTo);

				r.push_back(AxisPairType(p.first, lpT.x()));
				r.push_back(AxisPairType(lpF.x(), p.second));
			}
			else
			{
				const Point2d lpF = GetIntersectionWithVector(l, vec);

				if (a < aTo)
				{
					const Point2d vecTo = utils::RotateVector(vec, a, rot);
					const Point2d lpT = GetIntersectionWithVector(l, vecTo);

					r.push_back(AxisPairType(lpF.x(), lpT.x()));
				}
				else
				{
					r.push_back(AxisPairType(lpF.x(), p.second));
				}
			}
		}
		else
		if (a >= aTo)
		{
			r.push_back(p);
		}
		else
		if (a > aFrom)
		{
			const Point2d vecTo = utils::RotateVector(vec, a, rot);
			const Point2d lp = GetIntersectionWithVector(l, vecTo);

			r.push_back(AxisPairType(p.first, lp.x()));
		}
	}

	if (r.size() == 2)
	{
		if (r[0].second >= r[1].first)
		{
			r[0].second = r[1].second;

			r.pop_back();
		}
	}

	return r;
}
