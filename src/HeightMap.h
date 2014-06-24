#ifndef _HEIGHTMAP_H_
#define _HEIGHTMAP_H_

#include "Types.h"
#include "Triangle3d.h"
#include "HeightMapBase.h"
#include <boost/noncopyable.hpp>
#include <boost/range/begin.hpp>
#include <boost/range/end.hpp>
#include <vector>

namespace trm
{
namespace terrain
{
	class HeightMap
		: boost::noncopyable
		, public HeightMapBase
	{
	public:
		typedef float Type;
		typedef std::vector<Type> HeightMapType;

	public:
		HeightMap();
		virtual ~HeightMap();

		void Swap(HeightMapType & c);

		template<typename T, size_t N>
		void Load(T (&arr)[N]);
		template<typename T>
		void Load(T * arr, const size_t n);

		bool IsLoaded() const;

		const HeightMapType & GetData() const;

		void Clear();

		// HeightMapBase
		virtual size_t GetSize() const;
		virtual Type At(const Point2d & p) const;
		virtual void Set(const Point2d & p, const Type z);

	public:
		static size_t GetSideSize(const size_t); // throws if it's not square-like

	private:
		HeightMapType height_;
		size_t sz_;
	};

	template<typename T, size_t N>
	void HeightMap::Load(T (&arr)[N])
	{
		Load(arr, N);
	}

	template<typename T>
	void HeightMap::Load(T * arr, const size_t n)
	{
		sz_ = GetSideSize(n);

		height_.resize(n);

		size_t i = 0;
		size_t j = sz_;

		while (j > 0)
		{
			height_[(j - 1) * sz_ + i] = arr[(sz_ - j) * sz_ + i];

			if (++i == sz_)
			{
				--j;
				i = 0;
			}
		}
	}
} // terrain
} // trm

#endif // _HEIGHTMAP_H_
