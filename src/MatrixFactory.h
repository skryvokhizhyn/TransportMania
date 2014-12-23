#ifndef _MATRIXFACTORY_H_
#define _MATRIXFACTORY_H_

#include "Types.h"
#include "Matrix.h"
#include "Point3d.h"
#include "Angle.h"

namespace trm
{
	struct MatrixFactory
	{
		static Matrix Identity();
		static Matrix Zero();
		static Matrix Scale(const AxisType val);
		static Matrix Move(const Point3d & p);
		static Matrix Rotate(const Angle xAngle, const Angle yAngle, const Angle zAngle);
		static Matrix Rotate(const Point3d & vF, const Point3d & vT, const Point3d & vDefultRotation = Point3d());
		static Matrix Projection(const Angle angle, const float ratio, const float near, const float far);
		static Matrix Ortho(const size_t width, const size_t height);

		//static Point4d RotateTest(Point4d p, const Angle xAngle, const Angle yAngle, const Angle zAngle);
	};

} // namespace trm

#endif // _MATRIXFACTORY_H_