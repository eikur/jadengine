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
	Load("what", "assets/ArmyPilot/Animations/ArmyPilot_Idle.fbx"); 
	return true; 
}

update_status ModuleAnimation::Update(float)
{
	return UPDATE_CONTINUE;
}
bool ModuleAnimation::CleanUp()
{
	MYLOG("ModuleAnimation: Freeing animations"); 
	for (AnimationMap::iterator it = animations.begin(); it != animations.end();)
	{
		for (unsigned int i = 0; i < (*it).second->num_channels; i++)
		{
			RELEASE((*it).second->channels[i].positions);
			RELEASE((*it).second->channels[i].rotations); 
		}

		delete[] (*it).second->channels;
		RELEASE((*it).second);
		animations.erase(it);
		it = animations.begin(); 
	}
	animations.clear(); 
	
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

		AnimationMap::iterator it;
		for (unsigned int i = 0; i < scene->mNumAnimations; i++)
		{
			const std::string &anim_name = scene->mAnimations[i]->mName.C_Str();
			it = animations.find(anim_name);
			if (it != animations.end())
				break;	// we found it in the map already
			else
			{
				Animation* anim = new Animation();
				animations[anim_name] = anim; 

				anim->num_channels = scene->mAnimations[i]->mNumChannels;
				anim->duration = scene->mAnimations[i]->mDuration;
				anim->channels = new NodeAnimation[anim->num_channels];
				for (unsigned int j = 0; j < anim->num_channels; j++)
				{
					anim->channels[j].name = scene->mAnimations[i]->mChannels[j]->mNodeName.C_Str();
					anim->channels[j].num_positions = scene->mAnimations[i]->mChannels[j]->mNumPositionKeys;
					anim->channels[j].num_rotations = scene->mAnimations[i]->mChannels[j]->mNumRotationKeys;

					anim->channels[j].positions = new float3[anim->channels[j].num_positions];
					for (unsigned int k = 0; k < anim->channels[j].num_positions; k++)
						anim->channels[j].positions[k] = { scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.x,
															scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.y,
															scene->mAnimations[i]->mChannels[j]->mPositionKeys[k].mValue.z};

					anim->channels[j].rotations = new Quat[anim->channels[j].num_rotations];
					for (unsigned int k = 0; k < anim->channels[j].num_rotations; k++)
						anim->channels[j].rotations[k] = { scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.x,
															scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.y,
															scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.z,
															scene->mAnimations[i]->mChannels[j]->mRotationKeys[k].mValue.w };
				}
			}
		}

	}

}