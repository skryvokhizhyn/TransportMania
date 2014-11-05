#include "HeightMapLoaderFactory.h"
#include "HeightMapLoaderTest.h"
#include "HeightMapLoaderFile.h"
#include "HeightMapLoaderCache.h"

using namespace trm::terrain;

HeightMapLoaderPtr
HeightMapLoaderFactory::GetTestLoader(const size_t width)
{
	return std::make_shared<HeightMapLoaderTest>(width);
}

HeightMapLoaderPtr 
HeightMapLoaderFactory::GetFileLoader(const std::string & path)
{
	return std::make_shared<HeightMapLoaderFile>(path);
}

HeightMapLoaderPtr 
HeightMapLoaderFactory::GetCachedLoader(const size_t patchSize, const HeightMapLoaderPtr & baseLoader)
{
	return std::make_shared<HeightMapLoaderCache>(patchSize, baseLoader);
}