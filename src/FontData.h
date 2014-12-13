#pragma once

#include <boost/noncopyable.hpp>

#include <string>
#include <cstdint>
#include <vector>

namespace trm
{
	struct FontData
		: boost::noncopyable
	{
		std::string name;
		std::string file;
		std::uint8_t size;

		struct Char
		{
			uint16_t id;
			std::uint16_t x;
			std::uint16_t y;
			std::uint8_t width;
			std::uint8_t height;
			std::uint8_t xoffset;
			std::uint8_t yoffset;
			std::uint8_t xadvance;

			Char() 
				: id(0), x(0), y(0), width(0)
				, height(0), xoffset(0), yoffset(0), xadvance(0)
			{}
		};

		struct Kerning
		{
			uint16_t first;
			uint16_t second;
			std::uint8_t amount;

			Kerning()
				: first(0), second(0), amount(0)
			{}
		};

		using Chars = std::vector<Char>;
		Chars chars;

		using Kernings = std::vector<Kerning>;
		Kernings kernings;

		FontData()
			: size(0)
		{}

		FontData(FontData && other)
			: name(std::move(other.name))
			, file(std::move(other.file))
			, size(std::move(other.size))
			, chars(std::move(other.chars))
			, kernings(std::move(other.kernings))
		{}

		FontData & operator = (FontData && other)
		{
			name = std::move(other.name);
			file = std::move(other.file);
			size = std::move(other.size);
			chars = std::move(other.chars);
			kernings = std::move(other.kernings);

			other.size = 0;

			return *this;
		}
	};

} // namespace trm
