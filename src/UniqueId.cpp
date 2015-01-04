#include "UniqueId.h"

using namespace trm;

UniqueId::UniqueId()
	: id_(0)
{
	static IdType global_id = IdType();
	id_ = global_id++;
}

bool
UniqueId::operator < (const UniqueId & other) const
{
	return id_ < other.id_;
}

bool
UniqueId::operator == (const UniqueId & other) const
{
	return id_ == other.id_;
}