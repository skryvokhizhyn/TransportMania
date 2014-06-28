#include "Terrain.h"
#include "Terraformer.h"
#include "HeightMapLoader.h"
#include "Point2d.h"
#include <boost/numeric/conversion/cast.hpp>

using namespace trm;
using namespace trm::terrain;
using namespace trm::terrain::lod;

const size_t Terrain::PATCH_SIZE = 9u; // points
const float Terrain::MAX_HEIGHT = 40.0f;
const float Terrain::MIN_HEIGHT = -5.0f;

Terrain::Terrain(const HeightMapLoaderPtr & hmlPtr)
	:/* hmlPtr_(hmlPtr), */patchGrid_(PATCH_SIZE, boost::numeric_cast<unsigned short>((hmlPtr->getSize() - 1) / (PATCH_SIZE - 1)), hmlPtr)
{
	patchGrid_.Init();

	const size_t sz = hmlPtr->getSize();

	if (((sz - 1) % (PATCH_SIZE - 1)) != 0)
	{
		throw std::runtime_error((boost::format("Terrain size %d and patch size %d are not compatible") % sz % PATCH_SIZE).str());
	}
}

Terrain::Terrain(Terrain && t)
	: //hmlPtr_(std::move(t.hmlPtr_))
	/*,*/ patchGrid_(std::move(t.patchGrid_))
{
}

void
Terrain::Update(const WorldProjection & wp)
{
	patchGrid_.Update(wp);
}

void 
Terrain::Tasselate(const WorldProjection & wp)
{
	patchGrid_.Tasselate(wp);
}

bool
Terrain::Render(ModelData & md)
{	
	return patchGrid_.Render(md);
}

void
Terrain::Apply(Terraformer & t)
{
	t.Apply(patchGrid_);
	patchGrid_.Flush();
}