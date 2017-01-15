#include "TextManager.h"
#include "TextRendererProxy.h"
#include "GlobalDefines.h"
#include "MatrixFactory.h"
#include "ModelData.h"
#include "MatrixUtils.h"
#include "WindowPosition.h"

#include <boost/range/algorithm/for_each.hpp>

using namespace trm;

namespace
{
	static const float FRAME_RATE_SCALE_FACTOR = 0.05f; // 5% fron screen height
	static const float FRAME_RATE_X_MARGIN = 2.0f;
}

void
TextManager::Init(const Size2d & sz)
{
	screenSize_ = sz;
}

void
TextManager::PutFrameRate(const unsigned rate, std::uint8_t load)
{
	drawableItems_.clear();
	const std::wstring text = boost::str(boost::wformat(L"Frame rate: %d, load rate: %d%%") % rate % load);

#ifndef DRAWING_MODE_FULL
	TextData data = TextRendererProxy()->Render(text);
#else
	TextData data = TextRendererProxy()->Render(text, screenSize_.y() * FRAME_RATE_SCALE_FACTOR);
#endif // DRAWING_MODE_FULL
	
	const Point2d pos = GetWindowPosition(screenSize_, Size2d::Cast(data.size), WindowPosition::p0, WindowPosition::p100);
	Matrix mv = MatrixFactory::Move(Point3d::Cast(pos));
	
	drawableItems_.emplace_back(std::move(data.modelData), std::move(mv));
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
