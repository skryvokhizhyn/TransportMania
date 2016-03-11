#ifndef _TYPES_H_
#define _TYPES_H_

#include "Point3d.h"
#include "Size2d.h"
#include <utility>

namespace trm
{
	typedef float AxisType;
	typedef std::pair<AxisType, AxisType> AxisPairType;
	typedef unsigned IndexType;
	typedef unsigned GLuintType;

	enum class Direction
	{
		Down,
		Up,
		Left,
		Right
	};

	enum class Heading
	{
		Forward,
		Backward
	};

} // namespace trm

#endif // _TYPES_H_