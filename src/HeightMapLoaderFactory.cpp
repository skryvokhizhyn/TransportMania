#include "HeightMapLoaderFactory.h"
#include "HeightMapLoaderTest.h"
#include "HeightMapLoaderFile.h"

using namespace trm::terrain;

HeightMapLoaderPtr
HeightMapLoaderFactory::GetTestLoader(const size_t width)
{
	return std::make_shared<terrain::HeightMapLoaderTest>(width);
}

#include "Logger.h"

HeightMapLoaderPtr 
HeightMapLoaderFactory::GetFileLoader(const std::string & path)
{
	utils::Logger().Debug() << "Loader started";
	return std::make_shared<terrain::HeightMapLoaderFile>(path);
}