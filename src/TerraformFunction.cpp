#include "TerraformFunction.h"
#include "Point3d.h"
#include "CommonUtils.h"
#include "GeometryUtils.h"
#include "Point2d.h"
#include "Line.h"
#include <boost/bind.hpp>
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

	AxisType AngleLinearImpl(const Point2d & p, const AxisType /*h*/, const Line & l, const AxisType height, const AxisType coef)
	{
		const float dist = utils::GetDistance(l, p);

		return height + dist * coef;
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
TerraformFunctionFactory::GetAngleLinear(const Point2d & p1, const Point2d & p2, const AxisType height, const AxisType coef)
{
	const Line lDir = utils::GetLine(p1, p2);
	const Line lNorm = utils::GetPerpendicularAtPoint(lDir, p1);

	return boost::bind(&AngleLinearImpl, _1, _2, lNorm, height, coef);
}