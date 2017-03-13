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
	MYLOG("ModuleAnimation: Freeing animations"); 
/*	for (InstanceList::iterator it = instance_list.begin(); it != instance_list.end();)
	{
		// remove animations
	}
	*/
//	instance_list.clear(); 
	return true; 
}

void ModuleAnimation::Load(const char* name, const char* file_path)
{
	scene = aiImportFile(file_path, aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene == nullptr)
	{
		MYLOG("ModuleAnimation: Could not load animation located at: %s", file_path);
	}
	else
	{
		if (scene->HasAnimations() == false)
			return; 
		


	}

}