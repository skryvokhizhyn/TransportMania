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
		return true;
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

	bool SpiralImpl(const Point2d & p, AxisType & h, const Spiral3d & spiral)
	{
		const Point2d vecStart = Point2d::Cast(spiral.start) - spiral.center;
		const Point2d vecPoint = p - spiral.center;

		const Angle absAngle = utils::GetAngleAbs(spiral.angle);
		const Rotation spiralRotation = utils::GetAngleRotation(spiral.angle);
		Angle pointAngle = utils::GetRotationAngle360(vecStart, vecPoint, utils::GetAngleRotation(spiral.angle));

		if (pointAngle > absAngle)
		{
			const Angle signedAngle = utils::GetSignedAngle180(vecStart, vecPoint);
			if (spiralRotation != utils::GetAngleRotation(signedAngle))
			{
				pointAngle -= Degrees(360);
			}
		}

		h = spiral.start.z() + pointAngle / absAngle * spiral.zShift;

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

	template<typename T>
	class TerraformerFunctionWrapper
		: public trm::TerraformFunction
	{
	public:
		TerraformerFunctionWrapper(const T & t)
			: impl_(t)
		{}

		virtual bool operator () (const Point2d & p, AxisType & h) override
		{
			return impl_(p, h);
		}

	private:
		T impl_;
	};

	template<typename T>
	TerraformFunctionPtr MakeTerraformFunctoinPtr(const T & t)
	{
		return std::make_shared<TerraformerFunctionWrapper<T>>(t);
	}
}

TerraformFunctionPtr
TerraformFunctionFactory::GetConstant(const AxisType z)
{
	return MakeTerraformFunctoinPtr(boost::bind(&ConstantImpl, _1, _2, z));
}

TerraformFunctionPtr
TerraformFunctionFactory::GetSpherical(const Point2d & p, const AxisType radii, const AxisType height)
{
	return MakeTerraformFunctoinPtr(boost::bind(&SphericalImpl, _1, _2, p, radii, height));
}

TerraformFunctionPtr
TerraformFunctionFactory::GetRandom(const AxisType height)
{
	InitRand(RANDOM_MULTIPLIER);

	return MakeTerraformFunctoinPtr(boost::bind(&RandomImpl, _1, _2, height));
}

TerraformFunctionPtr
TerraformFunctionFactory::GetSphericalRandom(const Point2d & p, const AxisType radii, const AxisType height)
{
	InitRand(RANDOM_MULTIPLIER);

	return MakeTerraformFunctoinPtr(boost::bind(&SphericalRandomImpl, _1, _2, p, radii, height));
}

TerraformFunctionPtr
TerraformFunctionFactory::GetLinear(const Point3d & p1, const Point3d & p2)
{
	LinearHeightGetter heightGetter(p1, p2);

	return MakeTerraformFunctoinPtr([=](auto p, auto & h)
	{
		h = heightGetter(p);

		return true;
	});
}

TerraformFunctionPtr
TerraformFunctionFactory::GetIncrease(const AxisType z)
{
	return MakeTerraformFunctoinPtr(boost::bind(&IncreaseImpl, _1, _2, z));
}

TerraformFunctionPtr
TerraformFunctionFactory::GetSpiral(const Spiral3d & s)
{
	return MakeTerraformFunctoinPtr(boost::bind(&SpiralImpl, _1, _2, s));
}
