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
	{
		return GetForcedVariance(t);
	}

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

HeightMap::Value 
Variance::GetForcedVariance(const Triangle3d & t)
{
	if (forcedPoints_.empty())
	{
		return 0;
	}

	auto foundEdgeIt = forcedPoints_.find(Size2d::Cast(t.e()));

	if (foundEdgeIt != forcedPoints_.end())
	{
		forcedPoints_.erase(foundEdgeIt);
		return std::numeric_limits<HeightMap::Value>::max();
	}

	auto foundLeftIt = forcedPoints_.find(Size2d::Cast(t.l()));

	if (foundLeftIt != forcedPoints_.end())
	{
		forcedPoints_.erase(foundLeftIt);
		return std::numeric_limits<HeightMap::Value>::max();
	}

	auto foundRightIt = forcedPoints_.find(Size2d::Cast(t.r()));

	if (foundRightIt != forcedPoints_.end())
	{
		forcedPoints_.erase(foundRightIt);
		return std::numeric_limits<HeightMap::Value>::max();
	}

	return 0;
}

size_t
Variance::GetSize() const
{
	return variance_.size();
}

void 
Variance::PutForcedPoint(const Size2d & p)
{
	forcedPoints_.insert(p);
}

void 
Variance::Clear()
{
	//VarianceType().swap(variance_);
	variance_.clear();
}