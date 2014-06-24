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
	const HeightMap::Type v5x5[] = 
	{
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0,
		2, 0, 1, 0, 0,
		0, 0, 0, 0, 0,
		0, 0, 0, 0, 0
	};
	const size_t Level5x5 = utils::GetPowerOf2(4) * 2;
}

BOOST_AUTO_TEST_CASE(VarianceTest1)
{
	HeightMap hm;
	hm.Load(v5x5);
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
	hm.Load(&vec[0], vec.size());
	Variance v;
	v.Generate(utils::GetPowerOf2(sz - 1) * 2, hm, utils::GetTriangles(hm).first);

	BOOST_CHECK_EQUAL(v.GetSize(), 1023u);
}