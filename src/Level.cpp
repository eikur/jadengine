#include <assimp/include/cimport.h>
#include <assimp/include/postprocess.h>
#include <assimp/include/scene.h>

#include "Globals.h"
#include "glew-2.0.0/include/GL/glew.h"

#include "Level.h"
#include "Application.h"
#include "ModuleTextures.h"

Level::Level() {}
Level::~Level() {

}

bool Level::Load(const char* path, const char* file )
{
	std::string file_path = path; 
	file_path.append(file);;

	scene = aiImportFile(file_path.c_str(), aiProcess_Triangulate);
	if (scene == nullptr)
	{
		MYLOG("Level could not be loaded. Path: %s", file);
		return false;
	}

	root = CopyaiNode(scene->mRootNode);
	
	std::string name;
	/*float3 position = float3(0, 0, 0);
	Quat rotation = Quat(1, 0, 0, 0);
	std::vector<unsigned> meshes;
	Node* parent = nullptr;
	std::vector<Node*> children;
	*/
	

}

void Level::Clear()
{

}

void Level::Draw()
{
// old code
/*	for (std::vector<Mesh*>::const_iterator it = m_meshes.begin(); it != m_meshes.end(); ++it)
	{
		(*it)->Draw();
	}
	*/
}


Level::Node* Level::FindNode(const char* name)
{
	// to do : erase me please :)
	Node a;
	return &a;
}


void Level::LinkNode(Level::Node* node, Level::Node* new_parent) {
	// eliminar el nodo actual del vector de su padre
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
	// cambiar el parent del nodo a mover
	node->parent = new_parent;

	// añadir el nodo al vector de su nuevo padre
	if (new_parent != nullptr)
		new_parent->children.push_back(node);

}

Level::Node* Level::CopyaiNode( aiNode* origin)
{
	Node* ret = new Node();
	ret->name = origin->mName.C_Str();
	if (origin->mParent != nullptr)
		ret->parent = FindNode(origin->mName.C_Str());
	else
		ret->parent = nullptr;

	for (int i = 0; i < origin->mNumChildren; i++)
	{
		ret->children.push_back(CopyaiNode(origin->mChildren[i]));
	}
	return ret;
}
