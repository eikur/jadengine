#include <assimp/include/cimport.h>
#include <assimp/include/postprocess.h>
#include <assimp/include/scene.h>

#include "Globals.h"
#include "glew-2.0.0/include/GL/glew.h"

#include "Model.h"
#include "Mesh.h"
#include "Application.h"
#include "ModuleTextures.h"

Model::Model() {}
Model::~Model() {
	for (std::vector<Mesh*>::iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
		RELEASE(*it);
	m_meshes.clear();
}

bool Model::Load(const char* file)
{
	std::string file_path(file);
	
	//scene = aiImportFile(file, aiProcess_PreTransformVertices | aiProcess_FlipUVs | aiProcess_Triangulate);
	scene = aiImportFile(file, aiProcess_PreTransformVertices | aiProcess_Triangulate);
	if (scene == nullptr)
	{
		MYLOG("Model could not be loaded. Path: %s", file);
		return false;
	}
	else
	{
		// Load meshes
		for (size_t i = 0; i < scene->mNumMeshes; ++i) {
			// Load textures
			const aiMaterial* material = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
			GLuint texture_id = 0;
			if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
				aiString path;
				if (material->GetTexture(aiTextureType_DIFFUSE, 0, &path) == AI_SUCCESS) {
					std::string full_path(path.C_Str());
					full_path = file_path.substr(0, file_path.rfind('/') + 1) + full_path;
					texture_id = App->textures->LoadTexture(full_path);
					m_textures.push_back(App->textures->LoadTexture(full_path));
				}
			}
			m_meshes.push_back(new Mesh(scene->mMeshes[i], texture_id));
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

