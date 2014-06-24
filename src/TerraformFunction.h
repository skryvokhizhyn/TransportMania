#ifndef _TERRAFORMFUNCTION_H_
#define _TERRAFORMFUNCTION_H_

#include "Types.h"
#include <boost/function.hpp>

namespace trm
{
	struct Point2d;
	struct Point3d;

	typedef boost::function<AxisType (const Point2d &, const AxisType)> TerraformFunction;

	struct TerraformFunctionFactory
	{
		static TerraformFunction GetConstant(const AxisType z);
		static TerraformFunction GetSpherical(const Point2d & p, const AxisType radii, const AxisType height);
		static TerraformFunction GetRandom(const AxisType max);
		static TerraformFunction GetSphericalRandom(const Point2d & p, const AxisType radii, const AxisType height);
		static TerraformFunction GetAngleLinear(const Point2d & p1, const Point2d & p2, const AxisType height, const AxisType coef);
	};

} // namespace trm

#endif // _TERRAFORMFUNCTION_H_