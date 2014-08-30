#pragma once

#include "TriangleNode.h"

#include <list>

namespace trm
{
	struct Point3d;
	class TriangleMapper;

namespace terrain
{
	class HeightMap;

namespace lod
{
	class Variance;

	class TriangleNodeCache
	{
	public:
		void ResetNode(TriangleNodePtr nodePtr);

		bool Tasselate(const size_t det, const Variance & var, const HeightMap & hm,
			const TriangleMapper & tm, const Point3d & camera);

	public:
		using TriangleNodes = std::list<TriangleNodePtr>;

	public:
		static bool Update(TriangleNodes & nodes);

	private:
		TriangleNodes nodes_;
	};

} // namespace lod
} // namespace terrain
} // namespace trm