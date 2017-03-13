#include <assimp/include/cimport.h>
#include <assimp/include/postprocess.h>
#include <assimp/include/scene.h>

#include "Globals.h"
#include "Application.h"

#include "ModuleAnimation.h"


ModuleAnimation::ModuleAnimation(bool active) : Module(active) {}

ModuleAnimation::~ModuleAnimation() {}

bool ModuleAnimation::Init()
{
	return true; 
}

update_status ModuleAnimation::Update(float)
{
	return UPDATE_CONTINUE;
}
bool ModuleAnimation::CleanUp()
{
	return true; 
}

void ModuleAnimation::Load(const char* name, const char* file_path)
{
	const aiScene *scene; 
	scene = aiImportFile(file_path, aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene == nullptr)
	{
//		MYLOG("Level could not be loaded. Path: %s", file_path);

	}
	else
	{
//		LoadNode(path, scene->mRootNode, nullptr);

	}

}