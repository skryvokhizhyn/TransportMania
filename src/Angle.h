#ifndef _MYANGLE_H_
#define _MYANGLE_H_

#include <boost/units/quantity.hpp>
#include <boost/units/systems/si/plane_angle.hpp>
#include <boost/units/systems/angle/degrees.hpp>
#include <boost/units/io.hpp>

namespace trm
{
	typedef boost::units::quantity<boost::units::degree::plane_angle, float> Angle;

	inline Angle Degrees(const float val)
	{
		return Angle(val * boost::units::degree::degrees);
	}

	inline Angle Radians(const float val)
	{
		return Angle(val * boost::units::si::radians);
	}

	inline float RawDegrees(const Angle a)
	{
		typedef boost::units::quantity<boost::units::degree::plane_angle, float> DegreeBasedAngle;
		return DegreeBasedAngle(a).value();
	}

	inline float RawRadians(const Angle a)
	{
		typedef boost::units::quantity<boost::units::si::plane_angle, float> RadianBasedAngle;
		return RadianBasedAngle(a).value();
	}

} // namespace trm

#endif // _MYANGLE_H_