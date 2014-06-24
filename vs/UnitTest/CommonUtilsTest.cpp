//#define BOOST_TEST_DYN_LINK

#include <CommonUtils.h>

#include <boost/test/unit_test.hpp>

using namespace utils;

BOOST_AUTO_TEST_CASE(CommonUtilsTest1)
{
	float v1 = 1.0;

	BOOST_CHECK_EQUAL(CheckEqual(v1, v1 + std::numeric_limits<float>::epsilon()), false);
}

BOOST_AUTO_TEST_CASE(CommonUtilsTest2)
{
	float v1 = 1.0;

	BOOST_CHECK_EQUAL(CheckEqual(v1, v1 + std::numeric_limits<float>::epsilon() * 2), false);
}

BOOST_AUTO_TEST_CASE(CommonUtilsTest3)
{
	float v1 = 1.0;

	BOOST_CHECK_EQUAL(CheckEqual(v1, v1 + std::numeric_limits<float>::epsilon() / 2), true);
}

BOOST_AUTO_TEST_CASE(CommonUtilsTestPowerOf21)
{
	BOOST_CHECK_EQUAL(GetPowerOf2<true>(1), 0u);
	BOOST_CHECK_EQUAL(GetPowerOf2<true>(2), 1u);
	BOOST_CHECK_EQUAL(GetPowerOf2<true>(8), 3u);
}

BOOST_AUTO_TEST_CASE(CommonUtilsTestPowerOf22)
{
	BOOST_CHECK_EQUAL(GetPowerOf2<false>(9), 3u);
}

BOOST_AUTO_TEST_CASE(CommonUtilsTestPowerOf23)
{
	BOOST_CHECK_THROW(GetPowerOf2<true>(9), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(CommonUtilsTestPowerOf231)
{
	BOOST_CHECK_THROW(GetPowerOf2(9), std::runtime_error);
} 

BOOST_AUTO_TEST_CASE(CommonUtilsTestPowerOf24)
{
	BOOST_CHECK_NO_THROW(GetPowerOf2<false>(9));
}

BOOST_AUTO_TEST_CASE(CommonUtilsCheckEqual1Test1)
{
	BOOST_CHECK(CheckEqual(1, 1));
}

BOOST_AUTO_TEST_CASE(CommonUtilsCheckNearTest1)
{
	BOOST_CHECK(CheckNear(1.0, 1.0, 1.0));
}

BOOST_AUTO_TEST_CASE(CommonUtilsCheckNearTest2)
{
	BOOST_CHECK(!CheckNear(1.0, 1.0, 0.0));
}

BOOST_AUTO_TEST_CASE(CommonUtilsCheckNearTest3)
{
	BOOST_CHECK(!CheckNear(1.0, 1.0 + std::numeric_limits<double>::epsilon(), std::numeric_limits<double>::epsilon()));
}

BOOST_AUTO_TEST_CASE(CommonUtilsCheckNearTest4)
{
	BOOST_CHECK(CheckNear(1.0, 1.0 + std::numeric_limits<double>::epsilon(), 2 * std::numeric_limits<double>::epsilon()));
}