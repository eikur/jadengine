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
	LoadAnimation("", "assets/ArmyPilot/Animations/ArmyPilot_Idle.fbx"); 
	return true; 
}

update_status ModuleAnimation::Update(float dt)
{
	for (InstanceList::iterator it = instances.begin(); it != instances.end(); ++it)
	{
		(*it)->time_ms += dt;
	}
	return UPDATE_CONTINUE;
}
bool ModuleAnimation::CleanUp()
{
	MYLOG("ModuleAnimation: Freeing Instances"); 
	for (InstanceList::iterator it = instances.begin(); it != instances.end(); )
	{
		RELEASE(*it); 
		it = instances.begin(); 
	}
	instances.clear(); 

	MYLOG("Freeing holes");
	for (HoleList::iterator it = holes.begin(); it != holes.end(); ++it)
		holes.erase(it); 
	holes.clear(); 
	
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

void ModuleAnimation::LoadAnimation(const char* name, const char* file_path)
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


ModuleAnimation::AnimationInstanceID ModuleAnimation::Play(const char* animation_name) 
{
	AnimationInstanceID ret = 0; 
	AnimationMap::iterator it = animations.find(animation_name);
	if (it == animations.end())
		return ret; // to be improved
	AnimationInstance *instance = new AnimationInstance();
	instance->animation = (*it).second;
	instance->time_ms = 0; 
	instance->loop = true; 
	if (holes.size() > 0)
	{
		instances.at(*holes.begin()) = instance;
		ret = *holes.begin();
		holes.erase(holes.begin());
	}
	else
	{
		instances.push_back(instance);
		ret = instances.size() - 1;
	}
	return ret; 
}

void ModuleAnimation::Stop(AnimationInstanceID instance_id)
{
	AnimationInstance *inst = instances.at(instance_id); 
	instances.at(instance_id) = nullptr;
	delete inst;

	holes.push_back(instance_id); 
	std::sort(holes.begin(), holes.end());

}

bool ModuleAnimation::GetTransform(AnimationInstanceID instance_id, const char* channel, float3& position, Quat& rotation) const
{


}