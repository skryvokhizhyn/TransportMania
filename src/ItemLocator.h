#pragma once

#include "Polygon2d.h"
#include "UniqueId.h"

#include <memory>

namespace trm
{
	class ItemLocator
	{
	public:
		using Ids = std::vector<UniqueId>;

	public:
		ItemLocator();

		void Put(UniqueId id, const Polygon2d & p);
		void Remove(UniqueId id);

		Ids At(const Polygon2d & p) const;

	private:
		class ItemLocatorImpl;
		using ItemLocatorImplPtr = std::shared_ptr<ItemLocatorImpl>;

	private:
		ItemLocatorImplPtr implPtr_;
	};

} // namespace trm
