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
	terrainPtr_->Update(wp);
	terrainPtr_->Tasselate(wp);
}

void 
TerrainSceneObject::Render()
{
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