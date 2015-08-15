#pragma once

#include "Polygon2d.h"
#include "UniqueId.h"

#include <memory>
#include <unordered_map>

namespace trm
{
	class ItemLocator
	{
	public:
		using Ids = std::vector<UniqueId>;

	public:
		ItemLocator();
		~ItemLocator();

		void Put(UniqueId id, const Polygon2d & p);
		void Put(UniqueId id, Polygon2d && p);
		void Remove(UniqueId id);

		Ids At(const Polygon2d & p) const;

	private:
		class ItemLocatorImpl;
		using ItemLocatorImplPtr = std::unique_ptr<ItemLocatorImpl>;
		using PolygonMap = std::unordered_map<UniqueId, Polygon2d>;

	private:
		ItemLocatorImplPtr implPtr_;
		PolygonMap polyMap_;
	};

} // namespace trm
