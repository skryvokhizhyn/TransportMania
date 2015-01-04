#include "TextRenderer.h"
#include "ModelData.h"
#include "FontData.h"

#pragma warning(disable: 4512)

#include <boost/range/algorithm/for_each.hpp>
#include <boost/range/algorithm/transform.hpp>
#include <boost/lambda/lambda.hpp>

using namespace trm;

namespace
{
	template<typename PointV>
	IndexVector GenerateLinearIndexes(const PointV & pv)
	{
		IndexVector ret;
		const size_t sz = pv.size() - 1;
		ret.reserve(sz * 2);
		
		for (size_t i = 0; i < sz; ++i)
		{
			ret.push_back(i);
			ret.push_back(i + 1);
		}

		return ret;
	}

	// ?
	const auto PointVectorQuestion = {
		Point3d(-0.4f, 0.5f, 0.0f), 
		Point3d(-0.4f, 0.9f, 0.0f), 
		Point3d(0.4f, 0.9f, 0.0f), 
		Point3d(0.4f, 0.0f, 0.0f), 
		Point3d(0.0f, 0.0f, 0.0f), 
		Point3d(0.0f, -0.5f, 0.0f), 
		Point3d(0.0f, -0.9f, 0.0f)};
	const auto IndexVectorQuestion = GenerateLinearIndexes(PointVectorQuestion);

	// 1
	const auto PointVector1 = {
		Point3d(-0.4f, 0.0f, 0.0f),
		Point3d(0.0f, 0.9f, 0.0f),
		Point3d(0.0f, -0.9f, 0.0f)};
	const auto IndexVector1 = GenerateLinearIndexes(PointVector1);

	// 2
	const auto PointVector2 = {
		Point3d(-0.4f, 0.0f, 0.0f),
		Point3d(-0.4f, 0.9f, 0.0f),
		Point3d(0.4f, 0.9f, 0.0f),
		Point3d(0.4f, 0.0f, 0.0f),
		Point3d(-0.4f, -0.9f, 0.0f),
		Point3d(0.4f, -0.9f, 0.0f)};
	const auto IndexVector2 = GenerateLinearIndexes(PointVector2);

	// 3
	const auto PointVector3 = {
		Point3d(-0.4f, 0.9f, 0.0f),
		Point3d(0.4f, 0.9f, 0.0f),
		Point3d(0.4f, 0.0f, 0.0f),
		Point3d(-0.4f, 0.0f, 0.0f),
		Point3d(0.4f, 0.0f, 0.0f),
		Point3d(0.4f, -0.9f, 0.0f),
		Point3d(-0.4f, -0.9f, 0.0f)
	};
	const auto IndexVector3 = GenerateLinearIndexes(PointVector3);

	// 4
	const auto PointVector4 = {
		Point3d(0.4f, 0.0f, 0.0f),
		Point3d(-0.4f, 0.0f, 0.0f),
		Point3d(0.0f, 0.9f, 0.0f),
		Point3d(0.0f, -0.9f, 0.0f)};
	const auto IndexVector4 = GenerateLinearIndexes(PointVector4);

	// 5
	const auto PointVector5 = {
		Point3d(0.4f, 0.9f, 0.0f),
		Point3d(-0.4f, 0.9f, 0.0f),
		Point3d(-0.4f, 0.0f, 0.0f),
		Point3d(0.4f, 0.0f, 0.0f),
		Point3d(0.4f, -0.9f, 0.0f),
		Point3d(-0.4f, -0.9f, 0.0f)};
	const auto IndexVector5 = GenerateLinearIndexes(PointVector5);

	// 6
	const auto PointVector6 = {
		Point3d(0.4f, 0.9f, 0.0f),
		Point3d(-0.4f, 0.9f, 0.0f),
		Point3d(-0.4f, -0.9f, 0.0f),
		Point3d(0.4f, -0.9f, 0.0f),
		Point3d(0.4f, 0.0f, 0.0f),
		Point3d(-0.4f, 0.0f, 0.0f)};
	const auto IndexVector6 = GenerateLinearIndexes(PointVector6);

