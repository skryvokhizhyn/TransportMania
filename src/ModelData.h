#ifndef _MODELDATA_H_
#define _MODELDATA_H_

#include "Point2d.h"
#include "Point3d.h"
#include <vector>

namespace trm
{
	typedef std::vector<IndexType> IndexVector;
	typedef std::vector<Point3d> PointVector;
	typedef std::vector<Point3d> NormaleVector;
	typedef std::vector<Point2d> TextureCoords;

	struct ModelData
	{
		PointVector points;
		IndexVector indexes;
		NormaleVector normales;

		ModelData() 
		{}

		ModelData(ModelData && md)
			: points(std::move(md.points))
			, indexes(std::move(md.indexes))
			, normales(std::move(md.normales))
		{}

		void operator = (ModelData && md)
		{
			points = std::move(md.points);
			indexes = std::move(md.indexes);
			normales = std::move(md.normales);
		}

		void Clear()
		{
			points.clear();
			indexes.clear();
			normales.clear();
		}

		bool Valid() const
		{
			return !points.empty();
		}
	};

} // namespace trm

#endif // _MODELDATA_H_