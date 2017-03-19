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
	LoadAnimation("assets/ArmyPilot/Animations/", "ArmyPilot_Idle.fbx");
	LoadAnimation("assets/ArmyPilot/Animations/", "ArmyPilot_Run_Forwards.fbx");
	return true; 
}

update_status ModuleAnimation::Update(float dt)
{
	for (InstanceList::iterator it = instances.begin(); it != instances.end(); ++it)
	{
		if ((*it) != nullptr)
		{
			(*it)->time_ms += dt*1000;
			while ((*it)->time_ms > (*it)->animation->duration)
			{
				(*it)->time_ms -= (*it)->animation->duration;
			}
			if ((*it)->next != nullptr)
			{
				(*it)->blend_time += dt*1000; 
			}
		}
		
	}
	return UPDATE_CONTINUE;
}
bool ModuleAnimation::CleanUp()
{
	MYLOG("ModuleAnimation: Freeing Instances"); 
	for (InstanceList::iterator it = instances.begin(); it != instances.end(); )
	{
		if ((*it) != nullptr)
			RELEASE(*it); 
		instances.erase(it);
		it = instances.begin(); 
	}
	instances.clear(); 

	MYLOG("Freeing holes");
	for (HoleList::iterator it = holes.begin(); it != holes.end(); )
	{
		holes.erase(it);
		it = holes.begin();
	}
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

void ModuleAnimation::LoadAnimation(const char* path, const char* file)
{
	std::string file_path = path;
	file_path.append(file);

	scene = aiImportFile(file_path.c_str(), aiProcess_Triangulate | aiProcessPreset_TargetRealtime_MaxQuality);
	if (scene == nullptr)
	{
		MYLOG("ModuleAnimation: Could not load animation located at: %s", file_path.c_str());
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
				anim->duration = (float) scene->mAnimations[i]->mDuration * 16.6f;	// conversion from ticks to msec!
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
	AnimationInstanceID ret = -1; 

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

bool ModuleAnimation::IsChannelInAnimation(AnimationInstanceID instance_id, const char* channel_name) const
{
	AnimationInstance *instance = instances.at(instance_id);
	if (instance == nullptr) { return false; }

	// get the channel
	unsigned int i;
	for (i = 0; i < instance->animation->num_channels; i++)
		if (strcmp(instance->animation->channels[i].name.c_str(), channel_name) == 0)
			break;

	if (i == instance->animation->num_channels)
		return false;
	else
		return true;
}

bool ModuleAnimation::GetTransform(AnimationInstanceID instance_id, const char* channel_name, float3& position, Quat& rotation) const
{
	// get the animation
	AnimationInstance *instance = instances.at(instance_id); 
	if (instance == nullptr) { return false;  }

	// get the channel
	unsigned int i;
	for (i = 0; i < instance->animation->num_channels; i++)
		if (strcmp(instance->animation->channels[i].name.c_str(), channel_name) == 0)
			break;

	// if not found
	if (i == instance->animation->num_channels)
		return false;

	if (instance->next != nullptr && instance->blend_time > instance->blend_duration)
		return false; // blending is over

	NodeAnimation channel = instance->animation->channels[i];

	float position_key = float(instance->time_ms * (channel.num_positions)) / instance->animation->duration;
	float rotation_key = float(instance->time_ms * (channel.num_rotations)) / instance->animation->duration;

	unsigned pos_index = unsigned(position_key);
	unsigned rot_index = unsigned(rotation_key);

	float pos_lambda = position_key - float(pos_index);
	float rot_lambda = rotation_key - float(rot_index);

	float3 ret_position;
	Quat ret_rotation;

	if (pos_index == (channel.num_positions - 1))
		if (channel.num_positions == 1)
			ret_position = channel.positions[pos_index];
		else
			ret_position = InterpolateFloat3(channel.positions[pos_index], channel.positions[0], pos_lambda);
	else
		ret_position = InterpolateFloat3(channel.positions[pos_index], channel.positions[pos_index + 1], pos_lambda);

	if (rot_index == (channel.num_rotations - 1))
		if (channel.num_rotations == 1)
			ret_rotation = channel.rotations[rot_index];
		else
			ret_rotation = InterpolateQuat(channel.rotations[rot_index], channel.rotations[0], rot_lambda);
	else
		ret_rotation = InterpolateQuat(channel.rotations[rot_index], channel.rotations[rot_index + 1], rot_lambda);

	if (instance->next == nullptr)
	{
		position = ret_position; 
		rotation = ret_rotation;
	}
	else // animation blending
	{
		AnimationInstance *next = instance->next;

		NodeAnimation next_channel = next->animation->channels[i];
		float interpolation = instance->blend_time / instance->blend_duration;

		pos_index = unsigned(float(instance->time_ms * (next_channel.num_positions)) / instance->animation->duration);
		rot_index = unsigned(float(instance->time_ms * (next_channel.num_rotations)) / instance->animation->duration);

		position = InterpolateFloat3(ret_position, next_channel.positions[pos_index], interpolation);
		rotation = InterpolateQuat(ret_rotation, next_channel.rotations[rot_index], interpolation);
	}

	return true;
}

ModuleAnimation::AnimationInstanceID ModuleAnimation::BlendTo(AnimationInstanceID id, const char* next_animation_name, float blend_duration)
{
	AnimationMap::iterator it = animations.find(next_animation_name);
	if (it == animations.end())
		return -1;

	AnimationInstanceID next_id = Play(next_animation_name);
	if (next_id == -1) return -1; 
	
	instances.at(id)->next = instances.at(next_id);
	instances.at(id)->blend_duration = blend_duration;
	instances.at(id)->blend_time = 0.0f;

	return next_id;

}


float3 ModuleAnimation::InterpolateFloat3(const float3& first, const float3& second, float lambda) const
{
	return first.Lerp(second, lambda); 
//	return first*(1.0f - lambda) + second*lambda;
}

Quat ModuleAnimation::InterpolateQuat(const Quat& first, const Quat& second, float lambda) const
{
	/*Quat result;
	
	float dot = first.x*second.x + first.y*second.y + first.z*second.z + first.w*second.w;

	if (dot >= 0.0f)
	{
		result.x = first.x*(1.0f - lambda) + second.x*lambda;
		result.y = first.y*(1.0f - lambda) + second.y*lambda;
		result.z = first.z*(1.0f - lambda) + second.z*lambda;
		result.w = first.w*(1.0f - lambda) + second.w*lambda;
	}
	else
	{
		result.x = first.x*(1.0f - lambda) - second.x*lambda;
		result.y = first.y*(1.0f - lambda) - second.y*lambda;
		result.z = first.z*(1.0f - lambda) - second.z*lambda;
		result.w = first.w*(1.0f - lambda) - second.w*lambda;
	}
	return result;*/
	return first.Lerp(second, lambda);
}