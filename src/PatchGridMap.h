#pragma once

#include "Size2d.h"
#include "HeightMap.h"
#include "Patch.h"
#include "ModelData.h"

#include <boost/multi_index_container.hpp>
#include <boost/multi_index/ordered_index.hpp>
#include <boost/multi_index/member.hpp>
#include <boost/multi_index/sequenced_index.hpp>
#include <boost/multi_index/indexed_by.hpp>

namespace trm
{
namespace terrain
{
	class HeightMap;

namespace lod
{

namespace PatchCorner
{
	// values of this enume are indexes in vector
	const int LeftBottom = 0;
	const int LeftUp = 1;
	const int RightUp = 2;
	const int RightBottom = 3;
};

	using CornerValues = std::vector<float>;

	struct PatchGridNode
	{
		Size2d pos;

		struct Data
		{
			HeightMap heightMap;
			Patch patch;
			CornerValues heights;
			ModelData renderCache;
			bool dirty;
			bool valid;

			Data() : dirty(false), valid(false) {}
		};
		
		mutable Data data;

		PatchGridNode(const Size2d & p) : pos(p) {}
	};

namespace impl
{
	namespace bmi = boost::multi_index;

	struct PosTag {};
	struct SequentialTag {};

	using PatchGridMapImpl =
		boost::multi_index_container<
			PatchGridNode,
			bmi::indexed_by<
				bmi::ordered_unique<bmi::tag<PosTag>, bmi::member<PatchGridNode, Size2d, &PatchGridNode::pos>>,
				bmi::sequenced<bmi::tag<SequentialTag>>
			>>;

} // namespace impl

	class PatchGridMap
		: boost::noncopyable
	{
	public:
		using UniqueIt = impl::PatchGridMapImpl::index<impl::PosTag>::type::iterator;
		using SequentialIt = impl::PatchGridMapImpl::index<impl::SequentialTag>::type::iterator;

	public:
		PatchGridMap() = default;

		PatchGridMap(PatchGridMap && pgm)
			: patchGridImpl_(std::move(pgm.patchGridImpl_))
		{}

		UniqueIt Init(const Size2d & pos)
		{
			auto res = patchGridImpl_.get<impl::PosTag>().emplace(pos);
			if (!res.second)
			{
				std::stringstream ss;
				ss << "Patch " << pos << " already in grid";
				throw std::runtime_error(ss.str());
			}

			return res.first;
		}

		UniqueIt Find(const Size2d & pos) const
		{
			const auto & posMap = patchGridImpl_.get<impl::PosTag>();
			auto found = posMap.find(pos);

			if (found == posMap.end())
			{
				std::stringstream ss;
				ss << "Point " << pos << " is out of grid";
				throw std::runtime_error(ss.str());
			}

			return found;
		}

		SequentialIt Begin() const
		{
			return patchGridImpl_.get<impl::SequentialTag>().begin();
		}

		SequentialIt End() const
		{
			return patchGridImpl_.get<impl::SequentialTag>().end();
		}

		void MoveToBegin(const SequentialIt & it)
		{
			patchGridImpl_.get<impl::SequentialTag>().relocate(Begin(), it);
		}

		void MoveToEnd(const SequentialIt & it)
		{
			patchGridImpl_.get<impl::SequentialTag>().relocate(End(), it);
		}

	private:
		impl::PatchGridMapImpl patchGridImpl_;
	};

} // namespace lod
} // namespace terrain
} // namespace trm