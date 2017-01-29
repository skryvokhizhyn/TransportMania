//#define BOOST_TEST_DYN_LINK

#include <HeightMap.h>
#include <Variance.h>
#include <Point3d.h>
#include <GeometryUtils.h>
#include <boost/test/unit_test.hpp>

using namespace trm::terrain;
using namespace trm::terrain::lod;
using namespace trm;

namespace
{
	const HeightMap::Container v5x5 = 
	{
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		2, 0, 1, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0
	};
	const size_t Level5x5 = utils::GetPowerOf2(4) * 2;

	struct Test5x5HeightMap
	{
		HeightMap::Container sample5x5 = v5x5;
	};

	const HeightMap::Container v3x3 =
	{
		0, 0, 0, 
		0, 0, 0, 
		2, 0, 1
	};
	const size_t Level3x3 = utils::GetPowerOf2(2) * 2;

	struct Test3x3HeightMap
	{
		HeightMap::Container sample3x3 = v3x3;
	};
}

BOOST_FIXTURE_TEST_CASE(VarianceTest1, Test5x5HeightMap)
{
	HeightMap hm;
	hm.Swap(sample5x5);
	Variance v;
	v.Generate(Level5x5, hm, utils::GetTriangles(hm).first);

	BOOST_CHECK_EQUAL(2.0f, v.At(0));
	BOOST_CHECK_EQUAL(2.0f, v.At(1));
	BOOST_CHECK_EQUAL(0.5f, v.At(2));
	BOOST_CHECK_EQUAL(v.GetSize(), 15u);
}

BOOST_AUTO_TEST_CASE(VarianceTest2)
{
	const size_t sz = 33;
	std::vector<float> vec(sz * sz, 0);

	HeightMap hm;
	hm.Swap(vec);
	Variance v;
	v.Generate(utils::GetPowerOf2(sz - 1) * 2, hm, utils::GetTriangles(hm).first);

	BOOST_CHECK_EQUAL(v.GetSize(), 1023u);
}

BOOST_FIXTURE_TEST_CASE(VarianceForcedPointTest1, Test3x3HeightMap)
{
	HeightMap hm;
	hm.Swap(sample3x3);
	Variance v;
	v.PutForcedPoint(Size2d(0, 2));
	v.Generate(Level3x3, hm, utils::GetTriangles(hm).first);

	BOOST_CHECK_EQUAL(std::numeric_limits<HeightMap::Value>::max(), v.At(0));
}