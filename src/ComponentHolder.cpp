#include "ComponentHolder.h"
#include "WorldProjection.h"
#include "RoadPoint.h"
#include "TrainPartParameters.h"
#include "DrawContext.h"

#include <boost/range/algorithm.hpp>
#include <boost/range/adaptor/map.hpp>

using namespace trm;

void 
ComponentHolder::PutTrain(ComponentId id, Train && train, RoadPoint rp)
{
	const auto parts = train.Parts();

	float totalLength = 0.0f;

	std::for_each(parts.crbegin(), parts.crend(),
		[&](const TrainPart & tp)
	{
		movables_.insert(std::make_pair(id, TrainMovableObject(rp, tp.type)));

		const float partLength = TrainPartParameters::Get(tp.type).length;
		rp.Move(partLength);

		totalLength += partLength;
	});

	trains_.emplace(id, std::move(train));
}

void
ComponentHolder::Update(const WorldProjection & wp)
{
	boost::for_each(movables_ | boost::adaptors::map_values,
		boost::bind(&ComponentHolder::UpdateVisible, this, std::cref(wp), _1));
	
	auto it = boost::remove_if(visibles_,
		!boost::bind(&TrainVisibleObject::Update, _1));

	visibles_.erase(it, visibles_.end());
}

void 
ComponentHolder::UpdateVisible(const WorldProjection & wp, TrainMovableObject & to)
{
	const bool isVisible = wp.IsPointVisible(to.Position());
		
	if (to.GetVisible() == isVisible)
	{
		return;
	}

	// must be before visibles creating
	// visible takes shared state which is initialized when isVisible put as true
	to.SetVisible(isVisible);

	if (isVisible)
	{
		auto sharedPosition = to.SharedPosition();
		const Point3d nextPosition = to.MovedPosition(0.1f);

		visibles_.emplace_back(to.Type(), sharedPosition, nextPosition);
	}
}

void 
ComponentHolder::Move(ComponentId id, float dist)
{
	auto mov = movables_.equal_range(id);
	std::for_each(mov.first, mov.second, 
		std::bind(&TrainMovableObject::Move, 		
			std::bind(&Movables::value_type::second, std::placeholders::_1), dist));
}

void 
ComponentHolder::Remove(ComponentId id)
{
	trains_.erase(id);
	movables_.erase(id);
}

void 
ComponentHolder::Draw(const DrawContext & c, const Matrix & projectionViewMatrix) const
{
	boost::for_each(visibles_,
		[&](const TrainVisibleObject & tvo)
	{
		c.Transform(projectionViewMatrix, tvo.GetMatrix());
		tvo.Draw();
	});
}
