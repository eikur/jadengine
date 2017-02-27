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
	std::string file_path(file);

	//scene = aiImportFile(file, aiProcess_PreTransformVertices | aiProcess_FlipUVs | aiProcess_Triangulate);
	scene = aiImportFile(file, aiProcess_Triangulate);
	if (scene == nullptr)
	{
		MYLOG("Level could not be loaded. Path: %s", file);
		return false;
	}
	else
	{
		

		return true;
	}
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
//	else
//		root = node;
}

