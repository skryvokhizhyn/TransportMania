#pragma once

#include <ostream>

namespace trm
{
	class UniqueId
	{
	public:
		using IdType = unsigned long;

	public:
		// creates initialized UniqueId
		static UniqueId Generate();

		bool operator == (const UniqueId & other) const;
		bool operator < (const UniqueId & other) const;
		
		IdType Data() const;

	private:
		UniqueId(IdType val);

	private:
		IdType id_;
	};

	std::ostream & operator << (std::ostream & o, const UniqueId & id);

} // namespace trm

namespace std
{
	template<>
	struct hash<trm::UniqueId>
	{
		std::size_t operator()(const trm::UniqueId & uid) const;
	};

}
