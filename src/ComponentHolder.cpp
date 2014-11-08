#include "ComponentHolder.h"
#include "WorldProjection.h"
#include "RoadPoint.h"
#include "TrainPartParameters.h"
#include "DrawContext.h"

#include <boost/range/algorithm.hpp>
#include <boost/range/adaptor/reversed.hpp>

using namespace trm;

auto 
ComponentHolder::PutTrain(Train && train, RoadPoint rp) -> TrainIt
{
	const auto parts = train.Parts();

	float totalLength = 0.0f;

	Movables movables;
	movables.reserve(parts.size());

	boost::transform(parts | boost::adaptors::reversed, std::back_inserter(movables), 
		[&](const TrainPart & tp)
	{
		TrainMovableObject ret(rp, tp.type);

		const float partLength = TrainPartParameters::Get(tp.type).length;
		rp.Move(partLength);

		totalLength += partLength;

		return ret;
	});

	// putting to the front due to ease of iterator getting
	trains_.push_front(TrainData());
	auto & td = trains_.front();

	td.train = std::move(train);
	td.movables = std::move(movables);

	return trains_.begin();
}

void
ComponentHolder::Update(const WorldProjection & wp)
{
	for (auto && trainData : trains_)
	{
		boost::for_each(trainData.movables,
			boost::bind(&ComponentHolder::UpdateVisible, this, std::cref(wp), _1));
	}
	
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
ComponentHolder::Move(TrainIt it, float dist)
{
	Movables & m = it->movables;
	boost::for_each(m, 
		boost::bind(&TrainMovableObject::Move, _1, dist));		
}

void 
ComponentHolder::Remove(TrainIt it)
{
	trains_.erase(it);
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
