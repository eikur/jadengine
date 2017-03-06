#include <assimp/include/cimport.h>
#include <assimp/include/postprocess.h>
#include <assimp/include/scene.h>

#include "Globals.h"
#include "glew-2.0.0/include/GL/glew.h"

#include "Level.h"
#include "Application.h"
#include "ModuleTextures.h"
#include "Material.h"
#include "Mesh.h"
#include "GameObject.h"


Level::Level() {}
Level::~Level() {
	// free all memory
}

bool Level::Load(const char* path, const char* file)
{
	std::string file_path = path;
	file_path.append(file);;

	scene = aiImportFile(file_path.c_str(), aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene == nullptr)
	{
		MYLOG("Level could not be loaded. Path: %s", file);
		return false;
	}
	else
	{
		LoadNode(path, scene->mRootNode, nullptr, scene);
	}
	
}

void Level::Clear()
{

}

void Level::Draw()
{
	DrawNode(root);
}


Level::Node* Level::FindNode(const char* name)
{
	Node* node = CheckNode(name, root);
	if (node == nullptr) {
		// Node not found
		MYLOG("Node not found. Name: %s", name);
		MYLOG("Returning nullptr!");
	}
	return node;
}

Level::Node* Level::CheckNode(const char* name, Node* node)
{
	if (strcmp(name, node->name.c_str()) == 0)
		return node;
	else {
		for (size_t i = 0; i < node->children.size(); ++i) {
			Node* my_node = CheckNode(name, node->children[i]);
			if (my_node != nullptr)
				return my_node;
		}
	}
	return nullptr;
}


void Level::LinkNode(Level::Node* node, Level::Node* new_parent) {
	Node* old_parent = node->parent;
	if (old_parent != nullptr)
	{
		for (std::vector<Node*>::iterator it = old_parent->children.begin(); it != old_parent->children.end(); ++it)
		{
			if ((*it) == node)
			{
				old_parent->children.erase(it);
				break;
			}
		}
	}
	node->parent = new_parent;

	if (new_parent != nullptr)
		new_parent->children.push_back(node);

}

void Level::LoadNode(const char* asset_path, const aiNode* node, Node* parent, const aiScene* scene)
{
	// Load node data
	Node* my_node = new Node();
	my_node->name = node->mName.C_Str();

	my_node->parent = parent;

	if (parent == nullptr)
		root = my_node;
	else
		parent->children.push_back(my_node);

	// Node transformations
	aiVector3D translation;
	aiVector3D scaling;
	aiQuaternion rotation;
	node->mTransformation.Decompose(scaling, rotation, translation);
	float3 pos(translation.x, translation.y, translation.z);
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
	my_node->position = pos;
	my_node->rotation = rot;

	// Load meshes for this node
	for (size_t i = 0; i < node->mNumMeshes; ++i) {
		// Load textures
		const aiMaterial* material = scene->mMaterials[scene->mMeshes[i]->mMaterialIndex];
		GLuint texture_id = 0;
		if (material->GetTextureCount(aiTextureType_DIFFUSE) > 0) {
			aiString texture_file;
			if (material->GetTexture(aiTextureType_DIFFUSE, 0, &texture_file) == AI_SUCCESS) {
				texture_id = App->textures->LoadTexture(std::string(asset_path) + texture_file.C_Str());
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

		my_node->mesh_ids.push_back(meshes.size());
		meshes.push_back(new Mesh(scene->mMeshes[node->mMeshes[i]], my_material));
	}

	// Recursively process child nodes
	for (size_t i = 0; i < node->mNumChildren; ++i)
	{
		LoadNode(asset_path, node->mChildren[i], my_node, scene);
	}
}

void Level::DrawNode(const Node *node) 
{
	glPushMatrix();


	for (std::vector<unsigned int>::const_iterator it = node->mesh_ids.begin(); it != node->mesh_ids.end(); ++it)
	{
		meshes.at(*it)->Draw();
	}

	// Draw child nodes
	for (size_t i = 0; i < node->children.size(); ++i)
	{
		DrawNode(node->children[i]);
	}

	glPopMatrix();

}

GameObject* Level::CreateGameObject(const Node *node )
{
	GameObject *go = new GameObject(node->name.c_str(), true);
	go->SetTranform(node->position, node->rotation, float3(1, 1, 1));
	return go; 

}