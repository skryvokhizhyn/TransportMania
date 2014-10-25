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
		enum class Mode
		{
			Undefined,
			Point,
			Triangle,
			TriangleStrip,
			TriangleFan,
			Line,
			LineLoop
		};

		Mode type;
		PointVector points;
		IndexVector indexes;
		NormaleVector normales;

		ModelData() 
			: type(Mode::Undefined)
		{}

		ModelData(ModelData && md)
			: type(md.type)
			, points(std::move(md.points))
			, indexes(std::move(md.indexes))
			, normales(std::move(md.normales))
		{
			md.type = Mode::Undefined;
		}

		ModelData & operator = (ModelData && md)
		{
			type = md.type;
			points = std::move(md.points);
			indexes = std::move(md.indexes);
			normales = std::move(md.normales);
			md.type = Mode::Undefined;

			return *this;
		}

		void Clear()
		{
			type = Mode::Undefined;
			points.clear();
			indexes.clear();
			normales.clear();
		}

		bool Valid() const
		{
			return type != Mode::Undefined;
		}
	};

} // namespace trm

#endif // _MODELDATA_H_