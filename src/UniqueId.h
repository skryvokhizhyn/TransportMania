#pragma once

#include <ostream>

namespace trm
{
	class UniqueId
	{
	public:
		using IdType = unsigned long;

	public:
		UniqueId();

		bool operator == (const UniqueId & other) const;
		bool operator < (const UniqueId & other) const;
		
		IdType Data() const;

	private:
		IdType id_;
	};

	std::ostream & operator << (std::ostream & o, const UniqueId & id);

} // namespace trm

template<>
struct std::hash<trm::UniqueId>
{
	std::size_t operator()(const trm::UniqueId & uid) const;
};
