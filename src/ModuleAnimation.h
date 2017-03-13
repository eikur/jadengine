#ifndef __MODULE_ANIMATION_H__
#define __MODULE_ANIMATION_H__

#include <vector>
#include <map>

#include "MathGeoLib/include/MathGeoLib.h"

#include "Module.h"

struct aiScene;

class ModuleAnimation : public Module
{
	// -- structs and typedefs
private:
	struct NodeAnimation
	{
		std::string name;
		float3* positions = nullptr; 
		Quat* rotations = nullptr; 

		unsigned int num_positions = 0;
		unsigned int num_rotations = 0;
	};

	struct Animation
	{
		double duration = 0; 
		unsigned int num_channels = 0; 
		NodeAnimation* channels = nullptr;
	};

	struct AnimationInstance
	{
		Animation* animation = nullptr; 
		double time_ms = 0; 
		bool loop = true; 

		AnimationInstance* next = nullptr;
		unsigned int blend_duration = 0; 
		unsigned int blend_time = 0; 
	};

	struct LessString
	{
		bool operator()(const std::string left, const std::string right) const
		{
			return ::strcmp(left.c_str(), right.c_str()) < 0;
		}
	};

	typedef unsigned int AnimationInstanceID;
	typedef std::map<std::string, Animation*, LessString> AnimationMap; 
	typedef std::vector<AnimationInstance*> InstanceList; 
	typedef std::vector<AnimationInstanceID> HoleList;

	// -- variables
private: 
	const aiScene *scene = nullptr;

	AnimationMap animations; 
	InstanceList instances; 
	HoleList holes;

public: 
	ModuleAnimation( bool active = true); 
	~ModuleAnimation(); 

	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	void LoadAnimation(const char* name, const char *file_path); 

	AnimationInstanceID Play(const char* animation_name); 
	void Stop(AnimationInstanceID instance_id); 

	bool GetTransform(AnimationInstanceID instance_id, const char* channel, float3& position, Quat& rotation) const;
};

#endif