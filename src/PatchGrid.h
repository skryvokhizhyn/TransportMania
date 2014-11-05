#pragma once

#include "HeightMapLoader.h"
#include "PatchGridMap.h"

#include <boost/noncopyable.hpp>

namespace trm
{
	struct ModelData;
	class WorldProjection;

namespace terrain
{
	class HeightMap;

namespace lod
{
	class Patch;

	class PatchGrid
		: public HeightMapBase
		, boost::noncopyable
	{
	public:
		PatchGrid(const unsigned short patchSize, const unsigned short patchCnt, const HeightMapLoaderPtr & hmlPtr);
		PatchGrid(PatchGrid && pg);
		void Init();

		void Update(const WorldProjection & wp);
		bool Tasselate(const WorldProjection & wp);
		bool Render(ModelData & md);
		void Flush();

		// HeightMapBase
		virtual size_t GetSize() const;
		virtual HeightMap::Value At(const Point2d & p) const;
		virtual void Set(const Point2d & p, const Type z);

	public:
		typedef std::vector<Size2d> Positions;

	public:
		Positions GetAdjucentPatches(const Point2d & p) const;

	private:
		static void RenderNode(const PatchGridNode & node, ModelData & md);

	private:
		void LoadHeightMap(const Size2d & pos, HeightMap & hm) const;
		void GlueNormales(const PatchGridNode & node, ModelData & md) const;

	private:
		const unsigned short patchSize_;
		const unsigned short patchCount_;
		PatchGridMap grid_;
		PatchGridMap::SequentialIt currIt_;
		terrain::HeightMapLoaderPtr hmlPtr_;
	};

} // namespace lod
} // namespace terrain
} // namespace trm
