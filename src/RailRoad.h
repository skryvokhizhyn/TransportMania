#ifndef _RAILROAD_H_
#define _RAILROAD_H_

#include "Types.h"
#include "RailRoadVisitor.h"

#include <boost/noncopyable.hpp>

#include <memory>
#include <vector>

namespace trm
{
	struct RailRoad
		//: public DrawableObject
		: boost::noncopyable
	{
		virtual ~RailRoad() {}
		
		virtual void Accept(RailRoadVisitor & v) = 0;
		
	public:
		static AxisType GetTotalWidth();
		static AxisType GetRoadWidth();
		static AxisType GetRoadStep();
		static AxisType GetRoadZShift();
	};

	typedef std::shared_ptr<RailRoad> RailRoadPtr;
	typedef std::vector<RailRoadPtr> RailRoadPtrs;

} // namespace trm

#endif // _RAILROAD_H_