#include "TextManagerHub.h"

#include <boost/range/algorithm/for_each.hpp>

using namespace trm;

void
TextManagerHub::PutFrameRate(const unsigned rate)
{
	textManagers_.clear();
	textManagers_.emplace_back(boost::lexical_cast<std::wstring>(rate));
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
