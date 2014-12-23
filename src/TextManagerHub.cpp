#include "TextManagerHub.h"
#include "FontData.h"
#include "FontReader.h"
#include "Settings.h"
#include "TextRenderer.h"
#include "GlobalDefines.h"
#include "MatrixFactory.h"
#include "ModelData.h"
#include "MatrixUtils.h"

#include <boost/range/algorithm/for_each.hpp>

using namespace trm;

namespace
{
	static const float FRAME_RATE_SCALE_FACTOR = 0.5f;
	static const float FRAME_RATE_X_MARGIN = 2.0f;
}

TextManagerHub::TextManagerHub()
	: width_(0)
	, height_(0)
{}

void
TextManagerHub::Init(const size_t width, const size_t height)
{
	width_ = width;
	height_ = height;

	fontData_ = FontReader::Read(trm::GetFontPath("arial_ttf_cyr_lat.fnt"));
}

void
TextManagerHub::PutFrameRate(const unsigned rate)
{
	textManagers_.clear();
	const std::wstring text = boost::str(boost::wformat(L"Frame rate: %d") % rate);

#ifndef DRAWING_MODE_FULL
	ModelData data = TextRenderer::Render(text);
#else
	ModelData data = TextRenderer::Render(fontData_, text, boost::numeric_cast<std::uint16_t>(width_ / FRAME_RATE_SCALE_FACTOR));
#endif // DRAWING_MODE_FULL
	
	Matrix scale = MatrixFactory::Scale(FRAME_RATE_SCALE_FACTOR);
	scale.at_element(0, 3) = FRAME_RATE_X_MARGIN;
	scale.at_element(1, 3) = boost::numeric_cast<float>(height_);

	textManagers_.emplace_back(data, std::move(scale));
}

void
TextManagerHub::Draw(const DrawContext & c, const Matrix & orthoViewMatrix) const
{
	assert(width_);
	assert(height_);

	boost::for_each(textManagers_, 
		[&](const TextManager & tm)
	{
		c.Transform(orthoViewMatrix, tm.GetModelMatrix());
		tm.Draw();
	});
}
