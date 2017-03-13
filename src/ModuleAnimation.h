#ifndef __MODULE_ANIMATION_H__
#define __MODULE_ANIMATION_H__

#include "Module.h"
#include <vector>
#include <map>
#include "MathGeoLib/include/MathGeoLib.h"

class ModuleAnimation : public Module
{
	struct AnimationChannel
	{
		std::string name;
		std::vector<float3> positions;
		std::vector<Quat> rotations; 

		unsigned int num_positions = 0;
		unsigned int num_rotations = 0;
	};

	struct Animation
	{
		unsigned int duration = 0; 
		unsigned int num_channels = 0; 
		std::vector<AnimationChannel*> channels;
	};

	struct AnimationInstance
	{
		Animation* animation = nullptr; 
		unsigned int time_ms = 0; 
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

	typedef std::map<std::string, Animation*, LessString> AnimationMap; 
	typedef std::vector<AnimationInstance*> InstanceList; 


public: 
	ModuleAnimation( bool active = true); 
	~ModuleAnimation(); 
	bool Init();
	update_status Update(float dt);
	bool CleanUp();

	void Load(const char* name, const char *file_path); 

};

#endif