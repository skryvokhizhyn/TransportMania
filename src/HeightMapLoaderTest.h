#ifndef _HEIGHTMAPLOADERTEST_H_
#define _HEIGHTMAPLOADERTEST_H_

#include "HeightMapLoader.h"
#include <boost/noncopyable.hpp>

namespace trm
{
namespace terrain
{
	class HeightMapLoaderTest
		: public HeightMapLoader
		, private boost::noncopyable
	{
	public:
		HeightMapLoaderTest(const size_t sz);

		virtual size_t getSize() const override;
		virtual void Get(const Point2d & pos, const size_t sz, HeightMap & hm) override;
		virtual void Set(const Point2d & pos, const HeightMap & hm) override;

	private:
		const size_t sz_;
	};

} // namespace terrain
} // namespace trm

#endif // _HEIGHTMAPLOADERTEST_H_