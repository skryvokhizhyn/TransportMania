#ifndef _TRIANGLENODEHANDLER_H_
#define _TRIANGLENODEHANDLER_H_

#include "Types.h"
#include "TriangleNode.h"

namespace trm
{
	struct Point3d;
	struct ModelData;
	class TriangleMapper;

namespace terrain
{
	class HeightMap;

namespace lod
{
	class Variance;
	class TriangleNode;

	struct TriangleNodeHandler
	{
		static void Tasselate(TriangleNode * pTn, const size_t level, const Variance & var,
			const HeightMap & hm, const TriangleMapper & tm, const Point3d & camera);

		static void Render(TriangleNode * pTn, const size_t level, const HeightMap & hm, const TriangleMapper & tm, 
			ModelData & md, PointNormaleMap & normaleMap);

		static void TasselateLeaf(TriangleNode * pTn, const size_t det, const Variance & var, const HeightMap & hm,
			const TriangleMapper & tm, const Point3d & camera);

	private:
		static bool ShouldContinue(const Variance & var, const size_t num, 
			const Point3d & camera, const Point3d & t);

		static bool TasselateImpl(TriangleNode * pTn, const size_t det, const Variance & var, const HeightMap & hm,
			const TriangleMapper & tm, const Point3d & camera);
	};

} // namespace lod
} // namespace terrain
} // namespace trm

#endif // _TRIANGLENODEHANDLER_H_