#ifndef _POINTIMPL_H_
#define _POINTIMPL_H_

#pragma warning(push)
#pragma warning(disable: 4996)

#include "CommonUtils.h"
#include <boost/array.hpp>
#include <boost/static_assert.hpp>
#include <boost/bind.hpp>
#include <boost/operators.hpp>
#include <boost/integer/static_min_max.hpp>
#include <functional>
#include <numeric>

namespace trm
{
	template<typename T, unsigned short N>
	class PointImpl
		: boost::addable<PointImpl<T, N>
		, boost::subtractable<PointImpl<T, N>
		, boost::equality_comparable<PointImpl<T, N>
		, boost::less_than_comparable<PointImpl<T, N>
		, boost::dividable2<PointImpl<T, N>, T
		, boost::multipliable2<PointImpl<T, N>, T
		> > > > > >
	{
		typedef boost::array<T, N> ImplType;

	public:
		typedef T Type;
		typedef typename ImplType::iterator Iterator;
		typedef typename ImplType::const_iterator ConstIterator;

	public:
		PointImpl(const T a = T()) { std::fill(arr_.begin(), arr_.end(), a); }

		PointImpl(const T a, const T b)
		{
			BOOST_STATIC_ASSERT_MSG(N == 2, "Constructor available only for 2 dimentional implementation");
			Get<0>() = a; Get<1>() = b;
		}

		PointImpl(const T a, const T b, const T c)
		{
			BOOST_STATIC_ASSERT_MSG(N == 3, "Constructor available only for 3 dimentional implementation");
			Get<0>() = a; Get<1>() = b; Get<2>() = c;
		}

		PointImpl(const T a, const T b, const T c, const T d)
		{
			BOOST_STATIC_ASSERT_MSG(N == 4, "Constructor available only for 4 dimentional implementation");
			Get<0>() = a; Get<1>() = b; Get<2>() = c; Get<3>() = d;
		}
		
		template<typename U, unsigned short M>
		static PointImpl Cast(const PointImpl<U, M> & p)
		{
			PointImpl res;

			std::transform(p.Begin(), p.Begin() + boost::static_unsigned_min<M, N>::value, res.Begin(),
				[](const U val){return boost::numeric_cast<T>(val);});

			return res;
		}

		template<unsigned short K>
		T & Get()
		{
			BOOST_STATIC_ASSERT_MSG(K < N, "Index out of range");
			return arr_[K];
		}

		template<unsigned short K>
		const T Get() const
		{
			BOOST_STATIC_ASSERT_MSG(K < N, "Index out of range");
			return arr_[K];
		}

		const T operator [] (const size_t pos) const
		{
			assert(pos < N);
			return arr_[pos];
		}

		T & operator [] (const size_t pos)
		{
			assert(pos < N);
			return arr_[pos];
		}

		Iterator Begin() { return arr_.begin(); }
		ConstIterator Begin() const { return arr_.begin(); }
		Iterator End() { return arr_.end(); }
		ConstIterator End() const { return arr_.end(); }

		const PointImpl & operator += (const T v)
		{
			std::transform(arr_.begin(), arr_.end(), arr_.begin(), boost::bind(std::plus<T>(), _1, v));
			return *this;
		}
		const PointImpl & operator += (const PointImpl & p)
		{
			std::transform(arr_.begin(), arr_.end(), p.arr_.begin(), arr_.begin(), std::plus<T>());
			return *this;
		}

		const PointImpl & operator -= (const T v)
		{
			std::transform(arr_.begin(), arr_.end(), arr_.begin(), boost::bind(std::minus<T>(), _1, v));
			return *this;
		}
		const PointImpl & operator -= (const PointImpl & p)
		{
			std::transform(arr_.begin(), arr_.end(), p.arr_.begin(), arr_.begin(), std::minus<T>());
			return *this;
		}

		const PointImpl & operator *= (const T v)
		{
			std::transform(arr_.begin(), arr_.end(), arr_.begin(), boost::bind(std::multiplies<T>(), _1, v));
			return *this;
		}

		const PointImpl & operator *= (const PointImpl & p)
		{
			std::transform(arr_.begin(), arr_.end(), p.arr_.begin(), arr_.begin(), std::multiplies<T>());
			return *this;
		}

		const PointImpl & operator /= (const T v)
		{
			std::transform(arr_.begin(), arr_.end(), arr_.begin(), boost::bind(std::divides<T>(), _1, v));
			return *this;
		}

		bool operator == (const PointImpl & p) const
		{
			return std::equal(arr_.begin(), arr_.end(), p.arr_.begin(), static_cast<bool (*)(const T, const T)>(&utils::CheckEqual));
		}

		bool operator < (const PointImpl & p) const
		{
			return arr_ < p.arr_;
		}

		float GetLength() const
		{
			const float sumSqr = std::accumulate(arr_.begin(), arr_.end(), 0.0f, 
				boost::bind(std::plus<T>(), _1, 
					boost::bind(std::multiplies<T>(), _2, _2)));
			
			return ::sqrt(sumSqr);
		}

		void Normalize()
		{
			*this /= GetLength();
		}

		friend std::ostream & operator << (std::ostream & o, const PointImpl & p)
		{
			o << "(";
			if (!p.arr_.empty())
			{
				typename ImplType::const_iterator last = p.arr_.end();
				--last;
				std::copy(p.arr_.begin(), last, std::ostream_iterator<T>(o , ", "));

				o << *last;
			}
			o << ")";
			
			return o;
		}

	private:
		ImplType arr_;
	};

} // namespace trm

#pragma warning(pop)

#endif //_POINTIMPL_H_
