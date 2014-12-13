#include "TextManagerHub.h"
#include "FontData.h"
#include "FontReader.h"
#include "Settings.h"

#include <boost/range/algorithm/for_each.hpp>

using namespace trm;

void
TextManagerHub::Init()
{
	fontData_ = FontReader::Read(trm::GetFontPath("arial_ttf_cyr_lat.fnt"));
}

void
TextManagerHub::PutFrameRate(const unsigned rate)
{
	textManagers_.clear();
	const std::wstring text = boost::str(boost::wformat(L"Frame rate: %d") % rate);
	textManagers_.emplace_back(fontData_, text);
}

void
TextManagerHub::Draw(const DrawContext & c, const Matrix & orthoViewMatrix) const
{
	boost::for_each(textManagers_, 
	[&](const TextManager & tm)
	{
		c.Transform(orthoViewMatrix, tm.GetModelMatrix());
		tm.Draw();
	});
}
