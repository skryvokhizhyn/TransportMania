#include "TextManager.h"
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
	static const float FRAME_RATE_SCALE_FACTOR = 0.7f;
	static const float FRAME_RATE_X_MARGIN = 2.0f;
}

void
TextManager::Init(const Size2d & sz)
{
	screenSize_ = sz;

	fontData_ = FontReader::Read(trm::GetFontPath("arial_ttf_cyr_lat.fnt"));
}

void
TextManager::PutFrameRate(const unsigned rate)
{
	drawableItems_.clear();
	const std::wstring text = boost::str(boost::wformat(L"Frame rate: %d") % rate);

#ifndef DRAWING_MODE_FULL
	ModelData data = TextRenderer::Render(text);
#else
	ModelData data = TextRenderer::Render(fontData_, text, boost::numeric_cast<std::uint16_t>(screenSize_.x()/ FRAME_RATE_SCALE_FACTOR));
#endif // DRAWING_MODE_FULL
	
	Matrix scale = MatrixFactory::Scale(FRAME_RATE_SCALE_FACTOR);
	scale.at_element(0, 3) = FRAME_RATE_X_MARGIN;
	scale.at_element(1, 3) = boost::numeric_cast<float>(screenSize_.y());

	drawableItems_.emplace_back(data, std::move(scale));
}

void
TextManager::Draw(const DrawContext & c, const Matrix & orthoViewMatrix) const
{
	assert(screenSize_ != Size2d());

	boost::for_each(drawableItems_, 
		[&](const DrawableItem & tm)
	{
		c.Transform(orthoViewMatrix, tm.GetModelMatrix());
		tm.Draw();
	});
}
