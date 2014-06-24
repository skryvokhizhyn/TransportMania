#pragma once

#include "RailRoad.h"
#include "SymmetricMap.h"
#include "RoadRoute.h"
#include <boost/noncopyable.hpp>
#include <memory>
#include <map>

namespace trm
{
namespace network_impl
{
	template<typename T, typename U> class RoadNetworkImpl;

} // network_impl

	class RoadNetwork
		: boost::noncopyable
	{
	public:
		RoadNetwork();

		bool Insert(const RailRoadPtr & p);
		RoadRoutePtr GetRoute(const Point3d & from, const Point3d & to) const;
		
	private:
		typedef network_impl::RoadNetworkImpl<size_t, float> RoadNetworkType;
		typedef std::shared_ptr<RoadNetworkType> ImplPtr;
		typedef SymmetricMap<size_t, RailRoadPtr> RoadsType;
		
	private:
		ImplPtr implPtr_;
		RoadsType roads_;
	};

} // namespace trm
