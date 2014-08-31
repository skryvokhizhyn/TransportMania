#include "TriangleMapper.h"
#include "GeometryUtils.h"

using namespace trm;

TriangleMapper::TriangleMapper(const Triangle2d & t)
{
	const float length = utils::GetDistance(t.l(), t.e());
	const size_t size = boost::numeric_cast<size_t>(length);

	const size_t det = utils::GetPowerOf2(size);
	const size_t level = 2 * det + 2; // + 2 means that we need 1 level smaller than the smallest triangle
	triangles_.resize((1 << level) - 1);

	GenerateTriangle(1, level, t);
}

const Triangle2d & 
TriangleMapper::GetTriangleByNumber(const size_t num) const
{
	if (num == 0)
	{
		throw std::logic_error("Zero triangle id passed");
	}

	return triangles_.at(num - 1);
}

void 
TriangleMapper::GenerateTriangle(const size_t num, const size_t level, const Triangle2d & t)
{
	if (level == 0)
		return;

	triangles_[num - 1] = t;
	const Triangle2dPair tp = utils::SplitTriangle(t);
			
	GenerateTriangle(num * 2, level - 1, tp.first);
	GenerateTriangle(num * 2 + 1, level - 1, tp.second);
}