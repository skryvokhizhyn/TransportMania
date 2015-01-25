#include "TrainDrawableObject.h"
#include "ModelData.h"

#include <boost/range/algorithm/transform.hpp>
#include <boost/lambda/lambda.hpp>

#include <algorithm>

using namespace trm;

namespace
{
	const float Z_TRAIN_SHIFT = 0.6f;
}

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

void 
TrainDrawableObject::UpdateData(ModelData & md)
{
	namespace bl = boost::lambda;

	boost::transform(md.points, md.points.begin(), 
		bl::ret<Point3d>(bl::_1 + Point3d(0.0f, 0.0f, Z_TRAIN_SHIFT)));
}