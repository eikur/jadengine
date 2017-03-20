#ifndef __TRANSFORM_COMPONENT_H__
#define __TRANSFORM_COMPONENT_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class ComponentTransform : public Component
{
public:
	float3 position = float3::zero;
	float3 scale = float3::one;
	Quat rotation = Quat::identity; 

private: 
	float3 last_scale = float3::one;
	Quat last_rotation = Quat::identity;

public: 
	ComponentTransform(GameObject* parent, bool active);
	~ComponentTransform(); 

public: 
	bool Update(float dt);
	bool CleanUp(); 
	void OnEditor();

	void SetTransform(float3 new_pos, Quat new_rot, float3 new_scale);
	float4x4 GetTransform(); 

};

#endif