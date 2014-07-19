#pragma once

#include <boost/noncopyable.hpp>

namespace trm
{
	class TrainMovableObject;
	class WorldProjection;
	struct ComponentHolder;

	class TrainVisibilityHandler
		: boost::noncopyable
	{
	public:
		TrainVisibilityHandler(const WorldProjection & wp, ComponentHolder & ch);

		void operator () (TrainMovableObject & to) const;

	private:
		const WorldProjection & worldProjection_;
		ComponentHolder & componentHolder_;
	};

} // namespace trm
