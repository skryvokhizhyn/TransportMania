#include "HeightMapLoaderFactory.h"
#include "HeightMapLoaderTest.h"
#include "HeightMapLoaderFile.h"

using namespace trm::terrain;

HeightMapLoaderPtr
HeightMapLoaderFactory::GetTestLoader(const size_t width)
{
	return std::make_shared<terrain::HeightMapLoaderTest>(width);
}

HeightMapLoaderPtr 
HeightMapLoaderFactory::GetFileLoader(const std::string & path)
{
	return std::make_shared<terrain::HeightMapLoaderFile>(path);
}