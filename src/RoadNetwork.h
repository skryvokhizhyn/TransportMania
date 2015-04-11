#pragma once

#include <boost/noncopyable.hpp>

#include <memory>
#include <deque>

namespace trm
{
namespace network_impl
{
	template<typename T, typename U> class RoadNetworkImpl;

} // network_impl

	struct Point3i;

	class RoadNetwork
		: boost::noncopyable
	{
	public:
		using Route = std::deque<Point3i>;

	public:
		RoadNetwork();
		~RoadNetwork();

		bool Insert(const Point3i & from, const Point3i & to, float dist);
		Route GetRoute(const Point3i & from, const Point3i & to) const;
		
	private:
		typedef network_impl::RoadNetworkImpl<Point3i, float> RoadNetworkType;
		typedef std::unique_ptr<RoadNetworkType> ImplPtr;
		
	private:
		ImplPtr implPtr_;
	};

} // namespace trm
