#include "TerraformFunction.h"
#include "Point3d.h"
#include "CommonUtils.h"
#include "GeometryUtils.h"
#include "Point2d.h"
#include "Line.h"
#include "LinearHeightGetter.h"

#include <boost/bind.hpp>
#include <boost/units/cmath.hpp>

#include <stdlib.h>
#include <time.h>

using namespace trm;

namespace
{
	const size_t RANDOM_MULTIPLIER = 100;

	bool ConstantImpl(const Point2d & /*p*/, AxisType & h, const AxisType z)
	{
		h = z;
		return true;;
	}

	bool SphericalImpl(const Point2d & p, AxisType & h, const Point2d & center, const AxisType radii, const AxisType height)
	{
		const float dist = utils::GetDistance(p, center);

		if (!utils::CheckEqual(radii, 0.0f) && dist < radii)
		{
			const float t = -(height / (radii * radii)) * dist * dist + height;

			h = (t >= h) ? t : h;
		}

		return true;
	}

	bool RandomImpl(const Point2d & /*p*/, AxisType & h, const AxisType height)
	{
		const int m = static_cast<int>(height * RANDOM_MULTIPLIER);

		/* generate secret number: */
		const int r = rand() % m;
		const AxisType t = static_cast<AxisType>(r) / RANDOM_MULTIPLIER;

		h = (h >= t) ? h : t;

		return true;
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

	bool SphericalRandomImpl(const Point2d & p, AxisType & h, const Point2d & center, const AxisType radii, const AxisType height)
	{
		AxisType valSpherical = 0.0f;
		SphericalImpl(p, valSpherical, center, radii, height / 2);
		AxisType valRandom = 0.0f;
		RandomImpl(p, valRandom, height / 10);

		const float t = valSpherical + valRandom;

		h = (t >= height) ? height : t;

		return true;
	}

	bool LinearImpl(const float initialHeight, const Line & perpendicual, const Angle a, const Point2d & p, AxisType & h)
	{
		const float dist = utils::GetDistance(perpendicual, p);

		h = dist * boost::units::tan(a) + initialHeight;

		return true;
	}

	bool IncreaseImpl(const Point2d & /*p*/, AxisType & h, const AxisType z)
	{
		h += z;

		return true;
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
	LinearHeightGetter heightGetter(p1, p2);

	return [=](auto p, auto & h)
	{
		h = heightGetter(p);

		return true;
	};
}

TerraformFunction 
TerraformFunctionFactory::GetIncrease(const AxisType z)
{
	return boost::bind(&IncreaseImpl, _1, _2, z);
}
