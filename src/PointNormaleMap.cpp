#include "PointNormaleMap.h"

using namespace trm;

void
PointNormaleMap::Put(const Size2d & p, const Point3d & n)
{
	auto found = normaleMap_.lower_bound(p);
	if (found != normaleMap_.end() && !NormaleMap::key_compare()(p, found->first))
	{
		found->second += n;
	}
	else
	{
		normaleMap_.emplace_hint(found, p, n);
	}
}

const Point3d &
PointNormaleMap::At(const Size2d & p) const
{
	NormaleMap::const_iterator found = normaleMap_.find(p);

	if (found == normaleMap_.end())
	{
		std::stringstream ss;
		ss << "Point not found in normales map: " << p;
		throw std::runtime_error(ss.str());
	}

	return found->second;
}

void
PointNormaleMap::Clear()
{
	normaleMap_.clear();
}

auto
PointNormaleMap::Data() const -> const NormaleMap &
{
	return normaleMap_;
}