	// 7
	const auto PointVector7 = {
		Point3d(-0.4f, 0.9f, 0.0f),
		Point3d(0.4f, 0.9f, 0.0f),
		Point3d(-0.4f, -0.9f, 0.0f)};
	const auto IndexVector7 = GenerateLinearIndexes(PointVector7);
	
	// 8
	const auto PointVector8 = {
		Point3d(-0.4f, -0.9f, 0.0f),
		Point3d(-0.4f, 0.9f, 0.0f),
		Point3d(0.4f, 0.9f, 0.0f),
		Point3d(0.4f, -0.9f, 0.0f),
		Point3d(-0.4f, -0.9f, 0.0f),
		Point3d(-0.4f, 0.0f, 0.0f),
		Point3d(0.4f, 0.0f, 0.0f)};
	const auto IndexVector8 = GenerateLinearIndexes(PointVector8);

	// 9
	const auto PointVector9 = {
		Point3d(0.4f, 0.0f, 0.0f),
		Point3d(-0.4f, 0.0f, 0.0f),
		Point3d(-0.4f, 0.9f, 0.0f),
		Point3d(0.4f, 0.9f, 0.0f),
		Point3d(0.4f, 0.0f, 0.0f),
		Point3d(-0.4f, -0.9f, 0.0f)};
	const auto IndexVector9 = GenerateLinearIndexes(PointVector9);

	// 0
	const auto PointVector0 = {
		Point3d(-0.4f, -0.9f, 0.0f),
		Point3d(-0.4f, 0.9f, 0.0f),
		Point3d(0.4f, 0.9f, 0.0f),
		Point3d(0.4f, -0.9f, 0.0f),
		Point3d(-0.4f, -0.9f, 0.0f)};
	const auto IndexVector0 = GenerateLinearIndexes(PointVector0);

	struct CharMapper
	{
		std::reference_wrapper<trm::ModelData> mdRef;
		float shift;

		CharMapper(ModelData & m)
			: mdRef(m), shift(0.0f)
		{}

		void operator () (wchar_t ch)
		{
			switch (ch)
			{
			case L'1':
				PutCharModel(PointVector1, IndexVector1);
				break;
			case L'2':
				PutCharModel(PointVector2, IndexVector2);
				break;
			case L'3':
				PutCharModel(PointVector3, IndexVector3);
				break;
			case L'4':
				PutCharModel(PointVector4, IndexVector4);
				break;
			case L'5':
				PutCharModel(PointVector5, IndexVector5);
				break;
			case L'6':
				PutCharModel(PointVector6, IndexVector6);
				break;
			case L'7':
				PutCharModel(PointVector7, IndexVector7);
				break;
			case L'8':
				PutCharModel(PointVector8, IndexVector8);
				break;
			case L'9':
				PutCharModel(PointVector9, IndexVector9);
				break;
			case L'0':
				PutCharModel(PointVector0, IndexVector0);
				break;
			case L'?':
			default:
				PutCharModel(PointVectorQuestion, IndexVectorQuestion);
			}

			shift += 1.0f;
		}

		template<typename PointV, typename IndexV>
		void PutCharModel(const PointV & pv, const IndexV & iv)
		{
			using namespace boost::lambda;
			placeholder1_type Arg1;
			trm::ModelData & md = mdRef;

			md.points.reserve(md.points.size() + pv.size());
			boost::transform(pv, std::back_inserter(md.points), ret<Point3d>(Arg1 + Point3d(shift, 0.0f, 0.0f)));

			md.indexes.reserve(md.indexes.size() + pv.size());
			boost::transform(iv, std::back_inserter(md.indexes), 
				ret<int>(Arg1 + (md.indexes.empty() ? 0 : md.indexes.back() + 1)));
		}
	};
}

ModelData
TextRenderer::Render(const std::wstring & str)
{
	ModelData md;

	boost::for_each(str, CharMapper(md));
	md.type = ModelData::Mode::Line;

	return md;
}

#include <TextureManagerProxy.h>
#include <boost/range/algorithm/lower_bound.hpp>
#include <boost/assign/std/vector.hpp>

