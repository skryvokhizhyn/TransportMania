#ifndef _HEIGHTMAPLOADERFILE_H_
#define _HEIGHTMAPLOADERFILE_H_

#include "HeightMapLoader.h"
#include "BmpHandler.h"
#include <boost/noncopyable.hpp>
#include <string>

namespace trm
{
namespace terrain
{
	class HeightMapLoaderFile
		: public HeightMapLoader
		, private boost::noncopyable
	{
	public:
		HeightMapLoaderFile(const std::string & path);

		// HeightMapLoader
		virtual size_t Size() const override;
		virtual void Get(const Point2d & pos, const size_t sz, HeightMap & hm) const override;
		virtual void Set(const Point2d & pos, const HeightMap & hm) override;

	private:
		utils::BmpHandler hmFile_;
	};

} // namespace terrain
} // namespace trm

#endif // _HEIGHTMAPLOADERFILE_H_