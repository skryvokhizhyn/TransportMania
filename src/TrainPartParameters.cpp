#include "TrainPartParameters.h"
#include <boost/assign.hpp>
#include <map>

using namespace trm;

namespace 
{
	typedef std::map<TrainPartType, TrainPartParameters::Data> TrainPartParametersStorage;

	const TrainPartParametersStorage TRAIN_PART_PARAMETERS_STORAGE = boost::assign::map_list_of
		(TrainPartType::Locomotive,	TrainPartParameters::Data(0.1f, 1.0f, 10.0f, 0.0f, 2.0f))
		(TrainPartType::Wagon,		TrainPartParameters::Data(0.0f, 3.0f, 0.0f, 100.0f, 2.0f));
}

TrainPartParameters::Data::Data(float a, float ms, float p, float v, float l)
	: acceleration(a)
	, maxSpeed(ms)
	, power(p)
	, volume(v)
	, length(l)
{}

auto
TrainPartParameters::Get(const TrainPartType tpt) -> const Data &
{
	const auto found = TRAIN_PART_PARAMETERS_STORAGE.find(tpt);
	
	if (found == TRAIN_PART_PARAMETERS_STORAGE.end())
	{
		throw std::runtime_error("No parameters for type specified");
	}

	return found->second;
}