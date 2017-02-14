#include <assimp/include/cimport.h>
#include <assimp/include/postprocess.h>
#include <assimp/include/scene.h>

#include "Globals.h"
#include "glew-2.0.0/include/GL/glew.h"

#include "Model.h"

Model::Model() {}
Model::~Model() {
	
}

void Model::Load(const char* file)
{
	scene = aiImportFile(file, aiProcess_PreTransformVertices | aiProcess_FlipUVs);
	if (scene == nullptr)
	{
		
	}
}

void Model::Clear()
{

}

void Model::Draw()
{
	for (size_t mesh = 0; mesh < scene->mNumMeshes; mesh++)
	{
		for (size_t vertex = 0; vertex +2 < scene->mMeshes[mesh]->mNumVertices; vertex+=3)
		{
			glBegin(GL_TRIANGLES);
			glVertex3f(scene->mMeshes[mesh]->mVertices[vertex].x, scene->mMeshes[mesh]->mVertices[vertex].y, scene->mMeshes[mesh]->mVertices[vertex].z);
			glVertex3f(scene->mMeshes[mesh]->mVertices[vertex+1].x, scene->mMeshes[mesh]->mVertices[vertex+1].y, scene->mMeshes[mesh]->mVertices[vertex+1].z);
			glVertex3f(scene->mMeshes[mesh]->mVertices[vertex+2].x, scene->mMeshes[mesh]->mVertices[vertex+2].y, scene->mMeshes[mesh]->mVertices[vertex+2].z);
			glEnd();
		}
	}

}
