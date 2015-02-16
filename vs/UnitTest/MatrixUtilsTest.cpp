#include <MatrixUtils.h>
#include <MatrixFactory.h>

#include <boost/test/unit_test.hpp>

#include <boost/numeric/ublas/io.hpp>
#include <boost/iterator/zip_iterator.hpp>

using namespace utils;
using namespace trm;

namespace boost
{
	bool operator == (const Matrix & m1, const Matrix & m2)
	{
		auto it1Begin = m1.begin1();
		auto it1 = boost::make_zip_iterator(boost::make_tuple(m1.cbegin1(), m2.cbegin1()));
		auto end1 = boost::make_zip_iterator(boost::make_tuple(m1.cend1(), m2.cend1()));

		for (; it1 != end1; ++it1)
		{
			auto itTuple = it1.get_iterator_tuple();
			auto it2 = boost::make_zip_iterator(boost::make_tuple(
				itTuple.get<0>().cbegin(),
				itTuple.get<1>().cbegin()));

			auto end2 = boost::make_zip_iterator(boost::make_tuple(
				itTuple.get<0>().cend(),
				itTuple.get<1>().cend()));

			for (; it2 != end2; ++it2)
			{
				if (it2->get<0>() != it2->get<1>())
				{
					return false;
				}
			}
		}

		return true;
	}
}

BOOST_AUTO_TEST_CASE(MatrixUtilsEqualTest1)
{
	BOOST_CHECK_EQUAL(MatrixFactory::Identity(), MatrixFactory::Identity());
}

BOOST_AUTO_TEST_CASE(MatrixUtilsEqualTest2)
{
	BOOST_CHECK_NE(MatrixFactory::Identity(), MatrixFactory::Zero());
}

BOOST_AUTO_TEST_CASE(MatrixUtilsInverseTest1)
{
	Matrix m = MatrixFactory::Identity();
	Matrix inversed = Inverse(m);

	BOOST_CHECK_EQUAL(m, inversed);
}

BOOST_AUTO_TEST_CASE(MatrixUtilsInverseTest2)
{
	BOOST_REQUIRE_THROW(Inverse(MatrixFactory::Zero()), std::runtime_error);
}

BOOST_AUTO_TEST_CASE(MatrixUtilsInverseTest3)
{
	Matrix m = MatrixFactory::Identity();
	m.at_element(0, 1) = 0.5f;
	m.at_element(1, 0) = 0.5f;

	const Matrix inversed = Inverse(m);

	const Matrix p = m * inversed;

	BOOST_CHECK_EQUAL(p, MatrixFactory::Identity());
}

BOOST_AUTO_TEST_CASE(MatrixUtilsInverseTest4)
{
	Matrix m = MatrixFactory::Identity();
	m.at_element(0, 1) = 0.5f;
	m.at_element(1, 0) = 0.5f;

	Point4d p(1.0f, 2.0f, 3.0f, 4.0f);

	const Point4d p1 = m * p;

	Matrix inversed = Inverse(m);

	const Point4d p2 = inversed * p1;

	BOOST_CHECK_EQUAL(p, p2);
}
