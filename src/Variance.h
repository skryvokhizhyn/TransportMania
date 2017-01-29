#ifndef _VARIANCE_H_
#define _VARIANCE_H_

#include "HeightMap.h"
#include "Triangle3d.h"
#include "PointHash.h"

#include <boost/noncopyable.hpp>

#include <vector>
#include <unordered_set>

namespace trm
{
namespace terrain
{
namespace lod
{
	class Variance
		: boost::noncopyable
	{
	public:
		typedef HeightMap::Value Type;

	public:
		void Generate(const size_t level, const HeightMap & hm, const Triangle3d & t);
		Type At(const size_t num) const;
		size_t GetSize() const;

		void PutForcedPoint(const Size2d & p);
		
		void Clear();

	private:
		bool ShouldContinue(const size_t level);
		HeightMap::Value Generate(const size_t num, const size_t level,
			const HeightMap & hm, const Triangle3d & t);
		HeightMap::Value GetForcedVariance(const Triangle3d & t);

	private:
		typedef std::vector<Type> VarianceType;
		typedef std::unordered_set<Size2d, Hasher<Size2d>> ForcedPointsType;

	private:
		VarianceType variance_;
		ForcedPointsType forcedPoints_;
	};

} // namespace lod
} // namespace terrain
} // namespace trm

#endif // _VARIANCE_H_