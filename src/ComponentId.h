#pragma once

#include <cstdlib>

namespace trm
{
	typedef size_t ComponentIdType;

	class ComponentIdGenerator;

	class ComponentId
	{
	public:
		bool operator < (const ComponentId & c) const;

	private:
		friend class ComponentIdGenerator;

		ComponentId(ComponentIdType id) : id_(id) {}

		ComponentIdType id_;
	};

	class ComponentIdGenerator
	{
	public:
		static ComponentId Generate();

	private:
		static ComponentIdType nextId_;
	};

} // namespace trm
