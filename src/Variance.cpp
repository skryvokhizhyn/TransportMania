#include "Variance.h"
#include "GeometryUtils.h"
#include "Logger.h"

using namespace trm::terrain;
using namespace trm::terrain::lod;
using namespace trm;

void 
Variance::Generate(const size_t level, const HeightMap & hm, const Triangle3d & t)
{
	variance_.clear();
	variance_.resize((1 << level) - 1);

	Generate(1, level, hm, t);
}

Variance::Type 
Variance::At(const size_t num) const
{
	return variance_.at(num);
}

bool  
Variance::ShouldContinue(const size_t level)
{
	// calculate variance only up to the 2'nd level
	// as 1'st is already undividable squares
	if (level == 0)
		return false;

	return true;
}

HeightMap::Value  
Variance::Generate(const size_t num, const size_t level,
	const HeightMap & hm, const Triangle3d & t)
{
	if (!ShouldContinue(level))
		return 0;

	const Triangle3dPair tp = utils::SplitTriangle(t, hm);

	const Point3d & pc = tp.first.e();

	const HeightMap::Value h = (t.l().z() + t.r().z()) / 2;
	const HeightMap::Value v = std::abs(pc.z() - h);

	const HeightMap::Value vLeft = Generate(num * 2, level - 1, hm, tp.first);
	const HeightMap::Value vRight = Generate(num * 2 + 1, level - 1, hm, tp.second);

	const HeightMap::Value maxChildVar = std::max(vLeft, vRight);
	const HeightMap::Value accumulatedVariance = std::max(v, maxChildVar);

	variance_[num - 1] = accumulatedVariance;

	return accumulatedVariance;
}

size_t
Variance::GetSize() const
{
	return variance_.size();
}

void 
Variance::Clear()
{
	//VarianceType().swap(variance_);
	variance_.clear();
}