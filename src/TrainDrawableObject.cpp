#include "TrainDrawableObject.h"
#include "ModelData.h"
#include <algorithm>

using namespace trm;

float 
TrainDrawableObject::GetLength() const
{
	if (!length_)
	{
		throw std::runtime_error("Lenght of a train part hasn't been identified");
	}

	return length_.get();
}

void 
TrainDrawableObject::CalculateLength(const ModelData & md)
{
	const auto mm = std::minmax_element(md.points.begin(), md.points.end(),
		[](const Point3d & lhs, const Point3d & rhs)
	{
		return lhs.x() < rhs.x();
	});

	length_ = mm.second->x() - mm.first->x();

	if (length_.get() < 0.0f)
	{
		throw std::runtime_error("Zero train part length identified");
	}
}