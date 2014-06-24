#include "TerrainRangeArc.h"
#include "GeometryUtils.h"
#include "CommonUtils.h"
#include "Line.h"
#include <boost/units/cmath.hpp>

using namespace trm;

namespace
{
	AxisType GetShift(const int y, const AxisType radii)
	{
		const float x = sqrt(radii * radii - y * y);

		return x;
	}

	Angle GetIntersectionAngle(const Point2d & vecFrom, const Point2d & vecTo, const Direction dir)
	{
		const Line l1 = utils::GetLine(Point2d(), vecFrom);
		const Line l2 = utils::GetLine(Point2d(), vecTo);

		if (utils::CheckParallel(l1, l2))
		{
			if (utils::CheckColinear(vecFrom, vecTo))
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
			const Angle aUnsigned = utils::GetAngle(Point3d::Cast(vecFrom), Point3d::Cast(vecTo));
			const Angle aSigned = utils::GetSignedAngle(vecFrom, vecTo);
			
			if ((aSigned > Degrees(0) && dir == Direction::Left)
				|| (aSigned < Degrees(0) && dir == Direction::Right))
			{
				return Degrees(360) - aUnsigned;
			}

			return aUnsigned;
		}
	}

	Point2d GetIntersectionWithVector(const Line & l, const Point2d & vec)
	{
		const Line lVec = utils::GetLine(vec, Point2d());

		return utils::GetIntersectionPoint(l, lVec);
	}
}

TerrainRangeArc::Data::Data(const Point2d & s, const Angle a, const Point2d & c, const Direction d)
: start(s), angle(a), center(c), dir(d)
{
	if (dir != Direction::Left && dir != Direction::Right)
	{
		throw std::runtime_error("Incorrect direction value. Only Left or Right is supported");
	}
}

void 
TerrainRangeArc::ProcessRange(const Point2d & vec, const Angle a, const Direction dir, const int y, const AxisPairType & pt, const Point2d & c)
{
	const IntersectionType & r = GetIntersection(vec, a, dir, y, pt);

	for (const AxisPairType & apt : r)
	{
		const TerrainRange::Range range(
			/*utils::FloatToNearInt(y + c.y(), &floor),
			utils::FloatToNearInt(apt.first + c.x(), &floor), 
			utils::FloatToNearInt(apt.second + c.x(), &ceil)*/
			utils::FloatFloorToInt(y + c.y()),
			utils::FloatFloorToInt(apt.first + c.x()), 
			utils::FloatCeilToInt(apt.second + c.x())
			);

		PutRange(range);
	}
}

TerrainRangeArc::TerrainRangeArc(const Data & data, AxisType width)
{
	width += 2.0;

	const Point2d vec1 = data.start - data.center;

	const AxisType radii = vec1.GetLength();
	const AxisType radiiMax = radii + width / 2;
	const AxisType radiiMin = (width / 2 < radii) ? radii - width / 2 : 0;

	ProcessRange(vec1, data.angle, data.dir,
			//utils::FloatToNearInt(-radiiMax, &floor),
			utils::FloatFloorToInt(-radiiMax),
			AxisPairType(0.0f, 0.0f), data.center);

	/*const int iMin = utils::FloatToNearInt(-radiiMax, &ceil);
	const int iMax = utils::FloatToNearInt(radiiMax, &floor);
	const int iRadiiMin = utils::FloatToNearInt(radiiMin, &floor);*/
	const int iMin = utils::FloatCeilToInt(-radiiMax);
	const int iMax = utils::FloatFloorToInt(radiiMax);
	const int iRadiiMin = utils::FloatFloorToInt(radiiMin);

	for (int i = iMin; i <= iMax; ++i)
	{
		const AxisType yMax = GetShift(i, radiiMax);

		if (std::abs(i) >= iRadiiMin)
		{
			ProcessRange(vec1, data.angle, data.dir, i, AxisPairType(-yMax, yMax), data.center);
		}
		else
		{
			const AxisType yMin = GetShift(i, radiiMin);

			/*const int lyMin = utils::FloatToNearInt(-yMin, &ceil);
			const int ryMin = utils::FloatToNearInt(yMin, &floor);*/
			const int lyMin = utils::FloatCeilToInt(-yMin);
			const int ryMin = utils::FloatFloorToInt(yMin);

			if (lyMin == ryMin)
			{
				ProcessRange(vec1, data.angle, data.dir, i, AxisPairType(-yMax, yMax), data.center);
			}
			else
			{
				ProcessRange(vec1, data.angle, data.dir, i, AxisPairType(-yMax, (float)lyMin), data.center);
				ProcessRange(vec1, data.angle, data.dir, i, AxisPairType((float)ryMin, yMax), data.center);
			}
		}
	}

	// top
	ProcessRange(vec1, data.angle, data.dir, 
		//utils::FloatToNearInt(radiiMax, &ceil), 
		utils::FloatCeilToInt(radiiMax), 
		AxisPairType(0.0f, 0.0f), data.center);
}

auto
TerrainRangeArc::GetIntersection(Point2d vec, const Angle a, Direction d, const int y, const AxisPairType & p) -> IntersectionType
{
	if ((y >= 0 && d == Direction::Right) || (y < 0 && d == Direction::Left))
	{
		vec = utils::RotateVector(vec, a, d);
		d = (d == Direction::Right) ? Direction::Left : Direction::Right;
	}

	IntersectionType r;

	const Point2d::Type yf = boost::numeric_cast<Point2d::Type>(y);
	const Point2d pf(p.first, yf);
	const Point2d pt(p.second, yf);

	Angle aFrom = GetIntersectionAngle(vec, pf, d);
	Angle aTo = GetIntersectionAngle(vec, pt, d);

	const Line l = (pf != pt) ? utils::GetLine(pf, pt) : utils::GetLine(pf, pt + Point2d(1.0f, 0.0f));

	if ((aFrom == Degrees(0) || aFrom == Degrees(180))
		&& (aTo == Degrees(0) || aTo == Degrees(180)))
	{
		r.push_back(p);
	}
	else
	{
		if (aFrom > aTo)
		{
			if (a > aFrom)
			{
				const Point2d lpF = GetIntersectionWithVector(l, vec);
				const Point2d vecTo = utils::RotateVector(vec, a, d);
				const Point2d lpT = GetIntersectionWithVector(l, vecTo);

				r.push_back(AxisPairType(p.first, lpT.x()));
				r.push_back(AxisPairType(lpF.x(), p.second));
			}
			else
			{
				const Point2d lpF = GetIntersectionWithVector(l, vec);

				if (a < aTo)
				{
					const Point2d vecTo = utils::RotateVector(vec, a, d);
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
		if (a > aTo)
		{
			r.push_back(p);
		}
		else
		if (a > aFrom)
		{
			const Point2d vecTo = utils::RotateVector(vec, a, d);
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
