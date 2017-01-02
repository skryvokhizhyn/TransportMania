#pragma once

#include "Size2d.h"
#include "Point3d.h"
#include "PointHash.h"

#include <unordered_map>
#include <functional>

namespace trm
{
	class PointNormaleMap
	{
	public:
		using NormaleMap = std::unordered_map<Size2d, Point3d, Hasher<Size2d>>;

	public:
		void Put(const Size2d & p, const Point3d & n);
		const Point3d & At(const Size2d & p) const;
		void Clear();

		const NormaleMap & Data() const;

	private:
		NormaleMap normaleMap_;
	};

} // namespace trm
