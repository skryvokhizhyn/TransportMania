#ifndef _WORLDPROJECTION_H_
#define _WORLDPROJECTION_H_

#include "Matrix.h"
#include "Point3d.h"
#include "Angle.h"

namespace trm
{
	class WorldProjection
	{
	public:
		WorldProjection();

		void SetRatio(const size_t width, const size_t height);
		void SetAngles(const Angle theta, const Angle psi, const Angle rotate);
		void SetShift(const Point3d & shift);
		
		void Shift(const float x, const float y);
		void Zoom(const float z);
		void Rotate(const Angle angle);
		void Bend(const Angle dtheta, const Angle dpsi);

		const Point3d & GetCameraPosition() const;
		const Matrix & GetProjectionViewMatrix() const;
		const Matrix & GetOrthoViewMatrix() const;
		Point3d GetScreenCenter() const;
		bool IsPointVisible(const Point3d & p) const;

	private:
		void UpdateRotateMatrix();
		void UpdateProjectionViewMatrix();
		void UpdateCameraPosition();

	private:
		// Rotation angles
		Angle xAngle_;
		Angle yAngle_;
		Angle zAngle_;
		// Coord shift
		Point3d shiftPosition_;
		Point3d cameraPosition_;
		// caches
		Matrix projectionMatrix_;
		Matrix projectionViewMatrix_;
		Matrix orthoViewMatrix_;
		Matrix rotateMatrix_;
	};

} // namespace trm

#endif // _WORLDPROJECTION_H_