#include "Point3d.h"
#include "Point4d.h"
#include "MatrixFactory.h"
#include "MatrixUtils.h"
#include "CommonUtils.h"
#include "GeometryUtils.h"
#include <boost/units/cmath.hpp>

using namespace trm;
using namespace utils;
using namespace boost::numeric::ublas;

namespace
{
	typedef identity_matrix<AxisType> IdentityMatrix;
	typedef zero_matrix<AxisType> ZeroMatrix;

	typedef Point4d Quaternion;

	Quaternion CreateQuaternion(Angle angle, Point3d axis)
	{
		angle /= 2;

		const float sin_a = boost::units::sin(angle);
		const float cos_a = boost::units::cos(angle);

		axis.Normalize();

		Quaternion q;

		q.x() = axis.x() * sin_a;
		q.y() = axis.y() * sin_a;
		q.z() = axis.z() * sin_a;
		q.w() = cos_a;

		q.Normalize();

		return q;
	}

	Quaternion CreateQuaternion(Angle xAngle, Angle yAngle, Angle zAngle)
	{
		xAngle /= 2;
		yAngle /= 2;
		zAngle /= 2;

		const float sY = boost::units::sin(yAngle);
		const float cY = boost::units::cos(yAngle);
		const float sZ = boost::units::sin(zAngle);
		const float cZ = boost::units::cos(zAngle);
		const float sX = boost::units::sin(xAngle);
		const float cX = boost::units::cos(xAngle);
	 
		Quaternion q;

		q.w() = cY * cZ * cX - sY * sZ * sX;
		q.x() = sY * sZ * cX + cY * cZ * sX;
		q.y() = sY * cZ * cX + cY * sZ * sX;
		q.z() = cY * sZ * cX - sY * cZ * sX;

		q.Normalize();

		return q;
	}

	/*Point3d Normal(const Point3d & a, const Point3d & b)
	{
		return Point3d(a.y() * b.z() - a.z() * b.y(), a.z() * b.x() - a.x() * b.z(), a.x() * b.y() - a.y() * b.x());
	}*/

	Matrix RotateImpl(const Quaternion & q)
	{
		const AxisType xx = q.x() * q.x();
		const AxisType xy = q.x() * q.y();
		const AxisType xz = q.x() * q.z();
		const AxisType xw = q.x() * q.w();

		const AxisType yy = q.y() * q.y();
		const AxisType yz = q.y() * q.z();
		const AxisType yw = q.y() * q.w();

		const AxisType zz = q.z() * q.z();
		const AxisType zw = q.z() * q.w();

		Matrix m = MatrixFactory::Zero();
		
		// transponated to what written here:
		// http://www.cs.princeton.edu/~gewang/projects/darth/stuff/quat_faq.html

		m.at_element(0, 0) = 1 - 2 * ( yy + zz );
		m.at_element(0, 1) = 2 * ( xy - zw );
		m.at_element(0, 2) = 2 * ( xz + yw );

		m.at_element(1, 0) = 2 * ( xy + zw );
		m.at_element(1, 1) = 1 - 2 * ( xx + zz );
		m.at_element(1, 2) = 2 * ( yz - xw );

		m.at_element(2, 0) = 2 * ( xz - yw );
		m.at_element(2, 1) = 2 * ( yz + xw );
		m.at_element(2, 2) = 1 - 2 * ( xx + yy );

		m.at_element(3, 3) = 1;

		return m; 
	}
}

Matrix
MatrixFactory::Identity()
{
	return IdentityMatrix(4);
}

Matrix 
MatrixFactory::Zero()
{
	return ZeroMatrix(4);
}

Matrix
MatrixFactory::Scale(const trm::AxisType val)
{
	Matrix m = Identity();
	m.at_element(0, 0) = val;
	m.at_element(1, 1) = val;
	m.at_element(2, 2) = val;

	return m;
}

Matrix 
MatrixFactory::Move(const Point3d & p)
{
	Matrix m = Identity();
	m.at_element(0, 3) = p.x();
	m.at_element(1, 3) = p.y();
	m.at_element(2, 3) = p.z();

	return m;
}

Matrix
MatrixFactory::Rotate(const Angle xAngle, const Angle yAngle, const Angle zAngle)
{
	const Quaternion q = CreateQuaternion(xAngle, yAngle, zAngle);

	return RotateImpl(q);
}

Matrix
MatrixFactory::Projection(const Angle angle, const float ratio, const float near, const float far)
{
	assert(ratio != 0);
	assert(near < far);
	assert(near > 0);
	assert(far > 0);
	assert(angle < Degrees(180));

	const float r = near * boost::units::tan(angle / 2.0f);
	const float t = r / ratio;

	assert(r != 0);

	Matrix m = Zero();

	m.at_element(0, 0) = near / r;
	
	m.at_element(1, 1) = near / t;

	m.at_element(2, 2) = -(far + near) / (far - near);

	// changed
	m.at_element(3, 2) = -1;
	//m.at_element(2, 3) = -1;

	m.at_element(2, 3) = -2 * far * near / (far - near);
	//m.at_element(3, 2) = -2 * far * near / (far - near);

	return m;
}

//Point4d MatrixFactory::RotateTest(Point4d p, const Angle xAngle, const Angle yAngle, const Angle zAngle)
//{
//	Quaternion q = CreateQuaternion(xAngle, yAngle, zAngle);
//	Quaternion q1(-q.x(), -q.y(), -q.z(), q.w());
//
//	Quaternion r1;
//	r1.w() = q.w()*p.w() - q.x()*p.x() - q.y()*p.y() - q.z()*p.z();
//	r1.x() = q.w()*p.x() + q.x()*p.w() + q.y()*p.z() - q.z()*p.y();
//	r1.y() = q.w()*p.y() + q.y()*p.w() + q.z()*p.x() - q.x()*p.z();
//	r1.z() = q.w()*p.z() + q.z()*p.w() + q.x()*p.y() - q.y()*p.x();
//
//	Quaternion r2;
//	r2.w() = r1.w()*q1.w() - r1.x()*q1.x() - r1.y()*q1.y() - r1.z()*q1.z();
//	r2.x() = r1.w()*q1.x() + r1.x()*q1.w() + r1.y()*q1.z() - r1.z()*q1.y();
//	r2.y() = r1.w()*q1.y() + r1.y()*q1.w() + r1.z()*q1.x() - r1.x()*q1.z();
//	r2.z() = r1.w()*q1.z() + r1.z()*q1.w() + r1.x()*q1.y() - r1.y()*q1.x();
//
//	return r2;
//}

namespace
{
	Quaternion CreateQuaternion(const Point3d & v, const Angle a)
	{
		const float ca = boost::units::cos(a / 2.0f);
		const float sa = boost::units::sin(a / 2.0f);

		Quaternion q(sa * v.x(), sa * v.y(), sa * v.z(), ca);
		q.Normalize();

		return q;
	}
}

Matrix 
MatrixFactory::Rotate(const Point3d & vF, const Point3d & vT, const Point3d & vDefaultRotation /* = Point3d()*/)
{
	Matrix result = Identity();

	const short c = utils::CheckCodirectional(vF, vT);

	Point3d vR;

	if (c != 0)
	{
		vR = vDefaultRotation;
	}
	else
	{
		//vR = utils::GetCrossProductLeft(vF, vT);
		vR = utils::GetCrossProduct(vF, vT);
	}

	vR.Normalize();
	 
	const Angle a = utils::GetAngle(vF, vT);
	const Quaternion q = CreateQuaternion(vR, a);

	return RotateImpl(q);
}
