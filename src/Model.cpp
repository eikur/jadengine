#include <assimp/include/cimport.h>
#include <assimp/include/postprocess.h>
#include <assimp/include/scene.h>

#include "Globals.h"
#include "glew-2.0.0/include/GL/glew.h"

#include "Model.h"
#include "Mesh.h"

Model::Model() {}
Model::~Model() {
	for (std::vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
		RELEASE(*it);
	m_meshes.clear();
}

bool Model::Load(const char* file)
{
	scene = aiImportFile(file, aiProcess_PreTransformVertices | aiProcess_FlipUVs | aiProcess_Triangulate);
	if (scene == nullptr)
	{
		MYLOG("Model could not be loaded. Path: %s", file);
		return false;
	}
	else
	{
		for (size_t i = 0; i < scene->mNumMeshes; ++i) {
			m_meshes.push_back(new Mesh(scene->mMeshes[i]));
		}
		return true;
	}
}

void Model::Clear()
{

}

void Model::Draw()
{
	for (std::vector<Mesh*>::const_iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
	{
		(*it)->Draw();
	}
}

