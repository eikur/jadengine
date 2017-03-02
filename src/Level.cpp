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
	// free all memory
}

bool Level::Load(const char* path, const char* file)
{
	std::string file_path = path;
	file_path.append(file);;

	scene = aiImportFile(file_path.c_str(), aiProcess_Triangulate);
	if (scene == nullptr)
	{
		MYLOG("Level could not be loaded. Path: %s", file);
		return false;
	}

	CopyAllMeshes(scene);
	CopyAllMaterials(scene);
	LoadNodes(scene->mRootNode);

	int numMeshes = scene->mNumMeshes;
	vertex_array_ids = new unsigned int[numMeshes];
	glGenBuffers(numMeshes, vertex_array_ids);

	int i = 0;
	for (std::vector<Mesh>::const_iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		glBindBuffer(GL_ARRAY_BUFFER, vertex_array_ids[i]);
		glBufferData(GL_ARRAY_BUFFER, sizeof(float)*(*it).num_vertices * 3, (*it).vertices, GL_STATIC_DRAW);
		i++;
	}
	
}

void Level::Clear()
{

}

void Level::Draw()
{
	int i = 0; 
	for (std::vector<Mesh>::const_iterator it = meshes.begin(); it != meshes.end(); ++it)
	{
		glEnableClientState(GL_VERTEX_ARRAY);
		glBindBuffer(GL_ARRAY_BUFFER, vertex_array_ids[i]);
		glVertexPointer(3, GL_FLOAT, 0, NULL);
		glDrawArrays(GL_QUADS, 0, (*it).num_vertices*4);
		glDisableClientState(GL_VERTEX_ARRAY);
		i++;
	}
}


Level::Node* Level::FindNode(const char* name)
{
	return nullptr;
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

Level::Node* Level::LoadNodes(const  aiNode* origin)
{
	Node* ret;
	if (origin->mParent == nullptr)
	{
		root = new Node();
		ret = root; 
	}
	else
		ret = new Node();
	
	ret->name = origin->mName.C_Str();
	if (origin->mParent != nullptr)
		ret->parent = FindNode(origin->mParent->mName.C_Str());
	else
		ret->parent = nullptr;

	// load meshes and materials for each node here
	for (unsigned int i = 0; i < origin->mNumMeshes; i++)
	{
		ret->meshes.push_back(origin->mMeshes[i]);
	}

	//recursive for children
	for (int i = 0; i < origin->mNumChildren; i++)
	{
		ret->children.push_back(LoadNodes(origin->mChildren[i]));
	}
	return ret;
}

void Level::CopyAllMeshes(const aiScene* scn)
{
	for (unsigned int i = 0; i < scn->mNumMeshes; i++)
	{
		Mesh tmp;
		tmp.material = scn->mMeshes[i]->mMaterialIndex;
		tmp.num_vertices = scn->mMeshes[i]->mNumVertices;

		tmp.vertices = new float3[tmp.num_vertices];
		tmp.tex_coords = new float3[tmp.num_vertices];
		tmp.normals = new float3[tmp.num_vertices];
		for (unsigned int j = 0; j < tmp.num_vertices; j++)
		{
			tmp.vertices[j].x = scn->mMeshes[i]->mVertices[j].x;
			tmp.vertices[j].y = scn->mMeshes[i]->mVertices[j].y;
			tmp.vertices[j].z = scn->mMeshes[i]->mVertices[j].z;

			if (scn->mMeshes[i]->mTextureCoords[0] != nullptr)
			{
				tmp.tex_coords[j].x = scn->mMeshes[i]->mTextureCoords[0]->x;
				tmp.tex_coords[j].y = scn->mMeshes[i]->mTextureCoords[0]->y;
				tmp.tex_coords[j].z = scn->mMeshes[i]->mTextureCoords[0]->z;
			}

			tmp.normals[j].x = scn->mMeshes[i]->mNormals[j].x;
			tmp.normals[j].y = scn->mMeshes[i]->mNormals[j].y;
			tmp.normals[j].z = scn->mMeshes[i]->mNormals[j].z;
		}

		tmp.num_indices = 1;
		tmp.index = i;
		
		meshes.push_back(tmp);
	}
}
void Level::CopyAllMaterials(const aiScene* scn)
{
	aiColor4D tmpColor;
	for (unsigned int i = 0; i < scn->mNumMaterials; i++)
	{
		Material mat;

		if (scn->mMaterials[i]->Get(AI_MATKEY_COLOR_AMBIENT, tmpColor) == aiReturn_SUCCESS)
			mat.ambient.x = tmpColor.r; mat.ambient.y = tmpColor.g, mat.ambient.z = tmpColor.b; mat.ambient.w = tmpColor.a;
		if (scn->mMaterials[i]->Get(AI_MATKEY_COLOR_DIFFUSE, tmpColor) != aiReturn_SUCCESS)	
			mat.diffuse.x = tmpColor.r; mat.diffuse.y = tmpColor.g, mat.diffuse.z = tmpColor.b; mat.diffuse.w = tmpColor.a;
		if (scn->mMaterials[i]->Get(AI_MATKEY_COLOR_SPECULAR, tmpColor) != aiReturn_SUCCESS)
			mat.specular.x = tmpColor.r; mat.specular.y = tmpColor.g; mat.specular.z = tmpColor.b; mat.specular.w = tmpColor.a;

		if (scn->mMaterials[i]->Get(AI_MATKEY_SHININESS_STRENGTH, mat.shininess) != aiReturn_SUCCESS)
			break;

		// texture not loaded
		/*
		unsigned int texture = 0;
		*/
		materials.push_back(mat);
	}
}
