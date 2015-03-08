#pragma once

#include "DrawableObject.h"
#include "ModelData.h"
#include "Point2d.h"

#pragma warning(push)
// 4503 decorated name length exceeded, name was truncated
// 4512 assignment operator could not be generated
#pragma warning(disable: 4503 4512)

#include <boost/bimap.hpp>

#include <map>

namespace trm
{
	class TerrainCoverDrawableObject
		: public DrawableObject
	{
	public:
		using PointMap = boost::bimaps::bimap<Point2d, int>;
		using NormaleMap = std::map<int, Point3d>;

	public:
		TerrainCoverDrawableObject(const PointVector & points);

		virtual void Render(ModelData & md) override;

		static IndexVector GetTriangulationIndexes(const PointMap & pm);
		static NormaleMap GetNormales(const PointVector & pv, const IndexVector & iv);

	private:
		const PointVector & points_;
	};

} // namespace trm

#pragma warning(pop)