namespace
{
	ModelData GenerateCharModelData(const FontData::Char & foundChar, const utils::ImgProperties & imgProps)
	{
		using namespace boost::assign;

		ModelData charData;
		charData.points.reserve(4);
		charData.textures.reserve(4);
		charData.indexes.reserve(6);

		charData.points +=
			Point3d(0, 0, 0),
			Point3d(0, foundChar.height, 0),
			Point3d(foundChar.width, foundChar.height, 0),
			Point3d(foundChar.width, 0, 0);

		charData.indexes += 0, 2, 1, 0, 3, 2;

		charData.textures +=
			Point2d(0, foundChar.height),
			Point2d(0, 0),
			Point2d(foundChar.width, 0),
			Point2d(foundChar.width, foundChar.height);

		boost::transform(charData.textures, charData.textures.begin(),
			[&](const Point2d & p)
		{
			return p + Point2d(foundChar.x, foundChar.y);
		});

		boost::transform(charData.textures, charData.textures.begin(),
			[&](const Point2d & p)
		{
			return Point2d(p.x() / imgProps.width, p.y() / imgProps.height);
		});

		return charData;
	}

	void AppendCharData(ModelData && charData, const std::uint16_t xShift, const std::uint16_t yShift, ModelData & globalData)
	{
		boost::transform(charData.points, charData.points.begin(),
			[&](const Point3d & p)
		{
			return p + Point3d(xShift, static_cast<float>(-yShift), 0);
		});

		boost::transform(charData.indexes, charData.indexes.begin(), 
			boost::bind(std::plus<IndexVector::value_type>(), _1, globalData.indexes.size() / 6 * 4));

		globalData.points.insert(globalData.points.end(), charData.points.begin(), charData.points.end());
		globalData.indexes.insert(globalData.indexes.end(), charData.indexes.begin(), charData.indexes.end());
		globalData.textures.insert(globalData.textures.end(), charData.textures.begin(), charData.textures.end());
	}

	FontData::Chars::const_iterator FindCharData(wchar_t c, const FontData::Chars & chars)
	{
		static const auto comparer = 
			[](const FontData::Char & l, const FontData::Char & r)
		{
			return l.id < r.id;
		};

		FontData::Char ch; ch.id = c;

		auto found = boost::lower_bound(chars, ch, comparer);

		if (found == chars.end())
		{
			return found;
		}

		if (found->id != c)
		{
			return chars.end();
		}

		return found;
	}

	const FontData::Char & GetCharData(wchar_t c, const FontData::Chars & chars)
	{
		auto found = FindCharData(c, chars);

		if (found == chars.end())
		{
			found = FindCharData(L'?', chars);

			if (found == chars.end())
			{
				throw std::runtime_error(boost::str(boost::format("Failed to find symbol with code '%d' and backup symbol '?'") % c));
			}
		}

		return *found;
	}
}

ModelData
TextRenderer::Render(const FontData & fd, const std::wstring & str, const std::uint16_t maxWidth)
{
	ModelData md;

	md.type = ModelData::Mode::Triangle;
	md.textureId = TextureId::Font;

	const size_t stringSize = str.size();
	md.points.reserve(4 * stringSize);
	md.textures.reserve(4 * stringSize);
	md.indexes.reserve(6 * stringSize);

	const utils::ImgProperties & imgProps = TextureManagerProxy()->Get(TextureId::Font).props;

	std::uint16_t xShift = 0;
	std::uint16_t yShift = fd.size;

	for (wchar_t c : str)
	{
		const FontData::Char & ch = GetCharData(c, fd.chars);

		if (c == L'\n' || xShift + ch.xadvance >= maxWidth)
		{
			if (xShift == 0)
			{
				throw std::runtime_error("Maximum text width is smaller than one character");
			}

			xShift = 0;
			yShift += fd.size;

			continue;
		}

		ModelData charData = GenerateCharModelData(ch, imgProps);
		AppendCharData(std::move(charData), xShift, yShift, md);
		xShift += ch.xadvance;
	}

	return md;
}