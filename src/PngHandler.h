#pragma once

#include "ImgProperties.h"

#include <memory>
#include <string>
#include <vector>

namespace utils
{
	struct ImgArea
	{
		std::uint16_t x;
		std::uint16_t y;
		std::uint16_t width;
		std::uint16_t height;

		ImgArea(std::uint16_t a, std::uint16_t b, std::uint16_t w, std::uint16_t h);
	};

	using ImgData = std::vector<std::uint8_t>;

	class PngHandler
	{
	public:
		PngHandler(const std::string & path);

		const ImgProperties & Properties() const;
		ImgData GetData(const ImgArea & area) const;

	private:
		class HandlerImpl;
		using ImplPtr = std::shared_ptr<HandlerImpl>;

	private:
		ImplPtr implPtr_;
		ImgProperties props_;
	};

} // namespace utils
