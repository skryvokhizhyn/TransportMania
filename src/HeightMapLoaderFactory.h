#ifndef _HEIGHTMAPLOADERFACTORY_H_
#define _HEIGHTMAPLOADERFACTORY_H_

//#include "Point2d.h"
#include "HeightMapLoader.h"
#include <string>

namespace trm
{
namespace terrain
{
	struct HeightMapLoaderFactory
	{
		static HeightMapLoaderPtr GetTestLoader(const size_t width);
		static HeightMapLoaderPtr GetFileLoader(const std::string & path);
	};

} // namespace terrain
} // namespace trm

#endif // _HEIGHTMAPLOADERFACTORY_H_