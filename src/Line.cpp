#include "Line.h"
#include "CommonUtils.h"

using namespace trm;

Line::Line(const AxisType i, const AxisType k, const AxisType l)
	: a(i), b(k), c(l)
{
	if (utils::CheckEqual(a, 0.0f) && utils::CheckEqual(b, 0.0f))
	{
		throw std::runtime_error("Invalid Line created");
	}
}

//AxisType 
//Line::AtX(const AxisType x) const
//{
//	if (utils::CheckEqual(b, 0.0f))
//	{
//		return -c / b;
//	}
//	
//	return (-a * x - c) / b;
//}
//		
//AxisType 
//Line::AtY(const AxisType y) const
//{
//	if (utils::CheckEqual(a, 0.0f))
//	{
//		return -c / a;
//	}
//
//	return (-b * y - c) / a;
//}