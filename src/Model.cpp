#include "Model.h"
#include <assimp/include/cimport.h>
#include <assimp/include/postprocess.h>
#include <assimp/include/scene.h>

Model::Model() {}
Model::~Model() {}

void Model::Load(const char* file)
{
	scene = aiImportFile(file, aiProcess_PreTransformVertices | aiProcess_FlipUVs);
}

void Model::Clear()
{

}

void Model::Draw()
{

}
