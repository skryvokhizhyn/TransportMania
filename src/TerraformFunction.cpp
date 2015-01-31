#include "TerraformFunction.h"
#include "Point3d.h"
#include "CommonUtils.h"
#include "GeometryUtils.h"
#include "Point2d.h"
#include "Line.h"

#include <boost/bind.hpp>
#include <boost/units/cmath.hpp>

#include <stdlib.h>
#include <time.h>

using namespace trm;

namespace
{
	const size_t RANDOM_MULTIPLIER = 100;

	AxisType ConstantImpl(const Point2d & /*p*/, const AxisType /*h*/, const AxisType z)
	{
		return z;
	}

	AxisType SphericalImpl(const Point2d & p, const AxisType h, const Point2d & center, const AxisType radii, const AxisType height)
	{
		if (utils::CheckEqual(radii, 0.0f))
		{
			return h;
		}

		const float dist = utils::GetDistance(p, center);

		if (dist >= radii)
		{
			return h;
		}

		const float t = -(height / (radii * radii)) * dist * dist + height;

		return t >= h ? t : h;
	}

	AxisType RandomImpl(const Point2d & /*p*/, const AxisType h, const AxisType height)
	{
		const int m = static_cast<int>(height * RANDOM_MULTIPLIER);

		/* generate secret number: */
		const int r = rand() % m;
		const AxisType t = static_cast<AxisType>(r) / RANDOM_MULTIPLIER;

		return h >= t ? h : t;
	}

	struct RandOnce
	{
		RandOnce(const size_t seed)
		{
			::srand(seed);
		}
	};

	void InitRand(const size_t seed)
	{
		/* initialize random seed: */
		static RandOnce r(seed);
	}

	AxisType SphericalRandomImpl(const Point2d & p, const AxisType h, const Point2d & center, const AxisType radii, const AxisType height)
	{
		const AxisType h1 = SphericalImpl(p, h, center, radii, height / 2);
		const AxisType h2 = RandomImpl(p, 0.0f, height / 10);

		const float t = h1 + h2;

		return t >= height ? height : t;
	}

	float LinearImpl(const float initialHeight, const Line & perpendicual, const Angle a, const Point2d & p, const AxisType /*oldHeight*/)
	{
		const float dist = utils::GetDistance(perpendicual, p);

		return dist * boost::units::tan(a) + initialHeight;;
	}
}

TerraformFunction
TerraformFunctionFactory::GetConstant(const AxisType z)
{
	return boost::bind(&ConstantImpl, _1, _2, z);
}

TerraformFunction
TerraformFunctionFactory::GetSpherical(const Point2d & p, const AxisType radii, const AxisType height)
{
	return boost::bind(&SphericalImpl, _1, _2, p, radii, height);
}

TerraformFunction
TerraformFunctionFactory::GetRandom(const AxisType height)
{
	InitRand(RANDOM_MULTIPLIER);

	return boost::bind(&RandomImpl, _1, _2, height);
}

TerraformFunction
TerraformFunctionFactory::GetSphericalRandom(const Point2d & p, const AxisType radii, const AxisType height)
{
	InitRand(RANDOM_MULTIPLIER);

	return boost::bind(&SphericalRandomImpl, _1, _2, p, radii, height);
}

TerraformFunction 
TerraformFunctionFactory::GetLinear(const Point3d & p1, const Point3d & p2)
{
	const float dist = utils::GetDistance(p1, p2);
	const Angle a = Radians(std::asin((p2.z() - p1.z()) / dist));

	const Point2d p12d = Point2d::Cast(p1);

	const Line lDir = utils::GetLine(p12d, Point2d::Cast(p2));
	const Line lNorm = utils::GetPerpendicularAtPoint(lDir, p12d);

	return boost::bind(&LinearImpl, p1.z(), lNorm, a, _1, _2);
}