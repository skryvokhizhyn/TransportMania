#pragma once

#include <cstddef>
#include <vector>

namespace trm
{
	struct Triangle2d;

	class TriangleMapper
	{
	public:
		TriangleMapper(const Triangle2d & t);

		const Triangle2d & GetTriangleByNumber(const size_t num) const;

	private:
		void GenerateTriangle(const size_t num, const size_t level, const Triangle2d & t);

	private:
		std::vector<Triangle2d> triangles_;
	};

} // namespace trm
