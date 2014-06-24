#ifndef _TERRAINRANGE_H_
#define _TERRAINRANGE_H_

//#include "Point2d.h"
#include <boost/noncopyable.hpp>
#include <utility>
#include <vector>
#include <ostream>

namespace trm
{
	struct Point2d;

	class TerrainRange
	{
	public:
		struct Range
		{
			int y;
			int xBegin;
			int xEnd;

			Range();
			Range(const int a, const int b, const int c);
		};

		typedef std::vector<Range> Ranges;

	public:
		virtual ~TerrainRange();

		const Ranges & GetRanges() const;

	protected:
		void Init(const size_t sz);
		void PutRange(const Range & r);

	private:
		Ranges ranges_;
	};

	std::ostream & operator << (std::ostream & ostr, const trm::TerrainRange::Range & r);

} // namespace trm

#endif // _TERRAINRANGE_H_
