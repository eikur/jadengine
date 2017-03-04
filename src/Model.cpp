#include <assimp/include/cimport.h>
#include <assimp/include/postprocess.h>
#include <assimp/include/scene.h>

#include "Globals.h"
#include "glew-2.0.0/include/GL/glew.h"

#include "Model.h"
#include "Mesh.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "Material.h"

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
			Material* my_material = new Material(texture_id);
			aiColor3D color(0.0f, 0.0f, 0.0f);
			float shininess = 1.0f;
			float shine_strength = 1.0f;

			if (material->Get(AI_MATKEY_COLOR_AMBIENT, color) == AI_SUCCESS)
				my_material->SetColor({ color.r, color.g, color.b, 1.0f }, Material::COLOR_COMPONENT::AMBIENT);
			if (material->Get(AI_MATKEY_COLOR_DIFFUSE, color) == AI_SUCCESS)
				my_material->SetColor({ color.r, color.g, color.b, 1.0f }, Material::COLOR_COMPONENT::DIFFUSE);
			if (material->Get(AI_MATKEY_COLOR_EMISSIVE, color) == AI_SUCCESS)
				my_material->SetColor({ color.r, color.g, color.b, 1.0f }, Material::COLOR_COMPONENT::EMISSIVE);
			if (material->Get(AI_MATKEY_COLOR_SPECULAR, color) == AI_SUCCESS)
				my_material->SetColor({ color.r, color.g, color.b, 1.0f }, Material::COLOR_COMPONENT::SPECULAR);
			if (material->Get(AI_MATKEY_COLOR_TRANSPARENT, color) == AI_SUCCESS)
				my_material->SetColor({ color.r, color.g, color.b, 1.0f }, Material::COLOR_COMPONENT::TRANSPARENT);

			if (material->Get(AI_MATKEY_SHININESS, shininess) == AI_SUCCESS)
				my_material->SetShininess(shininess * 128.0f);

			/*if (material->Get(AI_MATKEY_SHININESS_STRENGTH, shine_strength) == AI_SUCCESS)*/
				
			/*aiReturn hasShiness = material->Get(AI_MATKEY_SHININESS, color);
			aiReturn hasStrength = material->Get(AI_MATKEY_SHININESS_STRENGTH, color);*/

			m_meshes.push_back(new Mesh(scene->mMeshes[i], my_material));
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

