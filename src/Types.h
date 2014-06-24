#ifndef _TYPES_H_
#define _TYPES_H_

#include "Point3d.h"
#include "Size2d.h"
#include <utility>
#include <vector>
#include <map>

namespace trm
{
	typedef float AxisType;
	typedef std::pair<AxisType, AxisType> AxisPairType;
	typedef unsigned IndexType;
	typedef std::vector<IndexType> IndexVector;
	typedef std::vector<Point3d> PointVector;
	typedef std::vector<Point3d> NormaleVector;
	typedef std::map<Size2d, Point3d> PointNormaleMap;
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