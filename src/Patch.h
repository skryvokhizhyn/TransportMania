#ifndef _PATCH_H_
#define _PATCH_H_

#include "Variance.h"
#include "HeightMap.h"
#include "TriangleNodeCache.h"

namespace trm
{
	struct ModelData;

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
		void Render(ModelData & md);
		void ComputeVariance();
		void Attach(Patch & p, const Direction dir);

		void Clear();
		void ZipNormales();

		const PointNormaleMap & GetNormales() const;

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
		PointNormaleMap normaleMap_;
		size_t detalization_;
		const HeightMap * pHeightMap_;
	};

} // namespace lod
} // namespace terrain
} // namespace trm

#endif // _PATCH_H_