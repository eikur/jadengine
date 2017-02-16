#include <assimp/include/cimport.h>
#include <assimp/include/postprocess.h>
#include <assimp/include/scene.h>

#include "Globals.h"
#include "glew-2.0.0/include/GL/glew.h"

#include "Model.h"

Model::Model() {}
Model::~Model() {
	
}

bool Model::Load(const char* file)
{
	scene = aiImportFile(file, aiProcess_PreTransformVertices | aiProcess_FlipUVs);
	if (scene == nullptr)
	{
		MYLOG("Model could not be loaded. Path: %s", file);
		return false;
	}
	else
	{
		index = new int*[scene->mNumMeshes];
		for (int mesh = 0; mesh < scene->mNumMeshes; mesh++)
		{
			aiMesh *m = scene->mMeshes[mesh];
			index[mesh] = new int[m->mNumFaces * 3];
			for (int face = 0; face < scene->mMeshes[mesh]->mNumFaces; face++)
			{
				aiFace f = m->mFaces[face];
				for (int i = 0; i < 3; i++) {
					index[mesh][3 * face + i] = f.mIndices[i];
				}
			}
		}
		return true;
	}
}

void Model::Clear()
{

}

void Model::Draw()
{
	for (int mesh = 0; mesh < scene->mNumMeshes; mesh++)
	{
		aiMesh *m = scene->mMeshes[mesh];
		for (int face = 0; face < m->mNumFaces; face++)
		{
			aiFace f = m->mFaces[face];


			glBegin(GL_TRIANGLES);

			for (int i = 0; i < f.mNumIndices; i++) {
				int index = f.mIndices[i];
				if (m->mColors[0] != NULL)
					glColor4fv((GLfloat*)&m->mColors[0][index]);
				if (m->mNormals != NULL)
					glNormal3fv(&m->mNormals[index].x);
				glVertex3fv(&m->mVertices[index].x);
			}
			glEnd();
		}
	}
}


