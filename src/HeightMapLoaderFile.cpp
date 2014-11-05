#include "HeightMapLoaderFile.h"
#include "CommonUtils.h"
#include "BmpHandler.h"
#include "HeightMap.h"
#include "Point2d.h"
#include "Terrain.h"
#include <boost/format.hpp>
#include <boost/numeric/conversion/cast.hpp>
#include <algorithm>

using namespace trm;
using namespace trm::terrain;

namespace {
//const float CONVERSION_BMP_2_HEIGHT_RATIO = 40.0f / 256;
//const float WATER_HEIGHT_SHIFT = 5.0f;
const float MAX_BMP_VARIANCE = 256.0f;
}

HeightMapLoaderFile::HeightMapLoaderFile(const std::string & path) :
		hmFile_(path) {
	if (hmFile_.GetWidth() != hmFile_.GetHeight()) {
		throw std::runtime_error(
				(boost::format("Only squared images allowed. Size is %d x %d")
						% hmFile_.GetWidth() % hmFile_.GetHeight()).str());
	}
}

size_t HeightMapLoaderFile::Size() const 
{
	return hmFile_.GetWidth();
}

void HeightMapLoaderFile::Get(const Point2d & pos, const size_t sz, HeightMap & hm) const
{
	const size_t cnt = sz * sz;

	utils::BmpHandler::ValueBuffer buff(cnt, 0);

	size_t x = boost::numeric_cast<size_t>(pos.x());
	size_t y = boost::numeric_cast<size_t>(pos.y());

	for (size_t j = y; j < y + sz; ++j) {
		hmFile_.Get(x, j, &buff[(j - y) * sz], sz);
	}

	HeightMap::Container hmBuff(cnt, 0);

	const float heightVariance = Terrain::MAX_HEIGHT - Terrain::MIN_HEIGHT + 1;
	const float conversionRatio = heightVariance / MAX_BMP_VARIANCE;

	std::transform(buff.begin(), buff.end(), hmBuff.begin(),
			[&](const utils::BmpHandler::ValueType val) {return conversionRatio * val + Terrain::MIN_HEIGHT;});

	hm.Swap(hmBuff);
}

void HeightMapLoaderFile::Set(const Point2d & pos, const HeightMap & hm) 
{
	const HeightMap::Container & data = hm.GetData();

	size_t x = boost::numeric_cast<size_t>(pos.x());
	size_t y = boost::numeric_cast<size_t>(pos.y());

	// row size is sz + 1
	const size_t cnt = hm.GetSize();

	utils::BmpHandler::ValueBuffer buff(data.size(), 0);

	const float heightVariance = Terrain::MAX_HEIGHT - Terrain::MIN_HEIGHT + 1;
	const float conversionRatio = heightVariance / MAX_BMP_VARIANCE;

	std::transform(data.begin(), data.end(), buff.begin(),
			[&](const HeightMap::Value val) {return (val - Terrain::MIN_HEIGHT) / conversionRatio;});

	for (size_t j = 0; j < cnt; ++j) {
		hmFile_.Put(x, y + j, &buff[j * cnt], cnt);
	}
}
