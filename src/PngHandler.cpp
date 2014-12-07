#include "PngHandler.h"

#pragma warning(push)
#pragma warning(disable: 4611) // interaction between '_setjmp' and C++ object destruction is non-portable
#pragma warning(disable: 4127) // conditional expression is constant

// fix for: "'int_p_NULL' was not declared in this scope"
#define png_infopp_NULL (png_infopp)NULL
#define int_p_NULL (int*)NULL

#include <boost/gil/extension/io/png_io.hpp>
#include <boost/gil/typedefs.hpp>
#include <boost/numeric/conversion/cast.hpp>

#pragma warning(pop)

using namespace utils;
namespace bgil = boost::gil;

ImgArea::ImgArea(std::uint16_t a, std::uint16_t b, std::uint16_t w, std::uint16_t h)
	: x(a)
	, y(b)
	, width(w)
	, height(h)
{}

class PngHandler::HandlerImpl
{
	using ImageType = bgil::rgba8_image_t;

public:
	HandlerImpl(const std::string & path)
	{
		bgil::png_read_image(path, img_);
	}

	std::uint16_t GetWidth() const
	{
		return boost::numeric_cast<std::uint16_t>(img_.width());
	}

	std::uint16_t GetHeight() const
	{
		return boost::numeric_cast<std::uint16_t>(img_.height());
	}

	ImgData GetData(const ImgArea & area) const
	{
		ImgData ret;
		ret.reserve(area.width * area.height * bgil::num_channels<ImageType>());
		auto subView = bgil::subimage_view(bgil::const_view(img_), area.x, area.y, area.width, area.height);

		// needs assignment operator in for_each_pixel
		std::function<void (const bgil::rgba8_pixel_t &)> func =
			[&](const bgil::rgba8_pixel_t & point)
		{
			ret.push_back(bgil::get_color(point, bgil::red_t()));
			ret.push_back(bgil::get_color(point, bgil::green_t()));
			ret.push_back(bgil::get_color(point, bgil::blue_t()));
			ret.push_back(bgil::get_color(point, bgil::alpha_t()));
		};

		bgil::for_each_pixel(subView, func);

		return ret;
	}

private:
	ImageType img_;
};

PngHandler::PngHandler(const std::string & path)
	: implPtr_(std::make_shared<HandlerImpl>(path))
{
	props_.height = implPtr_->GetHeight();
	props_.width = implPtr_->GetWidth();
	props_.format = ImgFormat::Rgba8;
}

const ImgProperties &
PngHandler::Properties() const
{
	return props_;
}

ImgData 
PngHandler::GetData(const ImgArea & area) const
{
	return implPtr_->GetData(area);
}