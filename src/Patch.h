#ifndef _PATCH_H_
#define _PATCH_H_

#include "Variance.h"
#include "HeightMap.h"
#include "TriangleNodeCache.h"

namespace trm
{
	struct ModelData;
	class PointNormaleMap;

namespace terrain
{
namespace lod
{
	class TriangleNode;

	class Patch
	{
	public:
		Patch();
		~Patch();

		void Init(const HeightMap & hm, const size_t det);
		bool Tasselate(const Point3d & camera);
		void Render(ModelData & md, PointNormaleMap & normales);
		void ComputeVariance();
		void PutForcedPoint(const Point2d & p);
		void Attach(Patch & p, const Direction dir);

		void Clear();

	private:
		void Reset();

	private:
		typedef std::vector<TriangleNode *> ActiveNodes;

	private:
		TriangleNode * pRootUp_;
		TriangleNode * pRootDown_;
		TriangleNodeCache rootUpCache_;
		TriangleNodeCache rootDownCache_;
		Variance varianceUp_;
		Variance varianceDown_;
		size_t detalization_;
		const HeightMap * pHeightMap_;
	};

} // namespace lod
} // namespace terrain
} // namespace trm

#endif // _PATCH_H_