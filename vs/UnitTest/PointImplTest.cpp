//#define BOOST_TEST_DYN_LINK

#include <PointImpl.h>

#include <boost/test/unit_test.hpp>

using namespace trm;

BOOST_AUTO_TEST_CASE(PointImplTest1)
{
	/*PointImpl<float, 2> p1(1.0f, 2.0f);
	BOOST_CHECK_EQUAL(p1.x, 1.0f);
	BOOST_CHECK_EQUAL(p1.y, 2.0f);

	PointImpl<float, 2> p2;
	BOOST_CHECK_EQUAL(p2.x, 0.0f);
	BOOST_CHECK_EQUAL(p2.y, 0.0f);

	p2.x = 1.0f;
	p2.y = 2.0f;
	BOOST_CHECK_EQUAL(p2.x, 1.0f);
	BOOST_CHECK_EQUAL(p2.y, 2.0f);

	const PointImpl<float, 2> p3 = p2;
	BOOST_CHECK_EQUAL(p3.x, 1.0f);
	BOOST_CHECK_EQUAL(p3.y, 2.0f);

	const PointImpl<float, 2> p4(p2);
	BOOST_CHECK_EQUAL(p3.x, 1.0f);
	BOOST_CHECK_EQUAL(p3.y, 2.0f);*/
}

BOOST_AUTO_TEST_CASE(PointImplTest2)
{
	/*PointImpl<float, 2> p1(1.0f, 2.0f);

	p1 += 1.0f;

	BOOST_CHECK_EQUAL(p1.x, 2.0f);
	BOOST_CHECK_EQUAL(p1.y, 3.0f);

	p1 -= 2.0f;
	BOOST_CHECK_EQUAL(p1.x, 0.0f);
	BOOST_CHECK_EQUAL(p1.y, 1.0f);

	p1 *= 4.0f;
	BOOST_CHECK_EQUAL(p1.x, 0.0f);
	BOOST_CHECK_EQUAL(p1.y, 4.0f);

	p1 /= 2.0f;
	BOOST_CHECK_EQUAL(p1.x, 0.0f);
	BOOST_CHECK_EQUAL(p1.y, 2.0f);*/
}

BOOST_AUTO_TEST_CASE(PointImplTest3)
{
	/*PointImpl<float, 2> p1(1.0f, 2.0f);
	PointImpl<float, 2> p2(2.0f, 3.0f);
	PointImpl<float, 2> p3(7.0f, 7.0f);

	p1 += p2;

	BOOST_CHECK_EQUAL(p1.x, 3.0f);
	BOOST_CHECK_EQUAL(p1.y, 5.0f);

	p1 -= p3;

	BOOST_CHECK_EQUAL(p1.x, -4.0f);
	BOOST_CHECK_EQUAL(p1.y, -2.0f);*/
}

BOOST_AUTO_TEST_CASE(PointImplTest4)
{
	/*PointImpl<float, 2> p1(1.0f, 2.0f);
	PointImpl<float, 2> p2(2.0f, 3.0f);
	PointImpl<float, 2> p3(7.0f, 7.0f);

	PointImpl<float, 2> p4 = p1 + p2;

	BOOST_CHECK_EQUAL(p4.x, 3.0f);
	BOOST_CHECK_EQUAL(p4.y, 5.0f);

	PointImpl<float, 2> p5 = p1 - p3;

	BOOST_CHECK_EQUAL(p5.x, -6.0f);
	BOOST_CHECK_EQUAL(p5.y, -5.0f);*/
}

BOOST_AUTO_TEST_CASE(PointImplTest5)
{
	/*PointImpl<float, 2> p1(1.0f, 2.0f);

	PointImpl<float, 2> p4 = p1 + 1.0f;

	BOOST_CHECK_EQUAL(p4.x, 2.0f);
	BOOST_CHECK_EQUAL(p4.y, 3.0f);

	PointImpl<float, 2> p5 = p1 - 2.0f;

	BOOST_CHECK_EQUAL(p5.x, -1.0f);
	BOOST_CHECK_EQUAL(p5.y, 0.0f);*/
}

BOOST_AUTO_TEST_CASE(PointImplTest6)
{
	//PointImpl<float, 3> p1(1.0f, 2.0f, 3.0f);
	//PointImpl<float, 3> p2(2.0f, 3.0f, 4.0f);
	//PointImpl<float, 3> p3(7.0f, 7.0f, 7.0f);

	//PointImpl<float, 3> p4 = p1 + p2;

	//BOOST_CHECK_EQUAL(p4.x, 3.0f);
	//BOOST_CHECK_EQUAL(p4.y, 5.0f);
	//BOOST_CHECK_EQUAL(p4.z, 7.0f);

	//PointImpl<float, 3> p5 = p1 - p3;

	//BOOST_CHECK_EQUAL(p5.x, -6.0f);
	//BOOST_CHECK_EQUAL(p5.y, -5.0f);
	//BOOST_CHECK_EQUAL(p5.z, -4.0f);
}

#include <boost/date_time/posix_time/posix_time.hpp>
#include <Point3d.h>

