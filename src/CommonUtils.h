#ifndef _COMMONUTILS_H_
#define _COMMONUTILS_H_

#include <boost/math/constants/constants.hpp>
#include <boost/type_traits/is_float.hpp>
#include <boost/format.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <limits>
#include <cmath>
#include <type_traits>
#include <math.h>

namespace utils
{
namespace priv
{
	template<bool B> struct CheckEqualImpl
	{
		template<typename T>
		static bool Compare (const T v1, const T v2, const T tolerance)
		{
			return v1 - v2 <= tolerance;
		}
	};

	template<> struct CheckEqualImpl<true>
	{
		template<typename T>
		static bool Compare(const T v1, const T v2, const T tolerance)
		{
			return ::fabs(v1 - v2) < tolerance;
		}
	};

} // namespace priv

/////////////////////

	#include <boost/utility/enable_if.hpp>
#include <boost/mpl/identity.hpp>

	template<typename T>
	bool CheckEqual(const T v1, const T v2)
	{
		static_assert(!std::is_floating_point<T>::value, "Floating points cannot be compared using ==. Use CheckNear instead.");

		return v1 == v2;
	}

	template<typename T>
	bool CheckNear(const T v1, const T v2, const T tolerance)
	{
		static_assert(std::is_floating_point<T>::value, "Only floating types can be compared using CheckNear. " \
			"Use own function overloading.");

		return ::fabs(v1 - v2) < tolerance;
	}

	inline bool CheckEqual(const float v1, const float v2)
	{
		return CheckNear(v1, v2, std::numeric_limits<float>::epsilon());
	}

	inline bool CheckEqual(const double v1, const double v2)
	{
		return CheckNear(v1, v2, std::numeric_limits<double>::epsilon());
	}

	inline bool CheckEqual(const long double v1, const long double v2)
	{
		return CheckNear(v1, v2, std::numeric_limits<long double>::epsilon());
	}

//////////////////////

	/*template<typename T>
	struct EqualityChecker
	{
		EqualityChecker(const T t = std::numeric_limits<T>::epsilon())
			: tolerance(t)
		{}

		bool operator()(const T v1, const T v2)
		{
			return priv::CheckEqualImpl<boost::is_float<T>::value>::Compare(v1, v2, tolerance);
		}

	private:
		void operator = (const EqualityChecker &);

	private:
		const T tolerance;
	};
*/
	/*template<typename T>
	inline bool CheckEqual(const T v1, const T v2)
	{
		return CheckEqual1(v1, v2);
	}*/

	//template<typename T>
	//inline bool CheckEqual(const T v1, const T v2, const T tolerance)
	//{
	//	return CheckNear(v1, v2, tolerance);
	//}

	inline float ToRadians(const float angle)
	{
		return angle * boost::math::constants::pi<float>() / 180.0f;
	}

	inline float ToAngle(const float radians)
	{
		return radians * 180.0f / boost::math::constants::pi<float>();
	}

	inline int FloatToNearInt(const float f, float (*c)(float))
	{
		return boost::numeric_cast<int>(c(f));
	}

	inline int FloatFloorToInt(float f)
	{
		return FloatToNearInt(f, &floorf);
	}

	inline int FloatCeilToInt(float f)
	{
		return FloatToNearInt(f, &ceilf);
	}


	template<bool shouldThrow>
	size_t GetPowerOf2(const size_t val);

	size_t GetPowerOf2(const size_t val);

	template<>
	inline size_t GetPowerOf2<false>(const size_t val)
	{
		if (val == 1)
			return 0;

		const size_t shifted = val >> 1;

		return GetPowerOf2<false>(shifted) + 1;
	}

	template<>
	inline size_t GetPowerOf2<true>(const size_t val)
	{
		const size_t powerOf2 = GetPowerOf2<false>(val);

		if (static_cast<size_t>(1 << powerOf2) != val)
		{
			throw std::runtime_error((boost::format("%s is not a power of 2") % val).str());
		}

		return powerOf2;
	}

	inline size_t GetPowerOf2(const size_t val)
	{
		return GetPowerOf2<true>(val);
	}

	size_t FloatHash(const float v);

} // namespace utils

#endif // _COMMONUTILS_H_
