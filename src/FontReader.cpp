#include "FontReader.h"
#include "FontData.h"

#pragma warning(push)
#pragma warning(disable: 6262)

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#pragma warning(pop)

#include <boost/range/algorithm/transform.hpp>
#include <boost/range/algorithm/sort.hpp>
#include <boost/range/adaptor/map.hpp>
#include <boost/range/adaptor/filtered.hpp>

#include <fstream>
#include <functional>

using namespace trm;
using namespace boost::property_tree;

namespace
{
	FontData::Char
	ParseChar(const ptree & node)
	{
		FontData::Char ch;

		ch.id = node.get<decltype(FontData::Char::id)>("<xmlattr>.id");
		ch.x = node.get<decltype(FontData::Char::x)>("<xmlattr>.x");
		ch.y = node.get<decltype(FontData::Char::y)>("<xmlattr>.y");
		ch.width = node.get<decltype(FontData::Char::width)>("<xmlattr>.width");
		// extend height for 1 pixel as it cuts off it from the glyph
		ch.height = node.get<decltype(FontData::Char::height)>("<xmlattr>.height") + 1;
		ch.xoffset = node.get<decltype(FontData::Char::xoffset)>("<xmlattr>.xoffset");
		ch.yoffset = node.get<decltype(FontData::Char::yoffset)>("<xmlattr>.yoffset");
		ch.xadvance = node.get<decltype(FontData::Char::xadvance)>("<xmlattr>.xadvance");

		return ch;
	}

	FontData::Kerning
	ParseKerning(const ptree & node)
	{
		FontData::Kerning kn;
		kn.first = node.get<decltype(FontData::Kerning::first)>("<xmlattr>.first");
		kn.second = node.get<decltype(FontData::Kerning::second)>("<xmlattr>.second");
		kn.amount = node.get<decltype(FontData::Kerning::amount)>("<xmlattr>.amount");

		return kn;
	}

	auto keyFilter = [](const ptree::value_type & vt)
	{
		if (vt.first == "<xmlattr>")
			return false;

		return true;
	};
}

FontData
FontReader::Read(const std::string & path)
{
	FontData fd;
	
	std::ifstream file(path.c_str(), std::ifstream::in);
	ptree tree;
	read_xml(file, tree);

	auto & root = tree.get_child("font");
	fd.name = root.get<std::string>("info.<xmlattr>.face");
	fd.file = root.get_child("pages").front().second.get<std::string>("<xmlattr>.file");
	fd.size = root.get<std::uint8_t>("info.<xmlattr>.size");
	
	auto & chars = root.get_child("chars");
	fd.chars.reserve(chars.get<int>("<xmlattr>.count"));

	boost::transform(chars | boost::adaptors::filtered(keyFilter) | boost::adaptors::map_values, 
		std::back_inserter(fd.chars), boost::bind(&ParseChar, _1));

	boost::sort(fd.chars, 
		[](const FontData::Char & l, const FontData::Char & r)
	{
		return l.id < r.id;
	});

	auto & kernings = root.get_child("kernings");
	fd.kernings.reserve(kernings.get<int>("<xmlattr>.count"));
	boost::transform(kernings | boost::adaptors::filtered(keyFilter) | boost::adaptors::map_values, 
		std::back_inserter(fd.kernings), boost::bind(&ParseKerning, _1));
	
	return fd;
}