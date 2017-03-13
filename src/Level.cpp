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
#include "ComponentMesh.h"
#include "ComponentMaterial.h"


Level::Level() {}
Level::~Level() {
	// Release Nodes recursively
//	ReleaseNode(root);
	// Release meshes
/*	for (std::vector<Mesh*>::iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		RELEASE(*it);
	}
	*/
}

GameObject* Level::Load(const char* path, const char* file)
{
	std::string file_path = path;
	file_path.append(file);

	scene = aiImportFile(file_path.c_str(), aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene == nullptr)
	{
		MYLOG("Level could not be loaded. Path: %s", file);
		return nullptr;
	}
	else
	{
	//	LoadNode(path, scene->mRootNode, nullptr);
		return CreateGameObject(path, scene->mRootNode, nullptr);
	}
	
}

void Level::Clear()
{

}

void Level::ReleaseNode(Node* node)
{

	for (std::vector<Node*>::iterator it = node->children.begin(); it != node->children.end(); ++it)
	{
		ReleaseNode(*it);
	}
	RELEASE(node);
}

void Level::Draw()
{
	DrawNode(root);
}

void Level::DrawHierarchy()
{
	glColor3f(0.0f, 1.0f, 0.0f);
	glLineWidth(5.0f);
	DrawNodeHierarchy(root);

	glLineWidth(1.0f);
	glColor3f(1.0f, 1.0f, 1.0f);
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

void Level::LoadNode(const char* asset_path, const aiNode* node, Node* parent)
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
	float3 scale(scaling.x, scaling.y, scaling.z);
	my_node->position = pos;
	my_node->rotation = rot;
	my_node->scale = scale;

	// Load meshes for this node
	for (size_t i = 0; i < node->mNumMeshes; ++i) {
		// Load textures
		const aiMaterial* material = scene->mMaterials[scene->mMeshes[node->mMeshes[i]]->mMaterialIndex];
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
		//float shine_strength = 1.0f;

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

		my_node->mesh_ids.push_back(meshes.size());
		meshes.push_back(new Mesh(scene->mMeshes[node->mMeshes[i]], my_material));
	}

	// Recursively process children nodes
	for (size_t i = 0; i < node->mNumChildren; ++i)
	{
		LoadNode(asset_path, node->mChildren[i], my_node);
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

void Level::DrawNodeHierarchy(const Node* node)
{
	glPushMatrix();
	glTranslatef(node->position.x, node->position.y, node->position.z);
	float3 euler_rot = node->rotation.ToEulerXYZ() * 180.0f / pi;
	glRotatef(euler_rot.x, 1, 0, 0);
	glRotatef(euler_rot.y, 0, 1, 0);
	glRotatef(euler_rot.z, 0, 0, 1);

	glScalef(node->scale.x, node->scale.y, node->scale.z);

	// Draw lines to child nodes
	for (size_t i = 0; i < node->children.size(); ++i)
	{
		glBegin(GL_LINES);
		glVertex3f(0, 0, 0);
		glVertex3f(node->children[i]->position.x, node->children[i]->position.y, node->children[i]->position.z);
		glEnd();
	}

	for (size_t i = 0; i < node->children.size(); ++i)
	{
		DrawNodeHierarchy(node->children[i]);
	}

	glPopMatrix();
}

GameObject* Level::CreateGameObject(const char* path, const aiNode* origin, GameObject* parent)
{
	GameObject *game_object = new GameObject(origin->mName.C_Str(), parent, true); 

	aiVector3D translation;
	aiVector3D scaling;
	aiQuaternion rotation;

	origin->mTransformation.Decompose(scaling, rotation, translation);
	
	float3 pos(translation.x, translation.y, translation.z);
	float3 scl(scaling.x, scaling.y, scaling.z); 
	Quat rot(rotation.x, rotation.y, rotation.z, rotation.w);
	
	game_object->SetTransform(pos, rot, scl);

	ComponentMaterial *component_material = nullptr;
	ComponentMesh *component_mesh = nullptr;
	GameObject *aux_game_object = nullptr; 

	for (size_t i = 0; i < origin->mNumMeshes; ++i)
	{
		if (i == 0)
			aux_game_object = game_object;
		else
		{
			std::string aux_game_object_name = origin->mName.C_Str();
			aux_game_object_name.append(" (");
			aux_game_object_name.append(std::to_string(i));
			aux_game_object_name.append(")");
			aux_game_object = new GameObject(aux_game_object_name.c_str(), game_object, true);
			aux_game_object->SetTransform(float3::zero, Quat::identity, { 1,1,1 });	
		}

		component_material = (ComponentMaterial*)aux_game_object->CreateComponent(Component::componentType::MATERIAL);
		if (component_material != nullptr)
			component_material->LoadMaterial(scene->mMaterials[scene->mMeshes[origin->mMeshes[i]]->mMaterialIndex], path);

		component_mesh = (ComponentMesh*)aux_game_object->CreateComponent(Component::Component::MESH);
		if (component_mesh != nullptr)
		{
			if (component_material == nullptr)
				component_mesh->LoadMesh(scene->mMeshes[origin->mMeshes[i]], nullptr);
			else
			{
				component_mesh->SetComponentMaterial(component_material);
				component_mesh->LoadMesh(scene->mMeshes[origin->mMeshes[i]], component_material->GetMaterial());
			}
		}

		if (i > 0)
			game_object->AddGameObjectToChildren(aux_game_object);
	}

	// Recursively process children 
	for (size_t i = 0; i < origin->mNumChildren; ++i)
	{
		game_object->AddGameObjectToChildren(CreateGameObject(path, origin->mChildren[i], game_object));
	}
	
	return game_object;

}