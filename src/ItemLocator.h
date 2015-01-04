#pragma once

#include "Polygon2d.h"

#include <memory>

namespace trm
{
	class ItemLocator
	{
	public:
		using Ids = std::vector<int>;

	public:
		ItemLocator();

		void Put(const int id, const Polygon2d & p);
		void Remove(const int id);

		Ids At(const Polygon2d & p) const;

	private:
		class ItemLocatorImpl;
		using ItemLocatorImplPtr = std::shared_ptr<ItemLocatorImpl>;

	private:
		ItemLocatorImplPtr implPtr_;
	};

} // namespace trm
