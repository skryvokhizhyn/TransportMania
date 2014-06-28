#ifndef _RAILROAD_H_
#define _RAILROAD_H_

#include "Types.h"
#include "RailRoadVisitor.h"
#include <boost/noncopyable.hpp>
#include <memory>

namespace trm
{
	struct RailRoad
		//: public DrawableObject
		: boost::noncopyable
	{
		virtual ~RailRoad() {}
		
		virtual void Accept(RailRoadVisitor & v) = 0;
		
	public:
		static const AxisType RAIL_ROAD_WIDTH;
		static const AxisType RAIL_ROAD_BORDER;
		static const AxisType RAIL_ROAD_STEP;

		static AxisType GetTotalWidth();
	};

	typedef std::shared_ptr<RailRoad> RailRoadPtr;

} // namespace trm

#endif // _RAILROAD_H_