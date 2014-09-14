#include "TextRenderer.h"
#include "ModelData.h"

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

	return md;
}