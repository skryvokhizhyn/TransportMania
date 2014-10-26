#include "WorldProjection.h"
#include "MatrixFactory.h"
#include "MatrixUtils.h"

using namespace trm;

namespace
{
	const float FRUSTUM_NEAR_VALUE = 0.001f;
	const float ORTHO_NEAR_VALUE = 0.0001f; // must be smaller than FRUSTUM_NEAR_VALUE
	const float FRUSTUM_FAR_VALUE = 1000.0f;
	const Angle PROJECTION_HORIZONTAL_ANGLE = Degrees(45.0f);
	const float SHIFT_DEFAULT_VALUE = 0.2f;
	const float ZOOM_DEFAULT_VALUE = 1.0f;
	const Point3d SHIFT_DEFAULT_POSITION(0, 0, 100.0f);
	const float CAMERA_MIN_HEIGHT = 20.0f;
	const float CAMERA_MAX_HEIGHT = 500.0f;
	const float DEFAULT_BEND_VALUE = 0.1f;
}

WorldProjection::WorldProjection()
	: xAngle_(Degrees(0.0f))
	, yAngle_(Degrees(0.0f))
	, zAngle_(Degrees(0.0f))
	, shiftPosition_(SHIFT_DEFAULT_POSITION)
{
	UpdateRotateMatrix();
}

void
WorldProjection::SetRatio(const float ratio)
{
	projectionMatrix_ = MatrixFactory::Projection(PROJECTION_HORIZONTAL_ANGLE, ratio, FRUSTUM_NEAR_VALUE, FRUSTUM_FAR_VALUE);
	orthoViewMatrix_ = MatrixFactory::Ortho(ratio) * MatrixFactory::Move(Point3d(0.0f, 0.0f, 1.0f - ORTHO_NEAR_VALUE));
}

void 
WorldProjection::SetAngles(const Angle theta, const Angle psi, const Angle rotate)
{
	xAngle_ = theta;
	yAngle_ = psi;
	zAngle_ = rotate;

	UpdateRotateMatrix();
}

void 
WorldProjection::UpdateRotateMatrix()
{
	rotateMatrix_= MatrixFactory::Rotate(xAngle_, yAngle_, zAngle_);

	UpdateProjectionViewMatrix();
	UpdateCameraPosition();
}

void 
WorldProjection::UpdateCameraPosition()
{
	Point3d horizontalShift = shiftPosition_;
	horizontalShift.z() = 0;
	
	Point4d camera(0, 0, shiftPosition_.z(), 1);
	
	const Point4d camera4d = MatrixFactory::Move(horizontalShift) * (rotateMatrix_ * camera);

	cameraPosition_ = Point3d::Cast(camera4d);
}

void 
WorldProjection::SetShift(const Point3d & shift)
{
	shiftPosition_ = shift;

	UpdateProjectionViewMatrix();
	UpdateCameraPosition();
}

void 
WorldProjection::UpdateProjectionViewMatrix()
{
	Point3d horizontalShift = shiftPosition_;
	horizontalShift.z() = 0;
	Point3d heightShift;
	heightShift.z() = -shiftPosition_.z();

	// transponated matrix is must. it inverts axis to rotate z (updated) and x (locked)
	//const Matrix v = MatrixFactory::Move(heightShift) *
	//	(trm::Transponate(rotateMatrix_) * MatrixFactory::Move(horizontalShift * -1));
	const Matrix v = MatrixFactory::Move(heightShift) *
		rotateMatrix_ * MatrixFactory::Move(horizontalShift * -1);
	projectionViewMatrix_ = projectionMatrix_ * v;
}

void 
WorldProjection::Shift(const float x, const float y)
{
	//const float shift = SHIFT_DEFAULT_VALUE * shiftPosition_.GetLength() / CAMERA_DEFAULT_HEIGHT;

	//cameraPosition_.x() += x * shift;
	//cameraPosition_.y() += y * shift;

	shiftPosition_.x() += x;
	shiftPosition_.y() += y;

	UpdateProjectionViewMatrix();
	UpdateCameraPosition();
}

void 
WorldProjection::Zoom(const float z)
{
	float currentHeight = shiftPosition_.z();
	//const float newHeight = currentHeight + currentHeight / cameraPosition_.GetLength() * z * ZOOM_DEFAULT_VALUE;
	currentHeight += z;

	//if (newHeight >= CAMERA_MAX_HEIGHT || newHeight <= CAMERA_MIN_HEIGHT)
	if (currentHeight >= CAMERA_MAX_HEIGHT || currentHeight <= CAMERA_MIN_HEIGHT)
		return;

	//shiftPosition_.z() = newHeight;
	shiftPosition_.z() = currentHeight;

	UpdateProjectionViewMatrix();
	UpdateCameraPosition();
}

void 
WorldProjection::Rotate(const Angle angle)
{
	zAngle_ += angle;

	UpdateRotateMatrix();
}

// theta - angle between z axis and point
// psi - agngle between y axis and point
void 
WorldProjection::Bend(const Angle dtheta, const Angle dpsi)
{
	xAngle_ += dtheta;
	yAngle_ += dpsi;

	UpdateRotateMatrix();
}

const Point3d &
WorldProjection::GetCameraPosition() const
{
	return cameraPosition_;
}

bool 
WorldProjection::IsPointVisible(const Point3d & p) const
{
	Point4d t = Point4d::Cast(p);
	t.w() = 1.0f;
	Point4d projection = projectionViewMatrix_ * t;

	const float w = abs(projection.w());

	if (abs(projection.x()) > w)
		return false;
	if (abs(projection.y()) > w)
		return false;
	if (abs(projection.z()) > w)
		return false;

	return true;
}

const Matrix &
WorldProjection::GetProjectionViewMatrix() const
{
	return projectionViewMatrix_;
}

const Matrix & 
WorldProjection::GetOrthoViewMatrix() const
{
	return orthoViewMatrix_;
}

Point3d 
WorldProjection::GetScreenCenter() const
{
	Point3d center = shiftPosition_;
	center.z() = 0;
	return center;
}