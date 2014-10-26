#pragma once

#include "Size2d.h"
#include "Point3d.h"

#include <map>
#include <functional>

namespace trm
{
	class PointNormaleMap
	{
	private:
		typedef std::map<Size2d, Point3d> NormaleMap;
		using Pred = std::function<bool (const NormaleMap::key_type & k)>;

	public:
		void Put(const Size2d & p, const Point3d & n);
		const Point3d & At(const Size2d & p) const;
		void Clear();
		void RemoveIf(Pred p);

	private:
		NormaleMap normaleMap_;
	};

} // namespace trm