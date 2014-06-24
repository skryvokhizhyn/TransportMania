#include "RangeRectangle.h"
#include "GeometryUtils.h"
#include "Angle.h"

using namespace trm;

#include <boost/units/cmath.hpp>

RangeRectangle 
trm::GetRangeRectangle(const Point2d & start, const Point2d & end, const AxisType width)
{
	const Line base = utils::GetLine(start, end);
	const Line ab = utils::GetPerpendicularAtPoint(base, start);
	const Line cd = utils::GetPerpendicularAtPoint(base, end);

	const Angle a = Radians(::atan2f(base.a, -base.b));

	const float aSin = boost::units::sin(a);
	const float aCos = boost::units::cos(a);

	if (utils::CheckEqual(std::abs(aCos), 1.0f))
	{
		const AxisType c0Shift = base.b * (width / 2);

		const Line bc(0, base.b, base.c - aCos * c0Shift);
		const Line da(0, base.b, base.c + aCos * c0Shift);

		return RangeRectangle(base, ab, bc, cd, da);
	}
	else if (utils::CheckEqual(std::abs(aSin), 1.0f))
	{
		const AxisType c90Shift = base.a * (width / 2);

		const Line bc(base.a, 0, base.c + aSin * c90Shift);
		const Line da(base.a, 0, base.c - aSin * c90Shift);

		return RangeRectangle(base, ab, bc, cd, da);
	}
	else
	{
		const float l = width / 2 / boost::units::cos(Degrees(90) - a);

		const Line bc(base.a, base.b, base.c + base.a * l);
		const Line da(base.a, base.b, base.c - base.a * l);

		return RangeRectangle(base, ab, bc, cd, da);
	}
}
