#include "WindowManager.h"
#include "DrawContext.h"
#include "WindowRenderer.h"
#include "ModelData.h"
#include "MatrixFactory.h"

#include <boost/range/algorithm/for_each.hpp>

using namespace trm;

namespace
{
	const float OK_WINDOW_RELATIVE_SIZE = 0.2f;
}

void
WindowManager::Init(const size_t width, const size_t height)
{
	width_ = width;
	height_ = height;
}

void
WindowManager::CreateOKWindow()
{
	const float okWindowSize = OK_WINDOW_RELATIVE_SIZE * height_;

	ModelData md = WindowRenderer::RenderSquareWindow(okWindowSize, TextureId::OkButton);

	const float xPos = width_ / 2.0f - okWindowSize / 2;
	const float yPos = height_ / 2.0f - okWindowSize / 2;

	Matrix pos = MatrixFactory::Move(Point3d(xPos, yPos, 0.0f));

	windows_.emplace_back(md, std::move(pos));
}

void
WindowManager::Draw(const DrawContext & c, const Matrix & orthoViewMatrix) const
{
	boost::for_each(windows_, 
		[&](const DrawableItem & d)
	{
		c.Transform(orthoViewMatrix, d.GetModelMatrix());
		d.Draw();
	});
}
