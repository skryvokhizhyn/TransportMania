#pragma once

#include <cstdint>

namespace utils
{
	enum class ImgFormat
	{
		Undefined,
		Rgba8
	};

	struct ImgProperties
	{
		std::uint16_t width;
		std::uint16_t height;
		ImgFormat format;

		ImgProperties();
	};

} // namespace utils
