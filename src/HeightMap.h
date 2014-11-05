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
		typedef float Value;
		typedef std::vector<Type> Container;

	public:
		HeightMap();
		//HeightMap(const HeightMap & h);
		HeightMap(HeightMap && h);
		virtual ~HeightMap();

		void Swap(Container & c);

		bool IsLoaded() const;

		const Container & GetData() const;

		void Clear();

		// HeightMapBase
		virtual size_t GetSize() const;
		virtual Type At(const Point2d & p) const;
		virtual void Set(const Point2d & p, const Type z);

	public:
		static size_t GetSideSize(const size_t); // throws if it's not square-like
		static Value GetValueAt(const Point2d & p, const Container & c, const size_t s);

	private:
		Container height_;
		size_t sz_;
	};

	// makes reversed initialization of height map
	HeightMap::Container PrepareDataFromVectorReversed(const HeightMap::Value * arr, const size_t n);
	
} // terrain
} // trm

#endif // _HEIGHTMAP_H_
