#include "UniqueId.h"

using namespace trm;

UniqueId::UniqueId(IdType val)
	: id_(val)
{}

UniqueId
UniqueId::Generate()
{
	static IdType global_id = IdType();

	UniqueId id(global_id++);
	
	return id;
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

auto
UniqueId::Data() const -> IdType
{
	return id_;
}

std::ostream &
trm::operator << (std::ostream & o, const UniqueId & id)
{
	o << id.Data();
	return o;
}

//template<>
std::size_t std::hash<UniqueId>::operator()(const UniqueId & uid) const
{
	return std::hash<UniqueId::IdType>()(uid.Data());
}
