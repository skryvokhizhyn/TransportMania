#ifndef _TERRAFORMFUNCTION_H_
#define _TERRAFORMFUNCTION_H_

#include "Types.h"
#include "Spiral3d.h"

namespace trm
{
	struct Point2d;
	struct Point3d;

	struct TerraformFunction
	{
		virtual ~TerraformFunction() {}

		virtual bool operator () (const Point2d &, AxisType &) = 0;
	};

	using TerraformFunctionPtr = std::shared_ptr<TerraformFunction>;

	struct TerraformFunctionFactory
	{
		static TerraformFunctionPtr GetConstant(const AxisType z);
		static TerraformFunctionPtr GetSpherical(const Point2d & p, const AxisType radii, const AxisType height);
		static TerraformFunctionPtr GetRandom(const AxisType max);
		static TerraformFunctionPtr GetSphericalRandom(const Point2d & p, const AxisType radii, const AxisType height);
		static TerraformFunctionPtr GetLinear(const Point3d & p1, const Point3d & p2);
		static TerraformFunctionPtr GetIncrease(const AxisType z);
		static TerraformFunctionPtr GetSpiral(const Spiral3d & s);
	};

} // namespace trm

#endif // _TERRAFORMFUNCTION_H_