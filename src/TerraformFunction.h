#ifndef _TERRAFORMFUNCTION_H_
#define _TERRAFORMFUNCTION_H_

#include "Types.h"
#include <boost/function.hpp>
#include <memory>

namespace trm
{
	struct Point2d;
	struct Point3d;

	using TerraformFunction = std::function<bool (const Point2d &, AxisType &)>;

	struct TerraformFunctionFactory
	{
		static TerraformFunction GetConstant(const AxisType z);
		static TerraformFunction GetSpherical(const Point2d & p, const AxisType radii, const AxisType height);
		static TerraformFunction GetRandom(const AxisType max);
		static TerraformFunction GetSphericalRandom(const Point2d & p, const AxisType radii, const AxisType height);
		static TerraformFunction GetLinear(const Point3d & p1, const Point3d & p2);
		static TerraformFunction GetIncrease(const AxisType z);
	};

} // namespace trm

#endif // _TERRAFORMFUNCTION_H_