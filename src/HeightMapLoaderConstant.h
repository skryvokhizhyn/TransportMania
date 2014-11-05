#pragma once

#include "HeightMapLoader.h"

namespace trm
{
namespace terrain
{
	class HeightMapLoaderConstant
		: public HeightMapLoader
	{
	public:
		HeightMapLoaderConstant(const size_t totalSize, const float patchV, const float pointV, const float increment = 0.0f);

		HeightMapLoaderConstant & operator = (const HeightMapLoaderConstant & h) = delete;

		// HeightMapLoader overrides
		virtual size_t Size() const override;
		virtual void Get(const Point2d & pos, const size_t sz, HeightMap & hm) const override;
		virtual void Set(const Point2d & pos, const HeightMap & hm) override;

	private:
		const size_t totalSize_;
		const float patchValue_;
		const float pointValue_;
		const float increment_;
	};

} // namespace terrain
} // namespace trm
