#ifndef _MODELDATA_H_
#define _MODELDATA_H_

#include "Point2d.h"
#include "Point3d.h"
#include "TextureId.h"
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
		TextureCoords textures;
		TextureId textureId;

		ModelData() 
			: type(Mode::Undefined)
		{}

		ModelData(ModelData && md)
			: type(md.type)
			, points(std::move(md.points))
			, indexes(std::move(md.indexes))
			, normales(std::move(md.normales))
			, textures(std::move(md.textures))
			, textureId(std::move(md.textureId))
		{
			md.type = Mode::Undefined;
			md.textureId = TextureId::Undefined;
		}

		ModelData & operator = (ModelData && md)
		{
			type = md.type;
			points = std::move(md.points);
			indexes = std::move(md.indexes);
			normales = std::move(md.normales);
			textures = std::move(md.textures);
			textureId = std::move(md.textureId);

			md.type = Mode::Undefined;
			md.textureId = TextureId::Undefined;

			return *this;
		}

		void Clear()
		{
			type = Mode::Undefined;
			points.clear();
			indexes.clear();
			normales.clear();
			textures.clear();
			textureId = TextureId::Undefined;
		}

		bool Valid() const
		{
			return type != Mode::Undefined;
		}
	};

} // namespace trm

#endif // _MODELDATA_H_