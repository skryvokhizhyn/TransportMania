#include "HeightMapLoaderTest.h"
#include "TerrainRangeCircle.h"
#include "Terraformer.h"
#include "HeightMap.h"
#include "Point2d.h"

using namespace trm;
using namespace trm::terrain;

namespace
{
	void PrepareHeightMap256(HeightMap & hm)
	{
		TerrainRangeCircle circle(trm::Point2d(32, 32), 10);

		auto tf = TerraformFunctionFactory::GetSpherical(trm::Point2d(32, 32), 10, 5);
		Terraformer t(circle, *tf.get());
		t.Apply(hm);
	}

	void PrepareHeightMap8(HeightMap & hm)
	{
		TerrainRangeCircle circle(trm::Point2d(4, 4), 4);

		auto tf = TerraformFunctionFactory::GetSpherical(trm::Point2d(4, 4), 3, 5);
		Terraformer t(circle, *tf.get());
		t.Apply(hm);
	}
}

HeightMapLoaderTest::HeightMapLoaderTest(const size_t sz)
	: sz_(sz)
{}

size_t 
HeightMapLoaderTest::Size() const
{
	return sz_;
}

void
HeightMapLoaderTest::Get(const Point2d & /*pos*/, const size_t sz, HeightMap & hm) const
{
	std::vector<HeightMap::Value> terrainHeight(sz * sz, 0.0f);

	//hm.Load(&terrainHeight[0], terrainHeight.size());
	hm.Swap(terrainHeight);

	if (sz <= 9)
	{
		PrepareHeightMap8(hm);
	}
	else
	{
		PrepareHeightMap256(hm);
	}
}

void
HeightMapLoaderTest::Set(const Point2d & /*pos*/, const HeightMap & /*hm*/)
{
	throw std::runtime_error("Not implemented");
}