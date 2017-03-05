#ifndef __TRANSFORM_COMPONENT_H__
#define __TRANSFORM_COMPONENT_H__

#include "Component.h"
#include "MathGeoLib/include/MathGeoLib.h"

class TransformComponent : public Component
{
private:
	float3 position = float3::zero;
	float3 scale = float3::one;
	Quat rotation = Quat::identity; 
public: 
	TransformComponent(GameObject* parent, componentType type, bool active);
	~TransformComponent(); 

public: 
	void OnEditor();

};

#endif