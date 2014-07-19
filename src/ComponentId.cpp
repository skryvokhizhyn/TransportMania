#include "ComponentId.h"

using namespace trm;

bool
ComponentId::operator< (const ComponentId & c) const
{
	return id_ < c.id_;
}

ComponentIdType ComponentIdGenerator::nextId_ = 0;

ComponentId
ComponentIdGenerator::Generate()
{
	return {nextId_++};
}