#include "TerrainSceneObject.h"
#include "MatrixFactory.h"
#include "ModelData.h"
#include "Logger.h"

using namespace trm;

TerrainSceneObject::TerrainSceneObject(const TerrainPtr & terrainPtr)
	: terrainPtr_(terrainPtr)
	, modelMatrix_(MatrixFactory::Identity())
{
}

void 
TerrainSceneObject::Update(const WorldProjection & wp)
{
	if (!updated_)
	{
		terrainPtr_->Update(wp);
		updated_ = true;
	}
}

void
TerrainSceneObject::UpdateRequired()
{
	tasselated_ = false;
	updated_ = false;
}

void 
TerrainSceneObject::Render(const WorldProjection & wp)
{
	if (!tasselated_)
	{
		const bool tasselateMore = terrainPtr_->Tasselate(wp);
		tasselated_ = !tasselateMore;
	
		modelDrawerPool_.Release();

		ModelData modelData;
		while (terrainPtr_->Render(modelData))
		{
			if (!modelData.Valid())
			{
				continue;
			}

			modelDrawerPool_.Push(modelData);

			modelData.Clear();
		}
	}
}
		
void 
TerrainSceneObject::Draw() const
{
	modelDrawerPool_.DrawActive();
}

const Matrix &
TerrainSceneObject::GetModelMatrix() const
{
	return modelMatrix_;
}