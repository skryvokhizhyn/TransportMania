#pragma once

#include "HeightMapLoader.h"

#include <boost/noncopyable.hpp>
#include <map>

namespace trm
{
	struct Size2d;

namespace terrain
{
	class HeightMapLoaderCache
		: public HeightMapLoader
		, boost::noncopyable
	{
	public:
		HeightMapLoaderCache(const size_t partSize, HeightMapLoaderPtr base);

		// HeightMapLoader
		virtual size_t Size() const override;
		virtual void Get(const Point2d & pos, const size_t sz, HeightMap & hm) const override;
		virtual void Set(const Point2d & pos, const HeightMap & hm) override;

	private:
		using HeightMapCache = std::map<Size2d, HeightMap>;

	private:
		const size_t partSize_;
		mutable HeightMapCache cache_;
		HeightMapLoaderPtr basePtr_;
	};

} // namespace terrain
} // namespace trm