//BOOST_AUTO_TEST_CASE(PointImplTest7)
//{
//	const size_t cnt = 10000000;
//
//	PointImpl<float, 3> p;
//
//	boost::posix_time::ptime p1 = boost::posix_time::microsec_clock::local_time();
//
//	for ( size_t i = 0; i < cnt ; ++i )
//	{
//		p += PointImpl<float, 3>(1.1f, 1.1f, 1.1f);
//	}
//
//	boost::posix_time::ptime p2 = boost::posix_time::microsec_clock::local_time();
//
//	Point3d v;
//
//	boost::posix_time::ptime p3 = boost::posix_time::microsec_clock::local_time();
//
//	for ( size_t i = 0; i < cnt ; ++i )
//	{
//		v += Point3d(1.1f, 1.1f, 1.1f);
//	}
//
//	boost::posix_time::ptime p4 = boost::posix_time::microsec_clock::local_time();
//
//	std::cout << (p2 - p1).total_microseconds() << "\t\t" << (p4 - p3).total_microseconds() << std::endl;
//
//	BOOST_CHECK_EQUAL(p.Get<0>(), v.x);
//	BOOST_CHECK_EQUAL(p.Get<1>(), v.y);
//	BOOST_CHECK_EQUAL(p.Get<2>(), v.z);
//}

BOOST_AUTO_TEST_CASE(PointImplTest8)
{
	PointImpl<float, 2> p1;

	BOOST_CHECK_EQUAL(p1.Get<0>(), 0);
	BOOST_CHECK_EQUAL(p1.Get<1>(), 0);

	PointImpl<float, 2> p2(1.0f, 2.0f);

	BOOST_CHECK_EQUAL(p2.Get<0>(), 1.0f);
	BOOST_CHECK_EQUAL(p2.Get<1>(), 2.0f);
}

BOOST_AUTO_TEST_CASE(PointImplTest9)
{
	PointImpl<float, 2> p1(1.0f, 2.0f);

	p1 += 1.0f;

	BOOST_CHECK_EQUAL(p1.Get<0>(), 2.0f);
	BOOST_CHECK_EQUAL(p1.Get<1>(), 3.0f);

	PointImpl<float, 2> p2(2.0f, 3.0f);

	p1 += p2;

	BOOST_CHECK_EQUAL(p1.Get<0>(), 4.0f);
	BOOST_CHECK_EQUAL(p1.Get<1>(), 6.0f);

	PointImpl<float, 2> p3(1.0f, 1.0f);

	p1 -= p3;

	BOOST_CHECK_EQUAL(p1.Get<0>(), 3.0f);
	BOOST_CHECK_EQUAL(p1.Get<1>(), 5.0f);

	p1 -= 1.0f;

	BOOST_CHECK_EQUAL(p1.Get<0>(), 2.0f);
	BOOST_CHECK_EQUAL(p1.Get<1>(), 4.0f);

	p1 *= 3.0f;

	BOOST_CHECK_EQUAL(p1.Get<0>(), 6.0f);
	BOOST_CHECK_EQUAL(p1.Get<1>(), 12.0f);

	p1 /= 2.0f;

	BOOST_CHECK_EQUAL(p1.Get<0>(), 3.0f);
	BOOST_CHECK_EQUAL(p1.Get<1>(), 6.0f);
}

BOOST_AUTO_TEST_CASE(PointImplTest10)
{
	PointImpl<float, 2> p1;
	PointImpl<float, 2> p2(1.0f, 2.0f);

	BOOST_CHECK_LE(p1, p2);
}

BOOST_AUTO_TEST_CASE(PointImplTest11)
{
	PointImpl<float, 2> p1(2.0f, 3.0f);
	PointImpl<float, 2> p2(1.0f, 2.0f);

	PointImpl<float, 2> p3 = p1 + p2;

	BOOST_CHECK_EQUAL(p3, (PointImpl<float, 2>(3.0f, 5.0f)));
}

BOOST_AUTO_TEST_CASE(PointImplTest12)
{
	PointImpl<float, 2> p1(2.0f, 3.0f);
	typedef PointImpl<size_t, 3> NewType;
	
	NewType p2 = NewType::Cast(p1);

	BOOST_CHECK_EQUAL(p2, (NewType(2, 3, 0)));
}

BOOST_AUTO_TEST_CASE(PointImplTest13)
{
	typedef PointImpl<size_t, 3> NewType;
	
	NewType p2 = NewType::Cast(NewType());

	BOOST_CHECK_EQUAL(p2, NewType());
}

BOOST_AUTO_TEST_CASE(PointImplTest14)
{
	typedef PointImpl<float, 2> NewType;
	
	NewType p1(0, 0);
	NewType p2(0, 4);

	BOOST_CHECK_NE(p1, p2);
	BOOST_CHECK_NE(p2, p1);
}

#include <Triangle2d.h>

BOOST_AUTO_TEST_CASE(PointImplTest15)
{
	trm::Triangle2d t1 = Triangle2d(Point2d(), Point2d(), Point2d());
	trm::Triangle2d t2 = Triangle2d(Point2d(0, 4), Point2d(0, 0), Point2d(4, 4));

	BOOST_CHECK_NE(t1, t2);
	BOOST_CHECK_NE(t2, t1);
}
